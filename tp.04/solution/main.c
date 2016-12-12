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
 * Abstract:	Introduction the C programming language
 *
 * Purpose:	Simple C program implementing basic access to the GPIO modules
 *		of the AM3358 microprocessor to control the wheel and the
 * 		7-segment display of the HEIA-FR extension card of the 
 *		Beaglebone Black board 
 *
 * Author: 	Daniel Gachet
 * Date: 	26.10.2016
 */

#include <stdio.h>
#include <stdbool.h>

#include "seg7.h"
#include "wheel.h"


static void first_version()
{
	// counter...
	int counter =  0;
	while(true) {
		seg7_display_value(counter);

		enum wheel_states state = wheel_get_state();
		switch (state) {
		case WHEEL_INCR:  
			if (counter < 99) counter++;
			break;

		case WHEEL_DECR:	 
			if (counter > -99) counter--;
			break;

		case WHEEL_RESET:  
			return;

		default: 		// non supported state, ignore it...
			break;
		}
	}
}

static void second_version()
{
	// counter...
	int counter =  0;
	while(true) {
		seg7_display_value(counter);

		enum wheel_direction dir = wheel_get_direction();
		switch (dir) {
		case WHEEL_RIGHT:  
			if (counter < 99) counter++;
			break;

		case WHEEL_LEFT:	 
			if (counter > -99) counter--;
			break;

		default: 		
			break;
		}

		if (wheel_button_is_pressed()) {
			return;
		}
	}
}


int main()
{
	// print program banner
	printf ("HEIA-FR - Embedded Systems 1 Laboratory\n"
		"An introduction the C programming language\n"
		"--> 7-segment and wheel demo program\n");

	// initialization 
	seg7_init();
	wheel_init();
/*
	// test 7-segment module
	// countdown from -99 to 99 and display value on 7-segment
	for (int i=-99; i<=99; i++) {
		for (int j=300; j>0; j--) {
			seg7_display_value(i);
		}
	}
*/
	while (true) {
		first_version();
		second_version();
	}

	return 0;
}

