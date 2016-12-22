#pragma once
#ifndef SEG7_H
#define SEG7_H
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
 * Abstract:    7-Segment Display Device Driver
 *
 * Purpose: This module implements a method to diplay a value in range
 *          of -99 to 99 on a two 7-segments display of the HEIA-FR
 *      extension board of the Beaglebone black.
 *
 * Author:  Daniel Gachet
 * Date:    26.10.2016
 */

/**
 * method to initialize the resoures of the 7-segment display
 * this method shall be called prior any other.
 */
extern void seg7_init();


/**
 * method to display a value [0..99] on the the 7-segments display
 * for values below 10, a dot will be displayed.
 * 
 * @param value value to display
 * @param is_dot
 */
extern void seg7_display_value (int value, bool is_dot);


/**
 * method to refresh the 7-segment display.
 */
extern void seg7_refresh_display();


/**
 * method to display zero.
 */
extern void seg7_display_zero();


/**
 * method to display two dashs.
 */
extern void seg7_display_null();

#endif
