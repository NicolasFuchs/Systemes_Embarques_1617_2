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
 * Author: 	Daniel Gachet
 * Date: 	21.03.2016
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <am335x_gpio.h>

#include "interrupt.h"
#include "exception.h"
#include "timer.h"
#include "intc.h"
#include "seg7.h"
#include "wheel.h"
#include "thermo.h"
#include "buzzer.h"

void wheel_menu(enum wheel_events event);
void wheel_tlow(enum wheel_events event);
void wheel_thigh(enum wheel_events event);
void wheel_temp(enum wheel_events event);

enum menus {TEMP, T_LOW, T_HIGH, MENU, MENU_MAX=MENU, NB_MENUS};
static unsigned menu = TEMP;
static unsigned display = TEMP;
static int values[NB_MENUS];

typedef void(*wheel_event_handler_t)(enum wheel_events event);
wheel_event_handler_t wheel_event_handler = wheel_menu; 

wheel_event_handler_t wheel_event_handlers[] = {
	[TEMP]   = wheel_temp,
	[T_LOW]  = wheel_tlow,
	[T_HIGH] = wheel_thigh,
	[MENU]   = wheel_menu,
};


void display_value(enum menus menu, int val)
{
	values[menu] = val;
	seg7_display_value(values[display]);	
}

static void intc2gpio_ih (enum intc_vectors vector, void* param)
{
	(void)vector;
	am335x_gpio_interrupt_hanlder ((enum am335x_gpio_modules)param);
}

static void seg7refresh_ih (enum timer_timers timer, void* param)
{
	(void)timer; (void)param;
	seg7_refresh_display();
}

static void thermo_read_ih (enum timer_timers timer, void* param)
{
	(void)timer; (void)param;
	int temp = thermo_get_temp();
	display_value(TEMP, temp);
}

void wheel_menu(enum wheel_events event)
{
	if (event == WHEEL_RIGHT) {
		menu++;
		if (menu >= MENU_MAX) menu = 0;
	} else if (event == WHEEL_LEFT) {
		menu--;
		if (menu >= MENU_MAX) menu = MENU_MAX-1;
	}
	display_value(MENU, menu);
}

void wheel_tlow(enum wheel_events event)
{
	int temp = thermo_get_limit(THERMO_T_LOW);
	if (event == WHEEL_RIGHT) {
		temp++;
		if (temp >= 75) menu = 75;
	} else if (event == WHEEL_LEFT) {
		temp--;
		if (temp <= -75) temp = -75;
	}
	display_value(T_LOW, temp);
	thermo_set_limit(THERMO_T_LOW, temp);
}

void wheel_thigh(enum wheel_events event)
{
	int temp = thermo_get_limit(THERMO_T_HIGH);
	if (event == WHEEL_RIGHT) {
		temp++;
		if (temp >= 75) menu = 75;
	} else if (event == WHEEL_LEFT) {
		temp--;
		if (temp <= -75) temp = -75;
	}
	display_value(T_HIGH, temp);
	thermo_set_limit(THERMO_T_HIGH, temp);
}

void wheel_temp(enum wheel_events event)
{
	(void)event;
	int temp = thermo_get_temp();
	display_value(TEMP, temp);
}

void wheel_event_ih(enum wheel_events event)
{
	wheel_event_handler(event);
}

void button_pressed_ih()
{
	static bool on=false;
	on = !on;
	if (on) {
		display=MENU;
	} else {
		display=menu;
	}
	wheel_event_handler = wheel_event_handlers[display];
	wheel_event_handler(WHEEL_STILL);
}

static void thermo_alarm(bool is_alarm_on)
{
	if (is_alarm_on) {
		buzzer_switch_on();
	} else {
		buzzer_switch_off();
	}
}

int main ()
{
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
	intc_attach (INTC_GPIO0A, intc2gpio_ih, (void*)AM335X_GPIO0);
	intc_attach (INTC_GPIO1A, intc2gpio_ih, (void*)AM335X_GPIO1);
	intc_attach (INTC_GPIO2A, intc2gpio_ih, (void*)AM335X_GPIO2);

	/* main loop */
	while(1);

	return 0;
}
