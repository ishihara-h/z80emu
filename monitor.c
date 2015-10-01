/* monitor.c
 * command example
 *  0[cr] 0.10[cr]
 *  0:0 1 2 3[cr] 
 *  0g
 *
 * Copyright (c) 2015 Hiroshi Ishihara 
 *
 * This program is free, do whatever you want with it.
 */

#include <stdio.h>
#include <stdlib.h> 
#include "z80emu.h"
#include "monitor.h"
#include "avrmem.h"
/* #include "monitor.h" */
#include "uart.h"
#include <util/delay.h>

#define Z80_CPU_SPEED           4000000   /* In Hz. */
#define CYCLES_PER_STEP         (Z80_CPU_SPEED / 50)
#define MAXIMUM_STRING_LENGTH   100

static void dump(uint16_t x, uint16_t y);

static FILE uart_std = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);

int main (void)
{
        uint16_t n = 0;
        uint16_t n1 = 0;
        uint16_t n2 = 0;
        int8_t mode = 0;
        char c = 0;
        char buf[80];
        char *p;
        Z80_STATE state;

	uart_init();
	stdout = stdin = &uart_std;

	while(1) {
		printf(">");
		fgets(buf, sizeof(buf), stdin);
		p = buf;
		while (1) {
			c = *p++;
			if ((c == '\0') || (c == '\n')) {
				if (mode == 2) {
					memwr(n1++, n & 0xff);
				} else {
					dump(n1, n2);
					n1 = n2 + 1;
					n2 = n1 | 0x07; }
				n = 0;
				mode = 0;
				break;
			} else if (c == '.') {
				mode = 1;
				n = 0;
			} else if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f')) {
				if (c >= 'a')
					n = (n << 4) + c - 'a' + 10;
				else if (c >= 'A')
				    	n = (n << 4) + c - 'A' + 10;
				else
					n = (n << 4) + c - '0';
				if (mode == 0)
					n1 = n2 = n;
				else if (mode == 1)
					n2 = n;
                                else if (mode != 2)
					printf("mode err %d\n", mode);
			} else if (c == ':') {
				n1 = n;
                                mode = 2;
			} else if (c == 'g') {
				Z80Reset(&state);
				state.pc = n;
				for ( ; ; ) {
					Z80Emulate(&state, CYCLES_PER_STEP);
					if (state.status & FLAG_STOP_EMULATION)
						break;
				}
				break;
                        } else if (c == ' ') {
				if (mode == 2) {
					memwr(n1++, n & 0xff);
					n = 0;
				}	
			} else
				printf("err\n");
		}
	}
	return EXIT_SUCCESS;
}

static void dump(uint16_t x, uint16_t y)
{
	uint8_t first_f;

	first_f = 1;
	printf("%04x", x);
        do {
            if ((first_f == 0) && ((x & 7) == 0))
                printf("\n%04x", x);
            printf(" %02x", memrd(x));
	    first_f = 0;
            x++;
        } while (x <= y);
	printf("\n");
}

/* Emulate CP/M bdos call 5 functions 2 (output character on screen) and 9
 * (output $-terminated string to screen).
 */

void SystemCall (Z80_STATE *state)
{
        if (state->registers.byte[Z80_C] == 2)

                printf("%c", state->registers.byte[Z80_E]);

        else if (state->registers.byte[Z80_C] == 9) {

                int16_t     i, c;

                for (i = state->registers.word[Z80_DE], c = 0; 
                        memrd(i) != '$';
                        i++) {

                        printf("%c", memrd(i & 0xffff));
                        if (c++ > MAXIMUM_STRING_LENGTH) {

                                fprintf(stderr,
                                        "String to print is too long!\n");
                                exit(EXIT_FAILURE);

                        }

                }

        }
}
