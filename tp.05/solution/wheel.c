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
 * Abstract:	Wheel Device Driver
 *
 * Purpose:	This module implements a method to get state of the wheel
 *		of the HEIA-FR extension board of the Beaglebone black. 
 *
 * Author: 	Daniel Gachet
 * Date: 	21.11.2016
 */

#include <am335x_gpio.h>

#include "wheel.h"

#define SW_PIN 		 2
#define SW_GPIO 	 0
#define CHA_PIN	 	 1
#define CHA_GPIO	 2
#define CHB_PIN	 	29
#define CHB_GPIO	 1

// rotary encoder finite state machine based on former and new state
static const enum wheel_direction transitions[4][4] = {
	{ WHEEL_STILL, WHEEL_STILL,  WHEEL_STILL,  WHEEL_STILL },
	{ WHEEL_LEFT,  WHEEL_STILL,  WHEEL_STILL,  WHEEL_RIGHT },
	{ WHEEL_RIGHT, WHEEL_STILL,  WHEEL_STILL,  WHEEL_LEFT  },
	{ WHEEL_STILL, WHEEL_STILL,  WHEEL_STILL,  WHEEL_STILL },
};

// well button finite state machine based on former and new state
static const bool states[2][2] = {
	{ false, false },
	{ true,  false },
};

// local variables
static int former_state = 0;
static bool btn_former_state = 0;

// private method implementation -----------------------------------------------

static inline int get_encoder_state()
{
	int state = 0;
	if (am335x_gpio_get_state (CHA_GPIO, CHA_PIN)) state += 1;
	if (am335x_gpio_get_state (CHB_GPIO, CHB_PIN)) state += 2;
	return state;
}

// public method implementation -----------------------------------------------

void wheel_init()
{
	// initialize gpio 1 module
	am335x_gpio_init(CHA_GPIO);
	am335x_gpio_init(CHB_GPIO);
	am335x_gpio_init(SW_GPIO);

	// configure gpio pins as output 
	am335x_gpio_setup_pin_in 
		(CHA_GPIO, CHA_PIN, AM335X_GPIO_PULL_NONE, true);
	am335x_gpio_setup_pin_in
		(CHB_GPIO, CHB_PIN, AM335X_GPIO_PULL_NONE, true);
	am335x_gpio_setup_pin_in
		(SW_GPIO,  SW_PIN,  AM335X_GPIO_PULL_NONE, true);

	former_state = get_encoder_state();
}

// ----------------------------------------------------------------------------

enum wheel_direction wheel_get_direction()
{
	int state = get_encoder_state();
	enum wheel_direction dir = transitions[former_state][state];
	former_state = state;
	return dir;
}

// ----------------------------------------------------------------------------

bool wheel_button_is_pressed()
{
	bool state = am335x_gpio_get_state (SW_GPIO, SW_PIN);
	bool is_pressed = states[btn_former_state][state];
	btn_former_state = state;
	return is_pressed;
}

