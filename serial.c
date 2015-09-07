/* serial.c
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
#include <stdint.h> 
#include <stdlib.h> 
/* #include "monitor.h" */

#ifndef F_CPU
#  define F_CPU  16000000L
#endif

void com_init()
{
  uint16_t  rate;

  rate = (F_CPU/19200/16) - 1;
  UBRR0H = rate >> 8;
  UBRR0L = rate;

  /* async, non-parity, 1-bit stop, 8-bit data */
  UCSR0C = _BV(UCSZ01)|_BV(UCSZ00);

  /* receiver enable, transmitter enable */
  UCSR0B = _BV(RXEN0)|_BV(TXEN0);
}

uint8_t com_getch()
{
  /* wait for a byte received */
  while ((UCSR0A & _BV(RXC0)) == 0)
    ;
  return UDR0;
}

void com_putch(uint8_t c)
{
  /* wait for buffer empty */
  while ((UCSR0A & _BV(UDRE0)) == 0)
    ;
  UDR0 = c;
}

