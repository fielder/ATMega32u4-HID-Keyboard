#ifndef F_CPU
#error F_CPU not defined
#endif

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdbool.h>
#include <util/delay.h>

#include "matrix.h"
#include "layout.h"
#include "usb.h"
#include "keys.h"

static bool has_unsent_packets = false;

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
const uint8_t mapping[SNES_CNT] = {
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
			has_unsent_packets = true;
		}
		if (cur)
		{
			if (outidx < REPORT_KEY_SLOTS)
				keyboard_pressed_keys[outidx++] = mapping[i];
		}
	}
	while (outidx < REPORT_KEY_SLOTS)
		keyboard_pressed_keys[outidx++] = KEY_NONE;
	sei ();

#if 0
	for (int i = 0; i < NUM_ROWS; i++)
	{
		_delay_ms(10);
		for (int j = 0; j < NUM_COLS; j++)
		{
			_delay_ms(10);
			if (j < 7)
			{
				// This is because of the dumb routing on PORTD
				int DNUM;
				switch (j)
				{
					case (0):
						DNUM = 0;
						break;
					case (1):
						DNUM = 6;
						break;
					case (2):
						DNUM = 4;
						break;
					case (3):
						DNUM = 5;
						break;
					case (4):
						DNUM = 3;
						break;
					case (5):
						DNUM = 2;
						break;
					case (6):
						DNUM = 1;
						break;
				}

				PORTD &= ~(1 << DNUM);  // Set it low to read from the row
			}
			else
			{
				// PORTF is not continuous, which means we cannot just loop over
				// it
				int FNUM = j - 7;
				if (FNUM > 1) FNUM += 2;  // Bits 2 and 3 do not exist on PORTF
				PORTF &= ~(1 << FNUM);
			}
			cli(); // Turn off interrupts for this section
			if ((bool)(PINB & (1 << (i + B_OFFSET))) != state_layer[i][j])
			{
				// Is the value in the state array different from the value read from the key?
				if (!(PINB & (1 << (i + B_OFFSET))))
				{
				       	// Low to High (Key Down)
					uint16_t key = layout[layout_num][i][j];
					if (key & KEY_MOD_MACRO)
					{
						// Is this a macro key, in which case we have to set the appropriate value in the modifier bit
						key &= ~(KEY_MOD_MACRO);
						keyboard_modifier |= key;

					}
					else if (key & KEY_LS_MACRO)
					{
						// Is this a layer shift macro, in which case we have to change the layer number
						key &= ~(KEY_LS_MACRO);
						if ((layout_num | key) <= (NUM_LAYERS - 1))
						{
							// Make sure we're not shifting to a layer that doesn't exist
							layout_num |= key;
							do_layer_led(); // Set the layer LEDs
							for (int k = 0; k < MAX_PRESS_KEYS; k++)
							{
								keyboard_pressed_keys[k] = 0; // We have to clear the report after shifting layers because of conflicts from key-ups and key-downs on different layers
							}
						}
					}
					else
					{
						// This is just a normal keypress, record it in the first available slot in the HID report
						for (int k = 0; k < MAX_PRESS_KEYS; k++)
						{
							if (keyboard_pressed_keys[k] == 0)
							{
								// '0' is the code for no key press
								keyboard_pressed_keys[k] = key;
								break;
							}
						}
					}
				}
				else
				{
					// Handle key up events
					uint16_t key = layout[layout_num][i][j];
					if (key & KEY_MOD_MACRO)
					{
						// Is this a macro key, we have to remove the macro bit from the modifier byte to signify a key up
						key &= ~(KEY_MOD_MACRO);
						keyboard_modifier &= ~(key);

					}
					else if (key & KEY_LS_MACRO)
					{
						// Is this a layer shift key, we have to remove the bit from layout_num to switch back to the previous layer
						key &= ~(KEY_LS_MACRO);
						for (int k = 0; k < MAX_PRESS_KEYS; k++)
						{
							keyboard_pressed_keys[k] = 0; // Clear the HID report
						}
						layout_num &= ~(key);
						do_layer_led();
					}
					else
					{
						for (int k = 0; k < MAX_PRESS_KEYS; k++)
						{
							// Normal key up event, find the key in the HID report and remove it
							if (keyboard_pressed_keys[k] == key)
							{
								keyboard_pressed_keys[k] = 0;
							}
						}
					}
				}
				state_layer[i][j] = (bool)(PINB & (1 << (i + B_OFFSET)));  // Record the changed state in the array to register future state changes
				has_unsent_packets = true; // Let the timer interrupt know that we've got some packets to be sent
			}
			sei(); // Re-enable the interrupts
			PORTF = 0xFF; // Set all the pins high to not generate false keypresses when scanning future columns
			PORTD = 0xFF;

		}
	}
#endif
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
		has_unsent_packets = false;
	}
}
