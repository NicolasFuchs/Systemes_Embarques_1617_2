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
 * Abstract:	HEIA-FR Extension Board Buttons Device Driver
 *
 * Purpose:	This module implements a method to get the state of one of the
 *		3 buttons of the HEIA-FR Beaglebone black extension board.
 *
 * Author: 	Daniel Gachet
 * Date: 	21.11.2016
 */

#include <stdbool.h>
#include <am335x_gpio.h>

#include "buttons.h"

// pin definition for 7-segment access
#define BUTTONS_GPIO	AM335X_GPIO1
#define BUTTON1		(15)
#define BUTTON2		(16)
#define BUTTON3		(17)

// macro to compute number of elements of an array
#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

// structure to initialize gpio pins used by 7-segment
static const struct gpio_init {
	enum am335x_gpio_modules module;
	uint32_t pin_nr;
} gpio_init[] = {
	{BUTTONS_GPIO, BUTTON1},
	{BUTTONS_GPIO, BUTTON2},
	{BUTTONS_GPIO, BUTTON3},
};

// well button finite state machine based on former and new state
static const bool states[2][2] = {
	{ false, false },
	{ true,  false },
};

// local variables
static bool former_state[3] = {[0]=false, };

static const uint32_t buttons[] = {
	BUTTON1,
	BUTTON2,
	BUTTON3,
};

// -----------------------------------------------------------------------------
// implementation of public methods
// -----------------------------------------------------------------------------

void buttons_init()
{
	// initialize gpio modules
	am335x_gpio_init(BUTTONS_GPIO);

	// configure gpio pins as output
	for (int i=ARRAY_SIZE(gpio_init)-1; i>=0; i--) {
		am335x_gpio_setup_pin_in(
			gpio_init[i].module, 
			gpio_init[i].pin_nr,
			AM335X_GPIO_PULL_NONE,
			false);
	}
}

// -----------------------------------------------------------------------------

bool buttons_is_pressed (unsigned button)
{
	button--;
	if (button < ARRAY_SIZE(buttons)) {
		bool state = am335x_gpio_get_state (BUTTONS_GPIO, buttons[button]);
		bool is_pressed = states[former_state[button]][state];
		former_state[button] = state;
		return is_pressed;
	}
	return false;
}

