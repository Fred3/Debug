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
        uint32_t flag;
} test_t;

int main(void)
{
	register volatile test_t *monkey asm("r20") = (void *) 0x7000;
	register volatile uint32_t foo asm("r21") = 0x84838281;
	register volatile void *local asm("r22") = (void *) 0x6ff0;
	register volatile void *chip  asm("r23") = (void *) 0x80901000;
	register volatile void *eram  asm("r24") = (void *) 0x8f100000;
	int  n;

	*((volatile uint32_t *)local) = 0xdeadbeef;
	*((volatile uint32_t *)chip) = 0xdeadbeef;
	*((volatile uint32_t *)eram) = 0xdeadbeef;
	while (*((volatile uint32_t *)eram) == 0) ;

	memset((void *) monkey, 0xAB, sizeof(*monkey));

	/* Read test */
	*((volatile uint32_t *)local) = foo;
	*((volatile uint32_t *)chip) = foo;
	*((volatile uint32_t *)eram) = foo;
	while (*((volatile uint32_t *)eram) == 0) ;

	monkey->local_read = *((volatile uint8_t*) local);
	monkey->chip_read  = *((volatile uint8_t*) chip);
	monkey->eram_read  = *((volatile uint8_t*) eram);

	monkey->local_sread = *((volatile int8_t*) local);
	monkey->chip_sread  = *((volatile int8_t*) chip);
	monkey->eram_sread  = *((volatile int8_t*) eram);

	/* Write test */

	/* Reset memory locations */
	*((volatile uint32_t *)local) = 0x0;
	*((volatile uint32_t *)chip) = 0x0;
	*((volatile uint32_t *)eram) = 0x0;
	while (*((volatile uint32_t *)eram) != 0) ;

	*((volatile uint8_t *)local) = foo;
	*((volatile uint8_t *)chip) = foo;
	*((volatile uint8_t *)eram) = foo;
	while (*((volatile uint32_t *)eram) == 0) ;

	monkey->local_write = *((volatile uint32_t*) local);
	monkey->chip_write =  *((volatile uint32_t*) chip);
	monkey->eram_write =  *((volatile uint32_t*) eram);

	*((volatile uint32_t *)local) = 0x0;
	*((volatile uint32_t *)chip) = 0x0;
	*((volatile uint32_t *)eram) = 0x0;
	while (*((volatile uint32_t *)eram) != 0) ;

	*((volatile int8_t *)local) = foo;  // Don't expect this to be different, but...
	*((volatile int8_t *)chip) = foo;
	*((volatile int8_t *)eram) = foo;
	while (*((volatile uint32_t *)eram) == 0) ;

	monkey->local_swrite = *((volatile uint32_t*) local);
	monkey->chip_swrite  = *((volatile uint32_t*) chip);
	monkey->eram_swrite  = *((volatile uint32_t*) eram);

	// Now repeat for 16b operations...
#if 1
	/* Read test */
	*((volatile uint32_t *)local) = foo;
	*((volatile uint32_t *)chip) = foo;
	*((volatile uint32_t *)eram) = foo;
	while (*((volatile uint32_t *)eram) == 0) ;

	monkey->local_read_16 = *((volatile uint16_t*) local);
	monkey->chip_read_16  = *((volatile uint16_t*) chip);
	monkey->eram_read_16  = *((volatile uint16_t*) eram);

	monkey->local_sread_16 = *((volatile int16_t*) local);
	monkey->chip_sread_16  = *((volatile int16_t*) chip);
	monkey->eram_sread_16  = *((volatile int16_t*) eram);

	/* Write test */

	/* Reset memory locations */
	*((volatile uint32_t *)local) = 0x0;
	*((volatile uint32_t *)chip)  = 0x0;
	*((volatile uint32_t *)eram)  = 0x0;
	while (*((volatile uint32_t *)eram) != 0) ;

	*((volatile uint16_t *)local) = foo;
	*((volatile uint16_t *)chip) = foo;
	*((volatile uint16_t *)eram) = foo;
	while (*((volatile uint16_t *)eram) == 0) ;

	monkey->local_write_16 = *((volatile uint32_t*) local);
	monkey->chip_write_16  = *((volatile uint32_t*) chip);
	monkey->eram_write_16  = *((volatile uint32_t*) eram);

	*((volatile uint32_t *)local) = 0x0;
	*((volatile uint32_t *)chip) = 0x0;
	*((volatile uint32_t *)eram) = 0x0;
	while (*((volatile uint32_t *)eram) != 0) ;


	*((volatile int16_t *)local) = foo;  // Don't expect this to be different, but...
	*((volatile int16_t *)chip) = foo;
	*((volatile int16_t *)eram) = foo;
	while (*((volatile int16_t *)eram) == 0) ;

	monkey->local_swrite_16 = *((volatile uint32_t*) local);
	monkey->chip_swrite_16  = *((volatile uint32_t*) chip);
	monkey->eram_swrite_16  = *((volatile uint32_t*) eram);
#endif

	monkey->flag = 0xCAFECAFE;
	return EXIT_SUCCESS;;
}

