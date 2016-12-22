/**
 * Copyright 2016 University of Applied Sciences Western Switzerland / Fribourg
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Project:	HEIA-FR / Embedded Systems 1 Laboratory
 *
 * Abstract:	7-Segment Display Device Driver
 *
 * Purpose:	This module implements a method to diplay a value in range
 *   		of -99 to 99 on a two 7-segments display of the HEIA-FR 
 * 		extension board of the Beaglebone black.
 *
 * Author: 	Daniel Gachet
 * Date: 	26.10.2016
 *
 * Modified by:	Jonathan Rial, Alan Sueur
 * Date:		december 2016
 */

#include <stdbool.h>
#include <am335x_gpio.h>

#include "seg7.h"

// pin definition for 7-segment access
#define DIG_GPIO	AM335X_GPIO2
#define DIG1		(1<<2)
#define DIG2		(1<<3)
#define DIG_ALL		(DIG1 | DIG2)

#define DP_GPIO		AM335X_GPIO2
#define DP1		(1<<4)
#define DP2		(1<<5)
#define DP_ALL		(DP1 | DP2)

#define SEG_GPIO	AM335X_GPIO0
#define SEG_A		(1<<4)
#define SEG_B		(1<<5)
#define SEG_C		(1<<14)
#define SEG_D		(1<<22)
#define SEG_E		(1<<23)
#define SEG_F		(1<<26)
#define SEG_G		(1<<27)
#define SEG_ALL		(SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G)

// macro to compute number of elements of an array
#define ARRAY_OF(x) (sizeof(x) / sizeof(x[0]))

// structure to initialize gpio pins used by 7-segment
static const struct gpio_init {
	enum am335x_gpio_modules module;
	uint32_t pin_nr;
	bool state;
} gpio_init[] = { { DIG_GPIO, 2, false },	// DIG1
		{ DIG_GPIO, 3, false },	// DIG2

		{ DP_GPIO, 4, false },	// DP1
		{ DP_GPIO, 5, false },	// DP2

		{ SEG_GPIO, 4, false },	// SEGA
		{ SEG_GPIO, 5, false },	// SEGB
		{ SEG_GPIO, 14, false },	// SEGC
		{ SEG_GPIO, 22, false },	// SEGD
		{ SEG_GPIO, 23, false },	// SEGE
		{ SEG_GPIO, 26, false },	// SEGF
		{ SEG_GPIO, 27, false },	// SEGG
		};

/* 7-segment: segment definition

 +-- SEG_A --+
 |           |
 SEG_F       SEG_B
 |           |
 +-- SEG_G --+
 |           |
 SEG_E       SEG_C
 |           |
 +-- SEG_D --+
 */
static const uint32_t seg7[] = {
		(SEG_A + SEG_B + SEG_C + SEG_D + SEG_E + SEG_F), // 0
		( SEG_B + SEG_C), // 1
		(SEG_A + SEG_B + SEG_E + SEG_D + SEG_G), // 2
		(SEG_A + SEG_B + SEG_C + SEG_D + SEG_G), // 3
		( SEG_B + SEG_C + SEG_F + SEG_G), // 4
		(SEG_A + SEG_C + SEG_D + SEG_F + SEG_G), // 5
		(SEG_A + SEG_C + SEG_D + SEG_E + SEG_F + SEG_G), // 6
		(SEG_A + SEG_B + SEG_C), // 7
		(SEG_A + SEG_B + SEG_C + SEG_D + SEG_E + SEG_F + SEG_G), // 8
		(SEG_A + SEG_B + SEG_C + SEG_D + SEG_F + SEG_G), // 9
		(SEG_A + SEG_B + SEG_C + SEG_E + SEG_F + SEG_G), // A
		( SEG_C + SEG_D + SEG_E + SEG_F + SEG_G), // b
		(SEG_A + SEG_D + SEG_E + SEG_F), // C
		( SEG_B + SEG_C + SEG_D + SEG_E + SEG_G), // d
		(SEG_A + SEG_D + SEG_E + SEG_F + SEG_G), // E
		(SEG_A + SEG_E + SEG_F + SEG_G), // F
		(0),

};

// 7-segment display: digit value
static struct digit {
	uint32_t seg7;
	uint32_t dot;
	uint32_t digit;
} display[2] = { [0] = { .digit = DIG2, }, [1] = { .digit = DIG1, }, };

// -----------------------------------------------------------------------------
// implementation of public methods
// -----------------------------------------------------------------------------

void seg7_init() {
	// initialize gpio modules
	am335x_gpio_init(DIG_GPIO);
	am335x_gpio_init(DP_GPIO);
	am335x_gpio_init(SEG_GPIO);

	// configure gpio pins as output
	for (int i = ARRAY_OF(gpio_init) - 1; i >= 0; i--) {
		am335x_gpio_setup_pin_out(gpio_init[i].module, gpio_init[i].pin_nr,
				gpio_init[i].state);
	}
}

// -----------------------------------------------------------------------------

void seg7_display_value(int value) {
	// test for negative values
	uint32_t dot = 0;
	if (value < 0) {
		dot = DP1;
		value = -value;
	}

	// display unit & decade
	display[0].seg7 = seg7[value % 10];
	display[1].seg7 = seg7[(value / 10) % 10];
	display[1].dot = dot;

	seg7_refresh_display();
}

// -----------------------------------------------------------------------------

void seg7_display_time(int milli) {
	if (milli / 1000 < 1 && milli != 0) {
		display[0].seg7 = seg7[milli / 100 % 10];
		display[1].dot = DP2;
		display[1].seg7 = seg7[0];
	} else {
		display[0].seg7 = seg7[milli / 1000 % 10];
		display[1].dot = 0;
		display[1].seg7 = seg7[(milli / 10000) % 10];
	}

	seg7_refresh_display();
}

// -----------------------------------------------------------------------------

void seg7_refresh_display() {
	static unsigned digit = 0;

	// turn-off all segments & digits
	am335x_gpio_change_states(DIG_GPIO, DIG_ALL, false);
	am335x_gpio_change_states(DP_GPIO, DP_ALL, false);
	am335x_gpio_change_states(SEG_GPIO, SEG_ALL, false);

	am335x_gpio_change_states(DP_GPIO, display[digit].dot, true);
	am335x_gpio_change_states(SEG_GPIO, display[digit].seg7, true);
	am335x_gpio_change_states(DIG_GPIO, display[digit].digit, true);

	digit = (digit + 1) % 2;
}

// -----------------------------------------------------------------------------

void seg7_reset() {
	am335x_gpio_change_states(DIG_GPIO, DIG_ALL, false);
	am335x_gpio_change_states(DP_GPIO, DP_ALL, false);
	am335x_gpio_change_states(SEG_GPIO, SEG_ALL, false);
}
