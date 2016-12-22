#pragma once
#ifndef WHEEL_H
#define WHEEL_H
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
 * Abstract:    Wheel Device Driver
 *
 * Purpose: This module implements a method to get state of the wheel
 *      of the HEIA-FR extension board of the Beaglebone black.
 *
 * Author:  Daniel Gachet
 * Date:    26.10.2016
 */

#include <stdint.h>

/**
 * wheel states
 */
enum wheel_states {WHEEL_NONE, WHEEL_INCR, WHEEL_DECR, WHEEL_RESET};

/**
 * wheel rotation directions
 */
enum wheel_direction {WHEEL_STILL, WHEEL_RIGHT, WHEEL_LEFT};


/**
 * method to initialize the resoures of the wheel
 * this method shall be called prior any other.
 */
extern void wheel_init();


/**
 * method to get state of the wheel
 * 
 * @return wheel state 
 */
extern enum wheel_states wheel_get_state();


/**
 * method to get the direction of rotation of the encoder
 * 
 * @return direction of rotation 
 */
extern enum wheel_direction wheel_get_direction();


/**
 * method to test if the button of encoder is pressed
 * 
 * @return button is pressed 
 */
extern bool wheel_button_is_pressed();

#endif
