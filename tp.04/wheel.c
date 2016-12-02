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
 * Project:  HEIA-FR / Embedded Systems 1 Laboratory
 *
 * Abstract: Introduction the C programming language
 *
 * Purpose:  Module for managing the wheel of the HEIA-FR extension card of the
 *           Beaglebone Black board
 *
 * Author: 	Jonathan Rial & Alan Sueur
 * Date: 	November 2016
 */

#include <stdbool.h>
#include <am335x_gpio.h>

#include "wheel.h"

// Numéro des pins GPIO
#define SW_PIN    2
#define SW_GPIO   0
#define CHA_PIN   1
#define CHA_GPIO  2
#define CHB_PIN  29
#define CHB_GPIO  1

// Constantes
static enum wheel_state transcient_state = WHEEL_NONE;
static bool cha_previous_state = false;
static bool chb_previous_state = false;

/**
 * Méthode d'initialisation de la roue
 */
void wheel_init() {
	// Initialisation des modules GPIO
	am335x_gpio_init(AM335X_GPIO0);
	am335x_gpio_init(AM335X_GPIO1);
	am335x_gpio_init(AM335X_GPIO2);

	// Initialisation des pins GPIO
	am335x_gpio_setup_pin(SW_GPIO, SW_PIN, AM335X_GPIO_PIN_IN,
			AM335X_GPIO_PULL_NONE);
	am335x_gpio_setup_pin(CHA_GPIO, CHA_PIN, AM335X_GPIO_PIN_IN,
			AM335X_GPIO_PULL_NONE);
	am335x_gpio_setup_pin(CHB_GPIO, CHB_PIN, AM335X_GPIO_PIN_IN,
			AM335X_GPIO_PULL_NONE);

	// Sauvegarde de l'état des pins CHA et CHB
	cha_previous_state = am335x_gpio_get_state(CHA_GPIO, CHA_PIN);
	chb_previous_state = am335x_gpio_get_state(CHB_GPIO, CHB_PIN);
}

/**
 * Méthode permettant de retouner l'état de la roue
 *
 * @return L'état de la roue
 */
enum wheel_state wheel_get_state() {
	enum wheel_state state = WHEEL_NONE;

	bool cha = am335x_gpio_get_state(CHA_GPIO, CHA_PIN);
	if (cha != cha_previous_state) {
		if (transcient_state == WHEEL_NONE) {
			transcient_state = WHEEL_INCR;
		}
	}

	bool chb = am335x_gpio_get_state(CHB_GPIO, CHB_PIN);
	if (chb != chb_previous_state) {
		if (transcient_state == WHEEL_NONE) {
			transcient_state = WHEEL_DECR;
		}
	}

	if ((transcient_state != WHEEL_NONE) && (cha == chb)) {
		state = transcient_state;
		transcient_state = WHEEL_NONE;
		cha_previous_state = cha;
		chb_previous_state = chb;
	}

	bool sw = am335x_gpio_get_state(SW_GPIO, SW_PIN);
	if (!sw) {
		state = WHEEL_RESET;
	}

	return state;
}
