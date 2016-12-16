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
 * Abstract:	Introduction to device driver development in C
 *
 * Purpose:	Demo program implementing a basic timer and countdown
 *		    application, which is based on the AM335x DMTimer1 timer.
 *
 * Author: 	Jonathan Rial, Alan Sueur
 * Date: 	december 2016
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "buttons.h"
#include "leds.h"
#include "dmtimer1.h"

// ----------------------------------------------------------------------------
// main program...
// ----------------------------------------------------------------------------

int main() {
	// print program banner
	printf("HEIA-FR - Embedded Systems 1 Laboratory\n"
			"An introduction to device driver development in C\n"
			"--> AM335x DMTimer1 device driver\n");

	// initialization...
	buttons_init();
	leds_init();
	dmtimer1_init();

	// application...
	while (true) {
		if (get_button_state(BUTTON1)) {
			printf("Button 1 pressed\n");
			change_led_state(LED1, true);
			change_led_state(LED2, false);
			change_led_state(LED3, false);
		}
		else if (get_button_state(BUTTON2)) {
			printf("Button 2 pressed\n");
			change_led_state(LED1, false);
			change_led_state(LED2, true);
			change_led_state(LED3, false);
		}
		else if (get_button_state(BUTTON3)) {
			printf("Button 3 pressed\n");
			change_led_state(LED1, false);
			change_led_state(LED2, false);
			change_led_state(LED3, true);
		}
	}

	return 0;
}
