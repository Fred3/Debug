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
} test_t;

int main(void)
{
	test_t *monkey = (void *) 0x7000;
	uint32_t foo = 0x04030201;
	volatile void *local = (void *) 0x7020;
	volatile void *chip = (void *) 0x80901000;
	volatile void *eram = (void *) 0x8f100000;

	*((uint32_t *)local) = 0xdeadbeef;
	*((uint32_t *)chip) = 0xdeadbeef;
	*((uint32_t *)eram) = 0xdeadbeef;

	memset((void *) monkey, 0, sizeof(*monkey));

	/* Read test */
	*((uint32_t *)local) = foo;
	*((uint32_t *)chip) = foo;
	*((uint32_t *)eram) = foo;

	monkey->local_read = *((uint8_t*) local);
	monkey->chip_read = *((uint8_t*) chip);
	monkey->eram_read =  *((uint8_t*) eram);


	/* Write test */

	/* Reset memory localtions */
	*((uint32_t *)local) = 0x0;
	*((uint32_t *)chip) = 0x0;
	*((uint32_t *)eram) = 0x0;

	*((uint8_t *)local) = foo;
	*((uint8_t *)chip) = foo;
	*((uint8_t *)eram) = foo;

	monkey->local_write = *((uint8_t*) local);
	monkey->chip_write = *((uint8_t*) chip);
	monkey->eram_write =  *((uint8_t*) eram);

	return EXIT_SUCCESS;;
}

