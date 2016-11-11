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
 * Author: 	<author's>
 * Date: 	<date>
 */

#include <stdio.h>
#include <stdbool.h>
#include "am335_gpio.h"
#include "wheel.h"
#include "seg7.h"

int main(){
	// print program banner
	printf ("HEIA-FR - Embedded Systems 1 Laboratory\n"
		"An introduction the C programming language\n"
		"--> 7-segment and wheel demo program\n");

	seg7_init();
	wheel_init();

	int count = 0;
	enum wheel_states state = wheel_get_state();

	while(true) {
		state = wheel_get_state();
		switch(state){
		case INCR:
			if(count<99) count++;
			break;
		case DECR:
			if(count>-99) count--;
			break;
		case RESET:
			count=0;
			break;
		case NONE:
		default:
		}

		gpio_display(count);
	}

	return 0;
}

