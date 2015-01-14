/*
Copyright (C) 2013 Adapteva, Inc.
Contributed by Yaniv Sapir <support@adapteva.com>
Contributed by Andreas Olofsson <support@adapteva.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program, see the file COPYING. If not, see
<http://www.gnu.org/licenses/>.
*/

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdint.h>

#include <e-hal.h>
#include <e-loader.h>

int main(int argc, char *argv[])
{
	e_epiphany_t dev;
	e_platform_t platform;
	uint32_t local_read, chip_read, eram_read;
	uint32_t local_write, chip_write, eram_write;
	uint32_t local_sread, chip_sread, eram_sread;
	uint32_t local_swrite, chip_swrite, eram_swrite;
	uint32_t local_read_16, chip_read_16, eram_read_16;
	uint32_t local_write_16, chip_write_16, eram_write_16;
	uint32_t local_sread_16, chip_sread_16, eram_sread_16;
	uint32_t local_swrite_16, chip_swrite_16, eram_swrite_16;
	int fail = 0;
	uint32_t bad = 0xbaadc0de;


	e_init(NULL);
	e_get_platform_info(&platform);
	e_reset_system();
	e_open(&dev, 0, 0, platform.rows, platform.cols);

	e_write(&dev, 0, 0, 0x7000, &bad, sizeof(uint32_t));

	e_load_group("./bin/elink-test1-device.elf" , &dev, 0, 0, 1, 1, E_TRUE);

	//Lazy way of waiting till finished
	sleep(1);

	e_read(&dev, 0, 0, 0x7000, &local_read, sizeof(uint32_t));
	e_read(&dev, 0, 0, 0x7004, &chip_read, sizeof(uint32_t));
	e_read(&dev, 0, 0, 0x7008, &eram_read, sizeof(uint32_t));
	if (local_read != chip_read || local_read != eram_read)
		fail++;

	e_read(&dev, 0, 0, 0x700c, &local_write, sizeof(uint32_t));
	e_read(&dev, 0, 0, 0x7010, &chip_write, sizeof(uint32_t));
	e_read(&dev, 0, 0, 0x7014, &eram_write, sizeof(uint32_t));
	if (local_write != chip_write || local_write != eram_write)
		fail++;

	e_read(&dev, 0, 0, 0x7018, &local_sread, sizeof(uint32_t));
	e_read(&dev, 0, 0, 0x701C, &chip_sread, sizeof(uint32_t));
	e_read(&dev, 0, 0, 0x7020, &eram_sread, sizeof(uint32_t));
	if (local_sread != chip_sread || local_sread != eram_sread)
		fail++;

	e_read(&dev, 0, 0, 0x7024, &local_swrite, sizeof(uint32_t));
	e_read(&dev, 0, 0, 0x7028, &chip_swrite, sizeof(uint32_t));
	e_read(&dev, 0, 0, 0x702C, &eram_swrite, sizeof(uint32_t));
	if (local_swrite != chip_swrite || local_swrite != eram_swrite)
		fail++;

	printf("Unsigned Reads\n");
	printf("local: 0x%08X\n", local_read);
	printf("chip: 0x%08X\n", chip_read);
	printf("eram: 0x%08X\n", eram_read);
	putchar('\n');

	printf("Unsigned Writes\n");
	printf("local: 0x%08X\n", local_write);
	printf("chip: 0x%08X\n", chip_write);
	printf("eram: 0x%08X\n", eram_write);
	putchar('\n');

	printf("Signed Reads\n");
	printf("local: 0x%08X\n", local_sread);
	printf("chip: 0x%08X\n", chip_sread);
	printf("eram: 0x%08X\n", eram_sread);
	putchar('\n');

	printf("Signed Writes\n");
	printf("local: 0x%08X\n", local_swrite);
	printf("chip: 0x%08X\n", chip_swrite);
	printf("eram: 0x%08X\n", eram_swrite);
	putchar('\n');

	e_read(&dev, 0, 0, 0x7030, &local_read_16, sizeof(uint32_t));
	e_read(&dev, 0, 0, 0x7034, &chip_read_16, sizeof(uint32_t));
	e_read(&dev, 0, 0, 0x7038, &eram_read_16, sizeof(uint32_t));
	if (local_read_16 != chip_read_16 || local_read_16 != eram_read_16)
		fail++;

	e_read(&dev, 0, 0, 0x703c, &local_write_16, sizeof(uint32_t));
	e_read(&dev, 0, 0, 0x7040, &chip_write_16, sizeof(uint32_t));
	e_read(&dev, 0, 0, 0x7044, &eram_write_16, sizeof(uint32_t));
	if (local_write_16 != chip_write_16 || local_write_16 != eram_write_16)
		fail++;

	e_read(&dev, 0, 0, 0x7048, &local_sread_16, sizeof(uint32_t));
	e_read(&dev, 0, 0, 0x704C, &chip_sread_16, sizeof(uint32_t));
	e_read(&dev, 0, 0, 0x7050, &eram_sread_16, sizeof(uint32_t));
	if (local_sread_16 != chip_sread_16 || local_sread_16 != eram_sread_16)
		fail++;

	e_read(&dev, 0, 0, 0x7054, &local_swrite_16, sizeof(uint32_t));
	e_read(&dev, 0, 0, 0x7058, &chip_swrite_16, sizeof(uint32_t));
	e_read(&dev, 0, 0, 0x705C, &eram_swrite_16, sizeof(uint32_t));
	if (local_swrite_16 != chip_swrite_16 || local_swrite_16 != eram_swrite_16)
		fail++;

	printf("16b Unsigned Reads\n");
	printf("local: 0x%08X\n", local_read_16);
	printf("chip: 0x%08X\n", chip_read_16);
	printf("eram: 0x%08X\n", eram_read_16);
	putchar('\n');

	printf("16b Unsigned Writes\n");
	printf("local: 0x%08X\n", local_write_16);
	printf("chip: 0x%08X\n", chip_write_16);
	printf("eram: 0x%08X\n", eram_write_16);
	putchar('\n');

	printf("16b Signed Reads\n");
	printf("local: 0x%08X\n", local_sread_16);
	printf("chip: 0x%08X\n", chip_sread_16);
	printf("eram: 0x%08X\n", eram_sread_16);
	putchar('\n');

	printf("16b Signed Writes\n");
	printf("local: 0x%08X\n", local_swrite_16);
	printf("chip: 0x%08X\n", chip_swrite_16);
	printf("eram: 0x%08X\n", eram_swrite_16);
	putchar('\n');

	e_close(&dev);
	e_finalize();

	if (fail)
	  printf("%d x FAILED\n", fail);
	else
	  printf("PASSED\n");

	return fail;
}

