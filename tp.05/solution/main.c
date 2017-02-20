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
 *		application, which is based on the AM335x DMTimer1 timer.
 *
 * Author: 	Daniel Gachet
 * Date: 	21.11.2016
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "dmtimer1.h"
#include "seg7.h"
#include "wheel.h"
#include "leds.h"
#include "buttons.h"

#define TIMER_BUTTON	1
#define CHRONO_BUTTON	2
#define RESET_BUTTON	3

#define TIMER_LED	1
#define CHRONO_LED	2

// ----------------------------------------------------------------------------

static inline bool is_reset_button_pressed()
{
	return buttons_is_pressed(RESET_BUTTON);
}

// ----------------------------------------------------------------------------

static void display_time_value(int value)
{
	if (value < 0) 
		value = 0;
	if (value < 100) {
		seg7_display_dot (true);
		seg7_display_value(value);
	} else {
		seg7_display_dot (false);
		seg7_display_value(value / 10);
	}
}

// ----------------------------------------------------------------------------

static void countdown_app()
{
	bool is_running = true;
	int cdown_value = 0;
	while(is_running) {
		// wait until countdown value is defined
		while(is_running) {
			is_running = !is_reset_button_pressed();
			if (wheel_button_is_pressed()) break;

			display_time_value(cdown_value);

			enum wheel_direction dir = wheel_get_direction();
			switch (dir) {
			case WHEEL_RIGHT:  
				if (cdown_value < 100) cdown_value++;
				else if (cdown_value < 990) cdown_value+=10;
				break;

			case WHEEL_LEFT:	 
				if (cdown_value > 100) cdown_value-=10;
				else if (cdown_value > 0) cdown_value--;
				break;

			default: 		
				break;
			}
		}
	
		// count down...
		uint64_t elapsed_ticks = 0;
		uint32_t start = dmtimer1_get_counter();
		uint32_t ticks_per_100ms = dmtimer1_get_frequency() / 10;
		while (is_running) {
			is_running = !is_reset_button_pressed();

			uint32_t stop  = dmtimer1_get_counter();
			uint32_t delta = stop - start;
			start = stop;
			elapsed_ticks += delta;
			int value = cdown_value - (elapsed_ticks / ticks_per_100ms);

			display_time_value (value);
			if ((value <= 0) || wheel_button_is_pressed()) {
				cdown_value = value;
				break;
			}
		}
	}
}

// ----------------------------------------------------------------------------

static void timer_app()
{
	bool is_running = true;
	int base_time = 0;
	while(is_running) {
		while(is_running) {
			is_running = !is_reset_button_pressed();
			if (wheel_button_is_pressed()) break;
			seg7_refresh_display();
		}

		uint64_t elapsed_ticks = 0;
		uint32_t start = dmtimer1_get_counter();
		uint32_t ticks_per_100ms = dmtimer1_get_frequency() / 10;
		int time = 0;
		while(is_running) {
			is_running = !is_reset_button_pressed();
			if (wheel_button_is_pressed()) break;

			uint32_t stop  = dmtimer1_get_counter();
			uint32_t delta = stop - start;
			start = stop;
			elapsed_ticks += delta;
			time = base_time + (elapsed_ticks / ticks_per_100ms);

			display_time_value (time);
		}
		base_time = time;
	}
}

// ----------------------------------------------------------------------------
// main program...
// ----------------------------------------------------------------------------

int main()
{
	// print program banner
	printf ("HEIA-FR - Embedded Systems 1 Laboratory\n"
		"An introduction to device driver development in C\n"
		"--> AM335x DMTimer1 device driver\n");

	// initialization...
	dmtimer1_init();
	seg7_init();
	wheel_init();
	leds_init();
	buttons_init();

	// application...
	while(true) {
		display_time_value (0);
		bool is_timer = buttons_is_pressed (TIMER_BUTTON);
		if (is_timer) {
			leds_set_state (TIMER_LED, true);
			timer_app();
			leds_set_state (TIMER_LED, false);
		}

		bool is_chrono = buttons_is_pressed (CHRONO_BUTTON);
		if (is_chrono) {
			leds_set_state (CHRONO_LED, true);
			countdown_app();
			leds_set_state (CHRONO_LED, false);
		}
	}

	return 0;
}

