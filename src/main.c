#ifndef F_CPU
#error F_CPU not defined
#endif

#include <stdint.h>

#include <avr/io.h>
#include <util/delay.h>
#include <avr/cpufunc.h>

#include "matrix.h"
#include "usb.h"


int
main (int argc, const char **argv)
{
	/* outputs */
	DDRB = _BV(PB6); /* clock */
	DDRF = _BV(PF6); /* latch */
	DDRC = _BV(PC7); /* LED */

	/* enable pull-ups */
	PORTB |=	_BV(PB5) |
			_BV(PB4) |
			_BV(PB7);
	PORTC |=	_BV(PC6);
	PORTD |=	_BV(PD7) |
			_BV(PD4) |
			_BV(PD0) |
			_BV(PD1) |
			_BV(PD3) |
			_BV(PD2) |
			_BV(PD6);
	PORTE |=	_BV(PE6);
	PORTF |=	_BV(PF7); /* serial data in */

	usb_init();

	/* blink LED until USB is set up */
	uint8_t onoff = 1;
	while (!usb_config_status)
	{
		onoff = !onoff;
		if (onoff)
			PORTC |= _BV(PC7);
		else
			PORTC &= ~_BV(PC7);
		_delay_ms (50);
	}
	PORTC &= ~_BV(PC7); /* LED off after USB init */

	PORTB |= _BV(PB6); /* snes clock pulses are from positive to ground */

	matrix_init();

	_delay_ms (50);

	while (1)
	{
		do_matrix_scan();

		/* try to hit 60 Hz */
		__builtin_avr_delay_cycles((uint32_t)1024 * (uint32_t)255);

		/* latch */
		PORTF |= _BV(PF6);
		__builtin_avr_delay_cycles(190);
		PORTF &= ~_BV(PF6);

		/* 6 us after lowering latch,
		 * send 16 down pulses on clock signal. */
		__builtin_avr_delay_cycles(95);

		for (uint8_t i = 0; i < 16; i++)
		{
			PORTB &= ~_BV(PB6);
			/* sample input on falling edge */
			if (i < SNES_CNT)
				snes_pressed[i] = !(PINF & _BV(PINF7));
			__builtin_avr_delay_cycles(95);

			/* controller shifts data out on rising edge */
			PORTB |= _BV(PB6);
			__builtin_avr_delay_cycles(95);
		}
	}

	return 0;
}
