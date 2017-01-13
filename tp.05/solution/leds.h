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
 * Abstract:	HEIA-FR Extension Board LED Device Driver
 *
 * Purpose:	This module implements a method to drive the state of one of
 *		the 3 LED of the HEIA-FR Beaglebone black extension board.
 *
 * Author: 	Daniel Gachet
 * Date: 	21.11.2016
 */

#include <stdbool.h>

/**
 * method to initialize the resoures of the module
 * this method shall be called prior any other.
 */
extern void leds_init();


/**
 * method to drive the state of one of the 3 LED.
 * 
 * @param led number of the LED to change the state (in range 1..3)
 * @param state true --> turn on the LED, false otherwise
 */
extern void leds_set_state (unsigned led, bool state);


#endif


