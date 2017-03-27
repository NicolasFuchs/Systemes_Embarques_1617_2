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
 * Author: 	<authors>
 * Date: 	<date>
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

static void intc2gpio_ih (enum intc_vectors vector, void* param)
{
	(void)vector;
	am335x_gpio_interrupt_hanlder ((enum am335x_gpio_modules)param);
}

static void seg7refresh_ih (enum timer_timers timer, void* param)
{
	(void)timer; (void)param;
}

static void thermo_read_ih (enum timer_timers timer, void* param)
{
	(void)timer; (void)param;
}


static void wheel_event_ih(enum wheel_events event)
{
	(void)event;
}

static void button_pressed_ih()
{
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
