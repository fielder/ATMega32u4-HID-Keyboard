#ifndef F_CPU
#error F_CPU not defined
#endif

#include <stdint.h>

#include <avr/io.h>
#include <util/delay.h>

#include "matrix.h"
#include "usb.h"


int
main (int argc, const char **argv)
{
	usb_init();

	// outputs
	DDRC = _BV(PC7);

	// disable pull-ups
	PORTC = 0x0;

	// enable pull-ups
	PORTD |= _BV(PD2);

	/* blink LED until USB is set up */
	_delay_ms (50);
	uint8_t onoff = 1;
	while (!usb_config_status)
	{
		onoff = !onoff;
		if (onoff)	PORTC |= _BV(PC7);
		else		PORTC &= ~_BV(PC7);
		_delay_ms (50);
	}
	PORTC &= ~_BV(PC7); /* turn LED off */

	matrix_init();
	while (1)
	{
		do_matrix_scan();

		// uint8_t pressed = !(PIND & _BV(PIND2));
		// if (pressed) PORTC |= _BV(PC7);
		// else PORTC &= ~_BV(PC7);
	}

	return 0;
}
