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

#define SET_CCLK

#ifdef SET_CCLK
void set_cclk_divider(unsigned int divider)
{
	e_syscfg_clk_t clkcfg;
	clkcfg.reg =  ee_read_esys(E_SYS_CFGCLK);
	clkcfg.fields.divider = divider; // Full speed
	ee_write_esys(E_SYS_CFGCLK, clkcfg.reg);
}

void set_tx_divider(e_epiphany_t *dev, unsigned int divider)
{
	e_syscfg_tx_t txcfg;

	txcfg.reg = ee_read_esys(E_SYS_CFGTX);

	// Force route east
	txcfg.fields.ctrlmode = 0x5;
	ee_write_esys(E_SYS_CFGTX, txcfg.reg);

	//Change clock divider to solve FPGA receiver speed path
	e_write(&dev, 2, 3, 0xf0300, &divider, sizeof(unsigned int));

	//Return to normal mode
	txcfg.fields.ctrlmode = 0;
	ee_write_esys(E_SYS_CFGTX, txcfg.reg);

}
#endif

char *strfail = "<<< FAIL!";
char *strpass = "";

#define chk(a, b)  ((a##_##b != local_##b) ? (fail++, strfail) : strpass)

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
	uint32_t flag;
	int fail = 0;
	uint32_t bad = 0xbaadc0de;

	e_init(NULL);
	e_get_platform_info(&platform);
	e_reset_system();
	e_open(&dev, 0, 0, platform.rows, platform.cols);

#ifdef SET_CCLK
	int  clkdiv = 7, txdiv = 1;
	printf("Setting clock divider to %d\n", clkdiv);
	set_cclk_divider(clkdiv);
	printf("Setting TX divider to %d.\n", txdiv);
	set_tx_divider(&dev, txdiv);
#endif

	e_write(&dev, 0, 0, 0x7060, &bad, sizeof(uint32_t));

	printf("Loading code to Epiphany\n");
	e_load_group("./bin/elink-test1-device.elf" , &dev, 0, 0, 1, 1, E_TRUE);

	// Wait until finished (check for timeout?)
	do {
	  e_read(&dev, 0, 0, 0x7060, &flag, sizeof(uint32_t));
	} while(flag == bad);

	if(flag == 0xABABABAB) {
	  printf("Running\n");
	  do {
	    e_read(&dev, 0, 0, 0x7060, &flag, sizeof(uint32_t));
	  } while(flag == 0xABABABAB);
	}

	if(flag != 0xCAFECAFE) {
	  printf("ERROR: Device code did not complete! (0x%08X)\n", flag);
	  fail++;
	} else {
	  printf("Device finished.\n");
	}

	e_read(&dev, 0, 0, 0x7000, &local_read, sizeof(uint32_t));
	e_read(&dev, 0, 0, 0x7004, &chip_read, sizeof(uint32_t));
	e_read(&dev, 0, 0, 0x7008, &eram_read, sizeof(uint32_t));

	e_read(&dev, 0, 0, 0x700c, &local_write, sizeof(uint32_t));
	e_read(&dev, 0, 0, 0x7010, &chip_write, sizeof(uint32_t));
	e_read(&dev, 0, 0, 0x7014, &eram_write, sizeof(uint32_t));

	e_read(&dev, 0, 0, 0x7018, &local_sread, sizeof(uint32_t));
	e_read(&dev, 0, 0, 0x701C, &chip_sread, sizeof(uint32_t));
	e_read(&dev, 0, 0, 0x7020, &eram_sread, sizeof(uint32_t));

	e_read(&dev, 0, 0, 0x7024, &local_swrite, sizeof(uint32_t));
	e_read(&dev, 0, 0, 0x7028, &chip_swrite, sizeof(uint32_t));
	e_read(&dev, 0, 0, 0x702C, &eram_swrite, sizeof(uint32_t));

	printf("Unsigned Reads\n");
	printf("local: 0x%08X\n", local_read);
	printf("chip:  0x%08X %s\n", chip_read, chk(chip, read));
	printf("eram:  0x%08X %s\n", eram_read, chk(eram, read));
	putchar('\n');

	printf("Unsigned Writes\n");
	printf("local: 0x%08X\n", local_write);
	printf("chip:  0x%08X %s\n", chip_write, chk(chip, write));
	printf("eram:  0x%08X %s\n", eram_write, chk(eram, write));
	putchar('\n');

	printf("Signed Reads\n");
	printf("local: 0x%08X\n", local_sread);
	printf("chip:  0x%08X %s\n", chip_sread, chk(chip, sread));
	printf("eram:  0x%08X %s\n", eram_sread, chk(eram, sread));
	putchar('\n');

	printf("Signed Writes\n");
	printf("local: 0x%08X\n", local_swrite);
	printf("chip:  0x%08X %s\n", chip_swrite, chk(chip, swrite));
	printf("eram:  0x%08X %s\n", eram_swrite, chk(eram, swrite));
	putchar('\n');

	e_read(&dev, 0, 0, 0x7030, &local_read_16, sizeof(uint32_t));
	e_read(&dev, 0, 0, 0x7034, &chip_read_16, sizeof(uint32_t));
	e_read(&dev, 0, 0, 0x7038, &eram_read_16, sizeof(uint32_t));

	e_read(&dev, 0, 0, 0x703c, &local_write_16, sizeof(uint32_t));
	e_read(&dev, 0, 0, 0x7040, &chip_write_16, sizeof(uint32_t));
	e_read(&dev, 0, 0, 0x7044, &eram_write_16, sizeof(uint32_t));

	e_read(&dev, 0, 0, 0x7048, &local_sread_16, sizeof(uint32_t));
	e_read(&dev, 0, 0, 0x704C, &chip_sread_16, sizeof(uint32_t));
	e_read(&dev, 0, 0, 0x7050, &eram_sread_16, sizeof(uint32_t));

	e_read(&dev, 0, 0, 0x7054, &local_swrite_16, sizeof(uint32_t));
	e_read(&dev, 0, 0, 0x7058, &chip_swrite_16, sizeof(uint32_t));
	e_read(&dev, 0, 0, 0x705C, &eram_swrite_16, sizeof(uint32_t));

	printf("16b Unsigned Reads\n");
	printf("local: 0x%08X\n", local_read_16);
	printf("chip:  0x%08X %s\n", chip_read_16, chk(chip, read_16));
	printf("eram:  0x%08X %s\n", eram_read_16, chk(eram, read_16));
	putchar('\n');

	printf("16b Unsigned Writes\n");
	printf("local: 0x%08X\n", local_write_16);
	printf("chip:  0x%08X %s\n", chip_write_16, chk(chip, write_16));
	printf("eram:  0x%08X %s\n", eram_write_16, chk(eram, write_16));
	putchar('\n');

	printf("16b Signed Reads\n");
	printf("local: 0x%08X\n", local_sread_16);
	printf("chip:  0x%08X %s\n", chip_sread_16, chk(chip, sread_16));
	printf("eram:  0x%08X %s\n", eram_sread_16, chk(eram, sread_16));
	putchar('\n');

	printf("16b Signed Writes\n");
	printf("local: 0x%08X\n", local_swrite_16);
	printf("chip:  0x%08X %s\n", chip_swrite_16, chk(chip, swrite_16));
	printf("eram:  0x%08X %s\n", eram_swrite_16, chk(eram, swrite_16));
	putchar('\n');

#if WAIT
	printf("Press <return>\n");
	getchar();
#endif

	e_close(&dev);
	e_finalize();

	if (fail)
	  printf("%d x FAILED\n", fail);
	else
	  printf("PASSED\n");

	return fail;
}

