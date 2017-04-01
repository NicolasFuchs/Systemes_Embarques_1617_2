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
 * Project:	HEIA-FR / Embedded Systems 2 Laboratory
 *
 * Abstract: 	Interrupt handling demo and test program
 *
 * Purpose:	Main module to demonstrate and to test the TI AM335x
 *              hardware interrupt handling.
 *
 * Author: 	<Nicolas Fuchs & Alan Sueur>
 * Date: 	<27.03.2017>
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
#include <am335x_gpio.h>

#include "interrupt.h"
#include "exception.h"
#include "timer.h"
#include "intc.h"
#include "seg7.h"
#include "wheel.h"
#include "thermo.h"
#include "buzzer.h"

#define DISPLAY_TEMP 0
#define DISPLAY_MAX_TEMP 1
#define DISPLAY_MIN_TEMP 2
#define DISPLAY_MENU 3
#define NB_MODES = 4

#define MENU_TEMP 0
#define MENU_TEMP_MAX 1
#define MENU_TEMP_MIN 2

struct program {
	uint32_t program_state;
	uint32_t temp_max;
	uint32_t temp_min;
	uint32_t temp;
	uint32_t menu;
};
static struct program program;

static void handle_incr(int* value, enum wheel_events event, int min, int max) {
	if (event == WHEEL_LEFT && *value > min) {
		(*value)--;
	} else if (event == WHEEL_RIGHT && *value < max) {
		(*value)++;
	}
	printf("value = %d\n", *value);
}

static void intc2gpio_ih (enum intc_vectors vector, void* param) {
	(void)vector;
	am335x_gpio_interrupt_hanlder((enum am335x_gpio_modules)param);
}

static void seg7refresh_ih (enum timer_timers timer, void* param) {
	(void)timer; (void)param;
	seg7_refresh_display();
}

static void thermo_read_ih (enum timer_timers timer, void* param) {
	(void)timer; (void)param;
	program.temp_max = thermo_get_limit(THERMO_T_HIGH);
	program.temp_min = thermo_get_limit(THERMO_T_LOW);
	program.temp = thermo_get_temp();
}


static void wheel_event_ih(enum wheel_events event) {
	switch (program.program_state) {
		case DISPLAY_MAX_TEMP:	handle_incr(&program.temp_max, event, INT_MIN, INT_MAX);
								thermo_set_limit(THERMO_T_HIGH,program.temp_max);
								break;
		case DISPLAY_MIN_TEMP:	handle_incr(&program.temp_min, event, INT_MIN, INT_MAX);
								thermo_set_limit(THERMO_T_LOW,program.temp_min);
								break;
		case DISPLAY_MENU:		int value = program.program_state;
								handle_incr(&value, event, 0, NB_MODES-1);
								program.program_state = value;
								break;
	}
}

static void button_pressed_ih() {
	switch (program.program_state) {
		case DISPLAY_MAX_TEMP:
		case DISPLAY_MIN_TEMP:
		case DISPLAY_TEMP:	program.program_state = DISPLAY_MENU; break;
		case DISPLAY_MENU:	switch (program.menu) {
								case MENU_TEMP:		program.program_state = DISPLAY_TEMP; break;
								case MENU_TEMP_MAX:	program.program_state = DISPLAY_MAX_TEMP; break;
								case MENU_TEMP_MIN:	program.program_state = DISPLAY_MIN_TEMP; break;
							}
							break;
	}
}

static void thermo_alarm(bool is_alarm_on) {
	if (is_alarm_on) {
		buzzer_switch_on();
	} else {
		buzzer_switch_off();
	}
}

int main () {
	printf ("\n");
	printf ("HEIA-FR - Embedded Systems 2 Laboratory\n");
	printf ("TI AM335x Hardware Interrupt Handling Test Program\n");
	printf ("--------------------------------------------------\n");

	interrupt_init();
	exception_init();
	intc_init();
	timer_init();
	seg7_init();
	buzzer_init();
	wheel_init(wheel_event_ih, button_pressed_ih);
	thermo_init(thermo_alarm);

	program.menu = 0;
	program.program_state = DISPLAY_TEMP;

	interrupt_enable();

	timer_attach(TIMER_2,  10, seg7refresh_ih, 0);
	timer_attach(TIMER_3, 500, thermo_read_ih, 0);

	// attach gpio interrupt handler to intc_gpio1a interrupt vector
	intc_attach (INTC_GPIO0A, intc2gpio_ih, (void*)AM335X_GPIO0);
	intc_attach (INTC_GPIO1A, intc2gpio_ih, (void*)AM335X_GPIO1);
	intc_attach (INTC_GPIO2A, intc2gpio_ih, (void*)AM335X_GPIO2);

	/* main loop */
	while(1);

		if (program.menu == DISPLAY_MENU) {
			seg7_display_value(program.program_state);
		} else {
			switch (program.program_state) {
				case DISPLAY_TEMP:		seg7_display_value(program.temp); break;
				case DISPLAY_MIN_TEMP:	seg7_display_value(program.temp_min); break;
				case DISPLAY_MAX_TEMP:	seg7_display_value(program.temp_max); break;
			}
		}

	return 0;
}
