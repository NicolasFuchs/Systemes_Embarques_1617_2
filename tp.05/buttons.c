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
 * Abstract:	Introduction to device driver development in C
 *
 * Purpose:	 Module for managing the buttons of the HEIA-FR extension card of the
 *           Beaglebone Black board
 *
 * Author: 	Alan Sueur, Jonathan Rial
 * Date: 	17.12.2016
 */

#include <stdint.h>
#include <stdbool.h>
#include <am335x_gpio.h>

#include "buttons.h"

#define BUTTON_GPIO	AM335X_GPIO1
#define PIN_BUTTON1	15
#define PIN_BUTTON2	16
#define PIN_BUTTON3	17

// Macro pour calculer la taille d'un tableau
#define ARRAY_OF(x) (sizeof(x) / sizeof(x[0]))

// local variables
// Structure servant à l'initialisation des GPIO
struct gpio_init {
	enum am335x_gpio_modules module;
	uint32_t pin;
	bool state;
};

// Données pour l'initialisation des GPIO
static const struct gpio_init gpio_init[] = {
	{ BUTTON_GPIO, PIN_BUTTON1, AM335X_GPIO_PIN_IN },
	{ BUTTON_GPIO, PIN_BUTTON2, AM335X_GPIO_PIN_IN },
	{ BUTTON_GPIO, PIN_BUTTON3, AM335X_GPIO_PIN_IN },
};

// Lookup table pour le mapping des buttons
static const uint32_t buttons[] = {
	PIN_BUTTON1, PIN_BUTTON2, PIN_BUTTON3,
};


// public method implementation -----------------------------------------------

void buttons_init()
{
	// Initialisation du module GPIO
	am335x_gpio_init(BUTTON_GPIO);

	for (uint32_t i = 0; i < ARRAY_OF(gpio_init); ++i) {
		am335x_gpio_setup_pin(gpio_init[i].module, gpio_init[i].pin,
				gpio_init[i].state, AM335X_GPIO_PULL_NONE);
	}
}


bool buttons_get_state(enum buttons_set button)
{
	return !am335x_gpio_get_state(BUTTON_GPIO, buttons[button]);
}
