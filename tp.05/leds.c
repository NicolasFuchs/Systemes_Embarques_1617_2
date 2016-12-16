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
 * Purpose:	 Module for managing the leds of the HEIA-FR extension card of the
 *           Beaglebone Black board
 *
 * Author: 	Jonathan Rial, Alan Sueur
 * Date: 	december 2016
 */

#include <stdint.h>
#include <stdbool.h>
#include <am335x_gpio.h>

#include "leds.h"

// Module GPIO pour les leds
#define BUTTON_GPIO	AM335X_GPIO1

// Numéro des pins pour les leds
#define PIN_LED1	12
#define PIN_LED2	13
#define PIN_LED3	14

// Macro pour calculer la taille d'un tableau
#define ARRAY_OF(x) (sizeof(x) / sizeof(x[0]))

// Structure servant à l'initialisation des GPIO
struct gpio_init {
	enum am335x_gpio_modules module;
	uint32_t pin;
	bool state;
};

// Données pour l'initialisation des GPIO
static const struct gpio_init gpio_init[] = {
		{ BUTTON_GPIO, PIN_LED1, AM335X_GPIO_PIN_OUT },
		{ BUTTON_GPIO, PIN_LED2, AM335X_GPIO_PIN_OUT },
		{ BUTTON_GPIO, PIN_LED3, AM335X_GPIO_PIN_OUT },
};

// Lookup table pour le mapping des buttons
static const uint32_t leds[] = {
		PIN_LED1, PIN_LED2, PIN_LED3,
};

void leds_init() {
	// Initialisation du module GPIO
	am335x_gpio_init(BUTTON_GPIO);

	for (uint32_t i = 0; i < ARRAY_OF(gpio_init); ++i) {
		am335x_gpio_setup_pin(gpio_init[i].module, gpio_init[i].pin,
				gpio_init[i].state, AM335X_GPIO_PULL_NONE);
	}
}

void enable_led_exclusively(enum leds_set led) {
	if (led > 2) {
		return;
	}
	leds_reset();
	am335x_gpio_change_state(BUTTON_GPIO, leds[led], true);
}

void change_led_state(enum leds_set led, bool state) {
	if (led > 2) {
		return;
	}
	am335x_gpio_change_state(BUTTON_GPIO, leds[led], state);
}

void leds_reset() {
	for (uint32_t i = 0; i < ARRAY_OF(gpio_init); ++i) {
		am335x_gpio_change_state(gpio_init[i].module, gpio_init[i].pin, false);
	}
}
