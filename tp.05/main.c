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
#include "seg7.h"
#include "wheel.h"
#include "dmtimer1.h"

#define CHRONO_LED LED1
#define COUNTDOWN_LED LED2
#define RESET_BUTTON BUTTON3

// ----------------------------------------------------------------------------
// implementation of local methods...
// ----------------------------------------------------------------------------

void reset_all() {
	leds_reset();
	seg7_reset();
	dmtimer1_stop();
	dmtimer1_reset();
}

// ----------------------------------------------------------------------------

void chrono() {
	enable_led_exclusively(CHRONO_LED);
	bool is_counting = false;
	bool has_been_pressed = false;
	while (!get_button_state(RESET_BUTTON)) {
		bool is_pressed = wheel_button_is_pressed();
		if (is_pressed && !has_been_pressed) {
			if (is_counting) {
				dmtimer1_stop();
			} else {
				dmtimer1_start();
			}
			is_counting = !is_counting;
		}
		has_been_pressed = is_pressed;
		dmtimer1_refresh_time();
		seg7_display_value(dmtimer1_get_seconds());
	}
	reset_all();
}

// ----------------------------------------------------------------------------

int32_t change_counter(int32_t counter) {
	bool has_been_released = false;
	while (!get_button_state(RESET_BUTTON)) {
		enum wheel_states state = wheel_get_state();
		switch (state) {
		case WHEEL_INCR:
			if (counter < 99) {
				counter++;
			}
			break;
		case WHEEL_DECR:
			if (counter > 0) {
				counter--;
			}
			break;
		case WHEEL_RESET:
			if (has_been_released) {
				return counter;
			}
			break;
		default:
			has_been_released = true;
			break;
		}
		dmtimer1_refresh_time();
		seg7_display_value(counter);
	}
	return -1;
}

// ----------------------------------------------------------------------------

void countdown() {
	enable_led_exclusively(COUNTDOWN_LED);
	bool has_been_pressed = false;
	int32_t counter = 0;
	while (!get_button_state(RESET_BUTTON)) {
		bool is_pressed = wheel_button_is_pressed();
		if ((is_pressed && !has_been_pressed)
				|| (counter - dmtimer1_get_seconds() <= 0)) {
			is_pressed = true;
			dmtimer1_stop();
			counter = change_counter(counter - dmtimer1_get_seconds());
			if (counter == -1) break;
			dmtimer1_reset();
			dmtimer1_start();
		}
		has_been_pressed = is_pressed;
		dmtimer1_refresh_time();
		seg7_display_value(counter - dmtimer1_get_seconds());
	}
	reset_all();
}

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
	seg7_init();
	wheel_init();
	dmtimer1_init();

	// application...
	while (true) {
		if (get_button_state(BUTTON1)) {
			chrono();
		} else if (get_button_state(BUTTON2)) {
			countdown();
		}
	}

	return 0;
}
