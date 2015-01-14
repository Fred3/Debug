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
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <string.h>

// test results data structure
typedef struct {
	uint32_t local_read;
	uint32_t chip_read;
	uint32_t eram_read;
	uint32_t local_write;
	uint32_t chip_write;
	uint32_t eram_write;
	uint32_t local_sread;
	uint32_t chip_sread;
	uint32_t eram_sread;
	uint32_t local_swrite;
	uint32_t chip_swrite;
	uint32_t eram_swrite;
	uint32_t local_read_16;
	uint32_t chip_read_16;
	uint32_t eram_read_16;
	uint32_t local_write_16;
	uint32_t chip_write_16;
	uint32_t eram_write_16;
	uint32_t local_sread_16;
	uint32_t chip_sread_16;
	uint32_t eram_sread_16;
	uint32_t local_swrite_16;
	uint32_t chip_swrite_16;
	uint32_t eram_swrite_16;
} test_t;

int main(void)
{
	test_t *monkey = (void *) 0x7000;
	uint32_t foo = 0x84838281;
	volatile void *local = (void *) 0x7020;
	volatile void *chip = (void *) 0x80901000;
	volatile void *eram = (void *) 0x8f100000;

	*((uint32_t *)local) = 0xdeadbeef;
	*((uint32_t *)chip) = 0xdeadbeef;
	*((uint32_t *)eram) = 0xdeadbeef;

	memset((void *) monkey, 0xAB, sizeof(*monkey));

	/* Read test */
	*((uint32_t *)local) = foo;
	*((uint32_t *)chip) = foo;
	*((uint32_t *)eram) = foo;

	monkey->local_read = *((uint8_t*) local);
	monkey->chip_read = *((uint8_t*) chip);
	monkey->eram_read =  *((uint8_t*) eram);

	monkey->local_sread = *((int8_t*) local);
	monkey->chip_sread = *((int8_t*) chip);
	monkey->eram_sread = *((int8_t*) eram);

	/* Write test */

	/* Reset memory locations */
	*((uint32_t *)local) = 0x0;
	*((uint32_t *)chip) = 0x0;
	*((uint32_t *)eram) = 0x0;

	*((uint8_t *)local) = foo;
	*((uint8_t *)chip) = foo;
	*((uint8_t *)eram) = foo;

	monkey->local_write = *((uint32_t*) local);
	monkey->chip_write = *((uint32_t*) chip);
	monkey->eram_write =  *((uint32_t*) eram);

	*((uint32_t *)local) = 0x0;
	*((uint32_t *)chip) = 0x0;
	*((uint32_t *)eram) = 0x0;

	*((int8_t *)local) = foo;  // Don't expect this to be different, but...
	*((int8_t *)chip) = foo;
	*((int8_t *)eram) = foo;

	monkey->local_swrite = *((uint32_t*) local);
	monkey->chip_swrite = *((uint32_t*) chip);
	monkey->eram_swrite =  *((uint32_t*) eram);

	// Now repeat for 16b operations...
#if 1
	/* Read test */
	*((uint32_t *)local) = foo;
	*((uint32_t *)chip) = foo;
	*((uint32_t *)eram) = foo;

	monkey->local_read_16 = *((uint16_t*) local);
	monkey->chip_read_16 = *((uint16_t*) chip);
	monkey->eram_read_16 =  *((uint16_t*) eram);

	monkey->local_sread_16 = *((int16_t*) local);
	monkey->chip_sread_16 = *((int16_t*) chip);
	monkey->eram_sread_16 = *((int16_t*) eram);

	/* Write test */

	/* Reset memory locations */
	*((uint32_t *)local) = 0x0;
	*((uint32_t *)chip) = 0x0;
	*((uint32_t *)eram) = 0x0;

	*((uint16_t *)local) = foo;
	*((uint16_t *)chip) = foo;
	*((uint16_t *)eram) = foo;

	monkey->local_write_16 = *((uint32_t*) local);
	monkey->chip_write_16 = *((uint32_t*) chip);
	monkey->eram_write_16 =  *((uint32_t*) eram);

	*((uint32_t *)local) = 0x0;
	*((uint32_t *)chip) = 0x0;
	*((uint32_t *)eram) = 0x0;

	*((int16_t *)local) = foo;  // Don't expect this to be different, but...
	*((int16_t *)chip) = foo;
	*((int16_t *)eram) = foo;

	monkey->local_swrite_16 = *((uint32_t*) local);
	monkey->chip_swrite_16 = *((uint32_t*) chip);
	monkey->eram_swrite_16 =  *((uint32_t*) eram);
#endif
	return EXIT_SUCCESS;;
}

