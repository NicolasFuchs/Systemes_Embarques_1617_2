#pragma once
#ifndef SEG7_H
#define SEG7_H
/**
 * Copyright 2015 University of Applied Sciences Western Switzerland / Fribourg
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
 * Abstract:	7-Segment Display Device Driver
 *
 * Purpose:	This module implements a method to diplay a value in range
 *   		of -99 to 99 on a two 7-segments display of the HEIA-FR 
 * 		extension board of the Beaglebone black.
 *
 * Author: 	Daniel Gachet
 * Date: 	25.10.2015
 */

/**
 * method to initialize the resoures of the 7-segment display
 * this method shall be called prior any other.
 */
extern void seg7_init();


/**
 * method to display a value [-99..99] on the the 7-segments display
 * for negative value, a dot will be displayed.
 * 
 * @param value value to display
 */
extern void seg7_display_value (int value);

#endif


