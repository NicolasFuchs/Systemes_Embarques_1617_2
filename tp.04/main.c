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
 * Purpose: Simple C program implementing basic access to the GPIO modules
 *          of the AM3358 microprocessor to control the wheel and the
 *          7-segment display of the HEIA-FR extension card of the
 *          Beaglebone Black board
 *
 * Author: 	Jonathan Rial & Alan Sueur
 * Date: 	November 2016
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "wheel.h"
#include "seg7.h"

// Variable compteur
int32_t counter;

/**
 * Méthode principale du programme
 */
int main() {
	// Initialisation de la roue et de l'affichage 7 segments
	wheel_init();
	seg7_init();

	// Afficher la bannière du programme
	printf("HEIA-FR - Embedded Systems 1 Laboratory\n"
			"An introduction the C programming language\n"
			"--> 7-segment and wheel demo program\n");

	// Boucle infinie permettant de surveiller l'état de la roue et d'afficher le compteur
	while (true) {
		seg7_display_value(counter);
		enum wheel_state state = wheel_get_state();
		switch (state) {
		case WHEEL_INCR:
			if (counter < 99)
				counter++;
			break;
		case WHEEL_DECR:
			if (counter > -99)
				counter--;
			break;
		case WHEEL_RESET:
			counter = 0;
			break;
		default:
			break;
		}
	}
	return 0;
}

