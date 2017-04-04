/**
 * Copyright 2017 University of Applied Sciences Western Switzerland / Fribourg
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
 * Project:	HEIA-FR / Embedded Systems 2 Laboratory
 *
 * Abstract:	Wheel Device Driver
 *
 * Purpose:	This module implements a method to get state of the wheel
 *		of the HEIA-FR extension board of the Beaglebone black. 
 *
 * Author: 	Daniel Gachet
 * Date: 	21.03.2017
 */

#include <am335x_gpio.h>

#include "wheel.h"

#define BTN_PIN 	 2
#define BTN_GPIO 	 0
#define CHA_PIN	 	 1
#define CHA_GPIO	 2
#define CHB_PIN	 	29
#define CHB_GPIO	 1


static wheel_button_pressed_t button_pressed;
static wheel_event_t wheel_event;

// wheel interrupt handler, this routine is called called when rotating 
// the rotary encoder wheel
static void wheel_ih(enum am335x_gpio_modules module, uint32_t pin, void* param)
{
	(void)module; (void)pin; (void)param;
	enum wheel_events event = WHEEL_RIGHT;
	bool cha = am335x_gpio_get_state (CHA_GPIO, CHA_PIN);
	bool chb = am335x_gpio_get_state (CHB_GPIO, CHB_PIN);
	if (cha == chb) event = WHEEL_LEFT;
	wheel_event(event);
}

// wheel button interrupt handler, this method is called when pressing the 
// push button of the rotary encoder wheel
static void button_ih(enum am335x_gpio_modules module, uint32_t pin, void* param)
{
	(void)module; (void)pin; (void)param;
	button_pressed();
}

// public method implementation -----------------------------------------------

void wheel_init(
	wheel_event_t event_routine,
	wheel_button_pressed_t button_routine)
{
	// initialize gpio 1 module
	am335x_gpio_init(CHA_GPIO);
	am335x_gpio_init(CHB_GPIO);
	am335x_gpio_init(BTN_GPIO);

	// configure chb gpio pin as input 
	am335x_gpio_setup_pin_in
		(CHB_GPIO, CHB_PIN, AM335X_GPIO_PULL_NONE, true);

	// configure cha gpio pin as interrupt port to detect both edges
	wheel_event = event_routine;
	am335x_gpio_attach (CHA_GPIO, CHA_PIN, AM335X_GPIO_IRQ_BOTH, true, 
			    wheel_ih, 0);


	button_pressed = button_routine;
	am335x_gpio_attach (BTN_GPIO, BTN_PIN, AM335X_GPIO_IRQ_FALLING, true, 
			    button_ih, 0);
}

