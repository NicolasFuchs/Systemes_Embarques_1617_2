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
 * Date: 	21.03.2016
 */

#include <am335x_gpio.h>

#include "seg7.h"

// macro to compute number of elements of an array
#define ARRAY_OF(x) (sizeof(x) / sizeof(x[0]))

// structure to initialize gpio pins used by 7-segment
static const struct gpio_init {
	enum am335x_gpio_modules module;
	uint32_t pin_nr;
} gpio_init[] = {
	{AM335X_GPIO2,  2},
	{AM335X_GPIO2,  3},
	{AM335X_GPIO2,  4},
	{AM335X_GPIO2,  5},

	{AM335X_GPIO0,  4},
	{AM335X_GPIO0,  5},
	{AM335X_GPIO0, 14},
	{AM335X_GPIO0, 22},
	{AM335X_GPIO0, 23},
	{AM335X_GPIO0, 26},
	{AM335X_GPIO0, 27},
};



// GPIO2: pin definition for 7-segment access
#define DIG1		(1<<2)
#define DIG2		(1<<3)
#define DP1		(1<<4)
#define DP2		(1<<5)
#define GPIO2_ALL	(DIG1 | DIG2 | DP1 | DP2)

// GPIO0: pin definition for 7-segment access
#define SEG_A		(1<<4)
#define SEG_B		(1<<5)
#define SEG_C		(1<<14)
#define SEG_D		(1<<22)
#define SEG_E		(1<<23)
#define SEG_F		(1<<26)
#define SEG_G		(1<<27)
#define GPIO0_ALL	(SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G)

/* 7-segement: segment definition

           +-- SEG_A --+
           |           |
         SEG_F       SEG_B
           |           |
           +-- SEG_G --+
           |           |
         SEG_E       SEG_C
           |           |
           +-- SEG_D --+
*/
static const uint32_t seg7[] = {
	(SEG_A + SEG_B + SEG_C + SEG_D + SEG_E + SEG_F 	    	), // 0
	(        SEG_B + SEG_C        			    	), // 1
     	(SEG_A + SEG_B                 + SEG_E + SEG_D + SEG_G 	), // 2
     	(SEG_A + SEG_B + SEG_C + SEG_D                 + SEG_G 	), // 3
     	(        SEG_B + SEG_C                 + SEG_F + SEG_G 	), // 4
     	(SEG_A         + SEG_C + SEG_D         + SEG_F + SEG_G 	), // 5
     	(SEG_A         + SEG_C + SEG_D + SEG_E + SEG_F + SEG_G 	), // 6
     	(SEG_A + SEG_B + SEG_C				    	), // 7
     	(SEG_A + SEG_B + SEG_C + SEG_D + SEG_E + SEG_F + SEG_G 	), // 8
     	(SEG_A + SEG_B + SEG_C + SEG_D + SEG_F + SEG_G	    	), // 9
     	(SEG_A + SEG_B + SEG_C + SEG_E + SEG_F + SEG_G	    	), // A
     	(                SEG_C + SEG_D + SEG_E + SEG_F + SEG_G 	), // b
     	(SEG_A                 + SEG_D + SEG_E + SEG_F         	), // C
     	(        SEG_B + SEG_C + SEG_D + SEG_E +         SEG_G 	), // d
     	(SEG_A                 + SEG_D + SEG_E + SEG_F + SEG_G 	), // E
     	(SEG_A                         + SEG_E + SEG_F + SEG_G 	), // F
     	(0							),

};

// -----------------------------------------------------------------------------
// implementation of public methods
// -----------------------------------------------------------------------------

void seg7_init()
{
	// initialize gpio 0 and 2 modules
	am335x_gpio_init(AM335X_GPIO0);
	am335x_gpio_init(AM335X_GPIO2);

	// configure gpio pins as output
	for (int i=ARRAY_OF(gpio_init)-1; i>=0; i--) {
		am335x_gpio_setup_pin_out(
			gpio_init[i].module, 
			gpio_init[i].pin_nr,
			false);
	}
}

// -----------------------------------------------------------------------------

void seg7_display_value (int value)
{
	// test for negative values
	bool dot = false;
	if (value < 0) {
		dot = true;
		value = -value;
	}

	// display unit
	am335x_gpio_change_states(AM335X_GPIO0, seg7[value%10], true);
	am335x_gpio_change_states(AM335X_GPIO2, DIG2, true);
	for (int i=80; i>0; i--);
	am335x_gpio_change_states(AM335X_GPIO2, GPIO2_ALL, false);
	am335x_gpio_change_states(AM335X_GPIO0, GPIO0_ALL, false);

	// display decade & dot
	if (dot) am335x_gpio_change_states(AM335X_GPIO2, DP1, true);
	am335x_gpio_change_states(AM335X_GPIO0, seg7[(value/10)%10], true);
	am335x_gpio_change_states(AM335X_GPIO2, DIG1, true);
	for (int i=80; i>0; i--);
	am335x_gpio_change_states(AM335X_GPIO2, GPIO2_ALL, false);
	am335x_gpio_change_states(AM335X_GPIO0, GPIO0_ALL, false);
}

