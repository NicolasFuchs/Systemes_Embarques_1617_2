#pragma once
#ifndef BUTTONS_H
#define BUTTONS_H
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
 * Abstract:	HEIA-FR Extension Board Buttons Device Driver
 *
 * Purpose:	This module implements a method to get the state of one of the
 *		3 buttons of the HEIA-FR Beaglebone black extension board.
 *
 * Author: 	Daniel Gachet
 * Date: 	21.11.2016
 */

#include <stdbool.h>

/**
 * method to initialize the resoures of the module
 * this method shall be called prior any other.
 */
extern void buttons_init();


/**
 * method to get the state of one of the 3 buttons.
 * 
 * @param button number of the button to get the state (in range 1..3)
 * @return button state, true --> button is pressed, false otherwise
 */
extern bool buttons_is_pressed (unsigned button);


#endif


