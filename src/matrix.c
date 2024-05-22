#ifndef F_CPU
#error F_CPU not defined
#endif

#include <avr/interrupt.h>
#include <avr/io.h>

#include "matrix.h"
#include "usb.h"
#include "keys.h"

static uint8_t has_unsent_packets = 0;

uint8_t snes_pressed[SNES_CNT];
static uint8_t old_snes_pressed[SNES_CNT];

/*
 * SNES		PC
 * -------------------
 * B		C
 * Y		X
 * Select	enter
 * Start	space
 * Up		up
 * Down		down
 * Left		left
 * Right	right
 * A		V
 * X		D
 * L		A
 * R		S
 */
const uint8_t snes_to_key[SNES_CNT] = {
	KEY_C,
	KEY_X,
	KEY_ENTER,
	KEY_SPACE,
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_V,
	KEY_D,
	KEY_A,
	KEY_S,
};


void
matrix_init ()
{
	TCCR0B |= (1 << CS00) | (1 << CS02); // Set up the 1024 prescaler on timer 0
	TCCR0A = (1 << WGM01); // Set the timer to compare mode
	OCR0A = 255; // Set the compare value to max for a lower frequency
	TIMSK0 = (1 << OCIE0A); // Enable timer compare interrupts

	for (uint8_t i = 0; i < SNES_CNT; i++)
	{
		snes_pressed[i] = 0;
		old_snes_pressed[i] = 0;
	}
}


void
do_matrix_scan ()
{
	uint8_t outidx = 0;

	cli ();
	for (uint8_t i = 0; i < SNES_CNT; i++)
	{
		uint8_t cur = snes_pressed[i];
		uint8_t old = old_snes_pressed[i];
		old_snes_pressed[i] = cur;
		if (cur != old)
		{
			/* Let the timer interrupt know that we've got some packets to be sent */
			has_unsent_packets = 1;
		}
		if (cur)
		{
			if (outidx < REPORT_KEY_SLOTS)
				usb_keyboard_pressed_keys[outidx++] = snes_to_key[i];
		}
	}
	while (outidx < REPORT_KEY_SLOTS)
		usb_keyboard_pressed_keys[outidx++] = KEY_NONE;
	sei ();
}


ISR(TIMER0_COMPA_vect)
{
	/* The USB packets are sent on a timer interrupt because OSX was
	 * showing strange USB keypress rejection errors due to high USB
	 * packet frequency
	 */
	if (has_unsent_packets)
	{
		usb_send ();
		has_unsent_packets = 0;
	}
}
