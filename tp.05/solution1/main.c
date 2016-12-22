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
 * Author: 	Alan Sueur, Jonathan Rial
 * Date: 	17.12.2016
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "buttons.h"
#include "seg7.h"
#include "leds.h"
#include "wheel.h"
#include "dmtimer1.h"

// local variables
static uint32_t counter;
static uint32_t old_counter;
static uint32_t initial_counter;
static bool is_lower_ten;


// local method implementation -----------------------------------------------

/**
 * Launch the timer at the screen.
 */
void launch_timer() {
	uint32_t value = is_lower_ten ? dmtimer1_get_second_and_tenth() : dmtimer1_get_second();
	counter = old_counter + value;

	// Transition from tenth of seconds to seconds, executed only once.
	if (counter >= 100 && is_lower_ten) {
		is_lower_ten = 0;
		old_counter = 10;
		counter = 10;
		dmtimer1_reset();
	}
}


/**
 * Launch the countdown at the screen.
 */
void launch_countdown() {
	uint32_t value = is_lower_ten ? dmtimer1_get_second_and_tenth() : dmtimer1_get_second();
	counter = initial_counter - old_counter - value;

	// Transition from seconds to tenths of seconds, executed only once.
	if (counter <= 10 && !is_lower_ten) {
		is_lower_ten = 1;
		initial_counter = 100;
		old_counter = 0;
		dmtimer1_reset();
	}
}


/**
 * Set the default value of the countdown, only available at beginning.
 */
void set_countdown() {
	enum wheel_states state = wheel_get_state();
	switch (state) {
		// Increments the counter.
		case WHEEL_INCR:
			if (counter >= 99 && is_lower_ten) {
				counter = 10;
				is_lower_ten = 0;
			} else if (counter < 99)
				counter++;
			break;

		// Decrement the counter.
		case WHEEL_DECR:
			if (counter <= 10 && !is_lower_ten) {
				counter = 99;
				is_lower_ten = 1;
			} else if (counter > 0)
				counter--;
			break;

		default:
			break;
	}
}


/**
 * Set the default display of the leds and timer, execute when a button is pressed.
 *
 * @param led The led to turn on.
 */
void init_all(enum leds_set led)
{
	switch (led) {
		case 0:
			leds_set_state(LED1, true);
			leds_set_state(LED2, false);
			break;
		case 1:
			leds_set_state(LED1, false);
			leds_set_state(LED2, true);
			break;
		default:
			leds_set_state(LED1, false);
			leds_set_state(LED2, false);
			break;
	}
	is_lower_ten = 1;
	counter = 0;
	old_counter = 0;
}


// main method implementation -----------------------------------------------

int main()
{
	// Print program banner
	printf("HEIA-FR - Embedded Systems 1 Laboratory\n"
			"An introduction to device driver development in C\n"
			"--> AM335x DMTimer1 device driver\n");

	// Initialization
	buttons_init();
	seg7_init();
	leds_init();
	wheel_init();
	dmtimer1_init();

	// Variables declarations
	uint32_t state;
	bool first_start;
	bool start;

	bool button_last_state = 0;
	bool button_current_state;

	while (true) {
		button_current_state = wheel_button_is_pressed();
		// Listeners on buttons
		if (buttons_get_state(BUTTON1)) {
			init_all(LED1);
			first_start = 1;
			start = 0;
			state = 0;
		} else if (buttons_get_state(BUTTON2)) {
			init_all(LED2);
			first_start = 1;
			start = 0;
			state = 1;
		} else if (buttons_get_state(BUTTON3)) {
			init_all(-1);
			state = 2;
		}


		// Look for the state in which we are.
		switch(state) {
			// TIMER STATE
			case 0:
				// When wheel pressed, looks that the action is not performed several times if the button is kept pressed.
				if (button_current_state != button_last_state && button_current_state == 1) {
					// If in paused or not started.
					if (!start) {
						dmtimer1_reset();
						first_start = 0;
						start = 1;
					// If in started.
					} else {
						old_counter = counter;
						start = 0;
					}
				}

				if (start)
					launch_timer();

				if (first_start)
					seg7_display_zero();
				else
					seg7_display_value(counter, is_lower_ten);

				break;

			// DISCOUNT STATE
			case 1:
				// When wheel pressed, looks that the action is not performed several times if the button is kept pressed.
				if (button_current_state != button_last_state && button_current_state == 1) {
					// If in paused or not started.
					if (!start) {
						if (first_start) { // attendre ici
							initial_counter = counter;
							first_start = 0;
						}
						dmtimer1_reset();
						start = 1;
					// If in started.
					} else {
						old_counter = initial_counter - counter;
						start = 0;
					}
				}

				if (start && counter > 0)
					launch_countdown();

				if (first_start) {
					set_countdown();
					if (counter == 0)
						seg7_display_zero();
					else
						seg7_display_value(counter, is_lower_ten);
				}
				else if (counter < 400000000 && counter > 0) {
					seg7_display_value(counter, is_lower_ten);
				}
				else {
					counter = 0;
					start = 0;
					first_start = 1;
					old_counter = 0;
				}

				break;

			case 2:
				seg7_display_null();
				break;

			default:
				break;
		}

		button_last_state = button_current_state;
	}

	return 0;
}
