/*
	PA0-PA7 --> 74VHC574(D0-D7) A0-A6
	PC0-PC7 --> D0-D7
	PB0 -> ~WE
	PB1 -> ~OE
	PB2 -> ~CS1
	PB3 -> ~CS2
	PB4 -> LATCH (low to high edge)

	PD0 -> RXD0
	PD1 -> TXD0
*/

#include <avr/io.h>
#include <stdlib.h>

void	meminit()
{
	DDRA = 0xff;
	DDRB = 0x1f;
	DDRC = 0x00;

	PORTB = 0x0f; 
}

int8_t	memrd(int16_t x)
{
	int8_t result;

	PORTA = x >> 8;
	PORTB |= 0x10; /* latch */
	PORTB &= ~0x10;
	PORTA = x & 0x7f;
	if (x & 0x80)
		PORTB &= 0xF5; /* CS2 = L, OE = L */
	else
		PORTB &= 0xF9; /* CS1 = L, OE = L */

	result = PORTC;
	PORTB |= 0x0f;
	return result;
}

void	memwr(int16_t x, int8_t d)
{
	DDRC = 0xff;
	PORTA = x >> 8;
	PORTB |= 0x10;
	PORTB &= ~0x10;
	PORTA = x & 0x7f;
	if (x & 0x80)
		PORTB &= 0xF7; /* CS2 = L, OE = H */
	else
		PORTB &= 0xFB; /* CS1 = L, OE = H */

	PORTC = d;
	PORTB &= 0xfe; /* WE = L */ 
	PORTB |= 0x0f;
	DDRC = 0x00;
}
