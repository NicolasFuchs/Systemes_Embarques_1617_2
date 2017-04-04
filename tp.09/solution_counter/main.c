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
#include "intc.h"
#include "seg7.h"

static void intc_test(enum intc_vectors vector, void* param)
{
	printf ("INTC interrupt simulation on vector %d occurred\n", vector);
	intc_force (vector, false);
	intc_detach (vector);
	(*(bool*)param)=true;
}

static void gpio_test(enum am335x_gpio_modules module, uint32_t pin, void* param)
{
	printf ("INTC interrupt gpio %d on pin %lu occurred\n", module, pin);
	(*(int*)param)--;
}

static void gpio_inc(enum am335x_gpio_modules module, uint32_t pin, void* param)
{
	(void)module; (void)pin;
	(*(int*)param)++;
}

static void gpio_dec(enum am335x_gpio_modules module, uint32_t pin, void* param)
{
	(void)module; (void)pin;
	(*(int*)param)--;
}

static void gpio_clr(enum am335x_gpio_modules module, uint32_t pin, void* param)
{
	(void)module; (void)pin;
	(*(int*)param)=0;
}

static void intc2gpio (enum intc_vectors vector, void* param)
{
	(void)vector;
	am335x_gpio_interrupt_hanlder ((enum am335x_gpio_modules)param);
}


int main ()
{
	printf ("\n");
	printf ("HEIA-FR - Embedded Systems 2 Laboratory\n");
	printf ("TI AM335x Hardware Interrupt Handling Test Program\n");
	printf ("--------------------------------------------------\n");

	interrupt_init();
	exception_init();
	intc_init();
	seg7_init();
	interrupt_enable();

	// test intc interrupt processing
	bool intc_simul_has_occured[2] = {[0]=false, };
	intc_attach (12, intc_test, &intc_simul_has_occured[0]);
	intc_attach (15, intc_test, &intc_simul_has_occured[1]);
	intc_force  (12, true);
	intc_force  (15, true);
	while (!intc_simul_has_occured[0] || !intc_simul_has_occured[1]);

	// attach gpio interrupt handler to intc_gpio1a interrupt vector
	intc_attach (INTC_GPIO1A, intc2gpio, (void*)AM335X_GPIO1);

	// test gpio interrupt processing
	int gpio_simul_has_occured[3] = {[0]=1, [1]=1, [2]=2, };
	am335x_gpio_attach (AM335X_GPIO1, 15, AM335X_GPIO_IRQ_RISING, false, 
			    gpio_test, &gpio_simul_has_occured[0]);
	am335x_gpio_attach (AM335X_GPIO1, 16, AM335X_GPIO_IRQ_FALLING, false,
			    gpio_test, &gpio_simul_has_occured[1]);
	am335x_gpio_attach (AM335X_GPIO1, 17, AM335X_GPIO_IRQ_BOTH, false,
			    gpio_test, &gpio_simul_has_occured[2]);
	while (gpio_simul_has_occured[0] > 0);
	while (gpio_simul_has_occured[1] > 0);
	while (gpio_simul_has_occured[2] > 0);
	am335x_gpio_detach (AM335X_GPIO1, 15);
	am335x_gpio_detach (AM335X_GPIO1, 16);
	am335x_gpio_detach (AM335X_GPIO1, 17);

	// application initialization
	int counter = 0;
	am335x_gpio_attach (AM335X_GPIO1, 15, AM335X_GPIO_IRQ_FALLING, false, 
			    gpio_inc, &counter);
	am335x_gpio_attach (AM335X_GPIO1, 16, AM335X_GPIO_IRQ_FALLING, false,
			    gpio_clr, &counter);
	am335x_gpio_attach (AM335X_GPIO1, 17, AM335X_GPIO_IRQ_FALLING, false, 
			    gpio_dec, &counter);

	/* main loop */
	while(1) {
		seg7_display_value (counter > 99 ? -99 : counter);
	}

	return 0;
}
