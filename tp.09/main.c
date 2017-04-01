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
 * Author: 	Nicolas Fuchs & Alan Sueur
 * Date: 	27.03.2017
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <am335x_gpio.h>
#include <limits.h>

#include "interrupt.h"
#include "exception.h"
#include "timer.h"
#include "intc.h"
#include "seg7.h"
#include "wheel.h"
#include "thermo.h"
#include "buzzer.h"

#define LOWER_BOUND 28
#define UPPER_BOUND 30

enum modes {
	DISPLAY,
	HIGH_BOUND,
	LOW_BOUND,
	NB_OF_MODES
};

struct ctrl {
	enum modes crt_mode;
	int lo_bound;
	int up_bound;
	int temp;
	bool is_bound_mode;
	bool is_menu;
};

struct ctrl ctrl = {
	.crt_mode = DISPLAY,
	.lo_bound = LOWER_BOUND,
	.up_bound = UPPER_BOUND,
	.temp = 0,
	.is_bound_mode = false,
	.is_menu = false
};

static void wheel_ctrl(int* value, enum wheel_events event, int min, int max)	{
	if(event == WHEEL_LEFT && *value > min)	{
		(*value)--;
	}
	else if(event == WHEEL_RIGHT && *value < max)	{
		(*value)++;
	}
}

static void intc2gpio_ih (enum intc_vectors vector, void* param) {
	(void)vector;
	am335x_gpio_interrupt_hanlder((enum am335x_gpio_modules)param);
}

static void seg7refresh_ih(enum timer_timers timer, void* param) {
	(void)timer; (void)param;
	seg7_refresh_display();
}

static void thermo_read_ih(enum timer_timers timer, void* param) {
	(void)timer; (void)param;
	ctrl.lo_bound = thermo_get_limit(THERMO_T_LOW);
	ctrl.up_bound = thermo_get_limit(THERMO_T_HIGH);
	ctrl.temp = thermo_get_temp();
}


static void wheel_event_ih(enum wheel_events event) {
	(void)event;
	if (ctrl.is_menu) {
		int value = ctrl.crt_mode;
		wheel_ctrl(&value, event, 0, NB_OF_MODES -1);
		ctrl.crt_mode = (enum modes) value;
	} else {
		switch(ctrl.crt_mode) {
		case LOW_BOUND:
			wheel_ctrl(&(ctrl.lo_bound), event, INT_MIN, INT_MAX);
			thermo_set_limit(THERMO_T_LOW, ctrl.lo_bound);
			break;
		case HIGH_BOUND:
			wheel_ctrl(&(ctrl.up_bound), event, INT_MIN, INT_MAX);
			thermo_set_limit(THERMO_T_HIGH, ctrl.up_bound);
			break;
		}
	}
}

static void button_pressed_ih() {
	if (!ctrl.is_menu && !ctrl.is_bound_mode) {
		ctrl.is_menu = true;
	} else if (!ctrl.is_menu && ctrl.is_bound_mode) {
		ctrl.is_bound_mode = false;
		ctrl.is_menu = true;
	} else {
		ctrl.is_menu = false;
		ctrl.is_bound_mode = true;
	}
}

static void thermo_alarm(bool is_alarm_on) {
	if (is_alarm_on) {
		buzzer_switch_on();
	} else {
		buzzer_switch_off();
	}
}

int main() {
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
	interrupt_enable();

	timer_attach(TIMER_2,  10, seg7refresh_ih, 0);
	timer_attach(TIMER_3, 500, thermo_read_ih, 0);

	// attach gpio interrupt handler to intc_gpio1a interrupt vector
	intc_attach(INTC_GPIO0A, intc2gpio_ih, (void*)AM335X_GPIO0);
	intc_attach(INTC_GPIO1A, intc2gpio_ih, (void*)AM335X_GPIO1);
	intc_attach(INTC_GPIO2A, intc2gpio_ih, (void*)AM335X_GPIO2);

	/* main loop */
	while(1) {
		if (ctrl.is_menu) {
			seg7_display_value(ctrl.crt_mode);
		} else {
			switch(ctrl.crt_mode) {
				case LOW_BOUND:	seg7_display_value(ctrl.lo_bound);	break;
				case HIGH_BOUND:seg7_display_value(ctrl.up_bound);	break;
				case DISPLAY:	seg7_display_value(ctrl.temp); 		break;
			}
		}
	}

	return 0;
}
