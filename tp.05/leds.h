#pragma once
#ifndef LEDS_H
#define LEDS_H
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

#include <stdbool.h>

/**
 * LEDs disponibles
 */
enum leds_set {
	LED1, LED2, LED3
};

/**
 * Cette méthode permet d'initialiser les LEDs
 */
void leds_init();

/**
 * Cette méthode permet d'allumer une seule LED.
 *
 * @param led la LED à allumer
 */
void change_led_state(enum leds_set led, bool state);

/**
 * Cette méthode permet de changer l'état d'une LED.
 *
 * @param led la LED à modifier
 * @param state l'état
 */
void enable_led_exclusively(enum leds_set led);

/**
 * Cette méthode permet de réinitialiser les LEDs.
 */
void leds_reset();

#endif
