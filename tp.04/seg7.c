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
 * Purpose:  Module for managing the 7 segments displays of the HEIA-FR extension
 *           card of the Beaglebone Black board
 *
 * Author: 	Jonathan Rial & Alan Sueur
 * Date: 	November 2016
 */

#include <stdint.h>
#include <stdbool.h>
#include <am335x_gpio.h>

// Les modules GPIO
#define DIG_GPIO	AM335X_GPIO2 // Pour les DIG
#define DP_GPIO		AM335X_GPIO2 // Pour les points
#define SEG_GPIO	AM335X_GPIO0 // Pour les segments

// Numéro des pins pour les DIG
#define PIN_DIG1	2
#define PIN_DIG2	3

// Bitset pour les DIG
#define DIG1	(1<<PIN_DIG1)
#define DIG2	(1<<PIN_DIG2)

// Numéro des pins pour les points
#define PIN_DP1	4
#define PIN_DP2	5

// Bitset pour les points
#define DP1	(1<<PIN_DP1)
#define DP2	(1<<PIN_DP2)

// Numéro des pins pour l'affichage 7 segments
#define PIN_SEGA	4
#define PIN_SEGB	5
#define PIN_SEGC	14
#define PIN_SEGD	22
#define PIN_SEGE	23
#define PIN_SEGF	26
#define PIN_SEGG	27

// Bitset pour l'affichage 7 segments
#define SEGA	(1<<PIN_SEGA)
#define SEGB	(1<<PIN_SEGB)
#define SEGC	(1<<PIN_SEGC)
#define SEGD	(1<<PIN_SEGD)
#define SEGE	(1<<PIN_SEGE)
#define SEGF	(1<<PIN_SEGF)
#define SEGG	(1<<PIN_SEGG)

// Macro pour calculer la taille d'un tableau
#define ARRAY_OF(x) (sizeof(x) / sizeof(x[0]))

// Structure servant à l'initialisation des GPIO
struct gpio_init {
	enum am335x_gpio_modules module;
	uint32_t pin;
	bool state;
};

// Données pour l'inialisation des GPIO
static const struct gpio_init gpio_init[] = {
	{DIG_GPIO, PIN_DIG1, false},
	{DIG_GPIO, PIN_DIG2, false},

	{DP_GPIO, PIN_DP1, false},
	{DP_GPIO, PIN_DP2, false},

	{SEG_GPIO, PIN_SEGA, false},
	{SEG_GPIO, PIN_SEGB, false},
	{SEG_GPIO, PIN_SEGC, false},
	{SEG_GPIO, PIN_SEGD, false},
	{SEG_GPIO, PIN_SEGE, false},
	{SEG_GPIO, PIN_SEGF, false},
	{SEG_GPIO, PIN_SEGG, false},
};

// Données pour l'affichage des chiffres
static const uint32_t seg7[] = {
	(SEGA + SEGB + SEGC + SEGD + SEGE + SEGF), // 0
	(SEGB + SEGC), // 1
	(SEGA + SEGB + SEGG + SEGE + SEGD), // 2
	(SEGA + SEGB + SEGG + SEGC + SEGD), // 3
	(SEGF + SEGG + SEGB + SEGC), // 4
	(SEGA + SEGF + SEGG + SEGC + SEGD),// 5
	(SEGA + SEGF + SEGE + SEGD + SEGC + SEGG), // 6
	(SEGA + SEGB + SEGC), // 7
	(SEGA + SEGB + SEGC + SEGD + SEGE + SEGF + SEGG), // 8
	(SEGA + SEGF + SEGG + SEGB + SEGC + SEGD), // 9
};

void seg7_init() {
	// Initialisation des modules GPIO
	am335x_gpio_init(AM335X_GPIO0);
	am335x_gpio_init(AM335X_GPIO2);

	// Initialisation des GPIO, DP et DIG
	for(uint32_t i = 0; i < ARRAY_OF(gpio_init); ++i){
		am335x_gpio_setup_pin_out(gpio_init[i].module, gpio_init[i].pin, gpio_init[i].state);
	}
}

void seg7_display_value(int32_t counter) {
	am335x_gpio_setup_pin_out(DIG_GPIO, PIN_DIG1, true);
	if (counter < 0) {
		am335x_gpio_setup_pin_out(DP_GPIO, PIN_DP1, true);
		am335x_gpio_setup_pin_out(DP_GPIO, PIN_DP1, false);
		counter = counter * -1;
	}
	int32_t leftDigit = counter / 10;
	int32_t rightDigit = counter % 10;
	am335x_gpio_change_states(SEG_GPIO, seg7[leftDigit], true);
	am335x_gpio_change_states(SEG_GPIO, seg7[leftDigit], false);
	am335x_gpio_setup_pin_out(DIG_GPIO, PIN_DIG1, false);
	//====================================================
	am335x_gpio_setup_pin_out(DIG_GPIO, PIN_DIG2, true);
	am335x_gpio_change_states(SEG_GPIO, seg7[rightDigit], true);
	am335x_gpio_change_states(SEG_GPIO, seg7[rightDigit], false);
	am335x_gpio_setup_pin_out(DIG_GPIO, PIN_DIG2, false);
}
