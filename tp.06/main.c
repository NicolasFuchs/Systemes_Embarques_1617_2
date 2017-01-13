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
 * Abstract:	Introduction to device driver and application development in C
 *
 * Purpose:	Demo program implementing a basic thermometer with sound alarm
 *		application, which deploys a click board Thermo3 to measure 
 *		the ambiant temperature and a click board Buzz to generate 
 *		sound alarm.
 *
 * Author: 	<author's>
 * Date: 	<date>
 */

#include <stdio.h>
#include <stdbool.h>
#include "buzzer.h"
// ----------------------------------------------------------------------------

int main()
{
	// print program banner
	printf ("HEIA-FR - Embedded Systems 1 Laboratory\n"
		"An introduction to device driver and application development in C\n"
		"--> AM335x ePWM1 device driver\n"
		"--> I2C Thermometer (click board Thermo3)\n"
		"--> PWM Buzzer (click board Buzz)\n");


	buzzer_init();
	buzzer_set_frequency(3800);
	buzzer_on();

	// application...
	while(true) {

	}

	return 0;
}

