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
 * Abstract:	HEIA-FR Extension Board LED Device Driver
 *
 * Purpose:	This module implements a method to drive the state of one of
 *		the 3 LED of the HEIA-FR Beaglebone black extension board.
 *
 * Author: 	Daniel Gachet
 * Date: 	21.11.2016
 */

#include <stdbool.h>
#include <am335x_gpio.h>

#include "leds.h"

// pin definition for 7-segment access
#define LEDS_GPIO	AM335X_GPIO1
#define LED1		(12)
#define LED2		(13)
#define LED3		(14)

// macro to compute number of elements of an array
#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

// structure to initialize gpio pins used by 7-segment
static const struct gpio_init {
	enum am335x_gpio_modules module;
	uint32_t pin_nr;
	bool state;
} gpio_init[] = {
	{LEDS_GPIO, LED1, false}, 
	{LEDS_GPIO, LED2, false}, 
	{LEDS_GPIO, LED3, false},
};

static const uint32_t leds[] = {
	LED1,
	LED2,
	LED3,
};

// -----------------------------------------------------------------------------
// implementation of public methods
// -----------------------------------------------------------------------------

void leds_init()
{
	// initialize gpio modules
	am335x_gpio_init(LEDS_GPIO);

	// configure gpio pins as output
	for (int i=ARRAY_SIZE(gpio_init)-1; i>=0; i--) {
		am335x_gpio_setup_pin_out(
			gpio_init[i].module, 
			gpio_init[i].pin_nr,
			gpio_init[i].state);
	}
}

// -----------------------------------------------------------------------------

void leds_set_state (unsigned led, bool state)
{
	led--;
	if (led < ARRAY_SIZE(leds)) {
		am335x_gpio_change_state(LEDS_GPIO, leds[led], state);
	}
}

