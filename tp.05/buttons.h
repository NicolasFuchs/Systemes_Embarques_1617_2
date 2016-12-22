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
 * Project: HEIA-FR / Embedded Systems 1 Laboratory
 *
 * Abstract:    Introduction to device driver development in C
 *
 * Purpose:  Module for managing the buttons of the HEIA-FR extension card of the
 *           Beaglebone Black board
 *
 * Author:  Alan Sueur, Jonathan Rial
 * Date:    17.12.2016
 */

#include <stdbool.h>

enum buttons_set {
    BUTTON1, BUTTON2, BUTTON3
};


/**
 * method to initialize the resoures of the button
 * this method shall be called prior any other.
 */
void buttons_init();


/**
 * method to get the state of a specific button.
 *
 * @param button the specific button
 */
bool buttons_get_state(enum buttons_set button);

#endif
