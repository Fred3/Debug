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

	printf("Reads\n");
	printf("local: 0x%08x\n", local_read);
	printf("chip: 0x%08x\n", chip_read);
	printf("eram: 0x%08x\n", eram_read);
	putchar('\n');


	printf("Writes\n");
	printf("local: 0x%08x\n", local_write);
	printf("chip: 0x%08x\n", chip_write);
	printf("eram: 0x%08x\n", eram_write);
	putchar('\n');

	e_close(&dev);
	e_finalize();

	if (fail)
		printf("FAILED\n");
	else
		printf("PASSED\n");

	return fail;
}

