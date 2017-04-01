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

#include <string.h>
#include <stdint.h>

#include "intc.h"
#include "interrupt.h"

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))

// INTC controller definition
struct intc_ctrl {
	uint32_t revision;		// 00
	uint32_t res1[3];		// 04-0c
	uint32_t sysconfig;		// 10
	uint32_t sysstatus;		// 14
	uint32_t res2[10];		// 18-3c
	uint32_t sir_irq;		// 40
	uint32_t sir_fiq;		// 44
	uint32_t control;		// 48
	uint32_t protection;	// 4c
	uint32_t idle;			// 50
	uint32_t res3[3];		// 54-5c
	uint32_t irq_priority;	// 60
	uint32_t fiq_priority;	// 64
	uint32_t threshold;		// 68
	uint32_t res4[5];		// 6c-7c
	struct {
		uint32_t itr;
		uint32_t mir;
		uint32_t mir_clear;
		uint32_t mir_set;
		uint32_t isr_set;
		uint32_t isr_clear;
		uint32_t pending_irq;
		uint32_t pending_fiq;
	} flags[4];
	uint32_t ilr[128];
};
	
static volatile struct intc_ctrl* intc = (struct intc_ctrl*) 0x48200000;

/**
 * INTC vector table containing all the application specific
 * interrupt service routines with their parameter
 */
struct intc_handler {
	intc_service_routine_t routine;
	void* param;
}; 
static struct intc_handler handlers[INTC_NB_OF_VECTORS];
static bool is_initialized = false;


/*-- local methods --------------------------------------------------------- */

/**
 * Normal/fast interrupt service routine. This method will look at the INTC to
 * get the highest pending interrupt vector and to call the related service
 * routine.
 */
void intc_irq_isr(enum interrupt_vectors int_vector, void* param) {
	(void)param; (void)int_vector;// unused arguments

	uint32_t vector = intc->sir_irq & 0x7f;

	if (handlers[vector].routine != 0) {
		handlers[vector].routine(vector, handlers[vector].param);
	} else {
		intc->flags[vector/32].mir_set = 1<<(vector%32);
	}

	intc->control = 0x1;

}


/*-- public methods -------------------------------------------------------- */

void intc_init() {
	if (is_initialized) return;

	memset (handlers, '\0', sizeof(handlers));
	interrupt_attach (INT_IRQ, intc_irq_isr, 0);

	// reset intc controller
	intc->sysconfig = 0x2;
	while ((intc->sysstatus & 0x1) == 0);

	// default config, no threshold
	intc->sysconfig = 0x0;
	intc->idle = 0x0;
	intc->threshold = 0xff;

	// mask all interrupt request lines
	for (int i = ARRAY_SIZE(intc->flags)-1; i >= 0; i--) {
		intc->flags[i].mir = -1;
		intc->flags[i].mir_set = -1;
		intc->flags[i].isr_clear = -1;
	}
	// 
	for (int i = ARRAY_SIZE(intc->ilr)-1; i >= 0; i--) {
		intc->ilr[i] = 0;
	}

	is_initialized = true;
}

/* ------------------------------------------------------------------------- */

int intc_attach (enum intc_vectors vector, intc_service_routine_t routine, void* param) {
	intc_init();
	int status = -1;
	if (vector < INTC_NB_OF_VECTORS && handlers[vector].routine == 0) {
		handlers[vector].routine = routine;
		handlers[vector].param = param;

		intc->ilr[vector] = 0;
		intc->flags[vector/32].mir_clear = (1<<(vector%32));
		status = 0;
	}

	return status;
}

/* ------------------------------------------------------------------------- */

void intc_detach (enum intc_vectors vector) {
	if (vector < INTC_NB_OF_VECTORS) {
		intc->flags[vector/32].mir_set = (1<<(vector%32));
		handlers[vector].routine = 0;
		handlers[vector].param = 0;
	}
}

/* ------------------------------------------------------------------------- */

void intc_force (enum intc_vectors vector, bool force) {
	if (vector < INTC_NB_OF_VECTORS) {
		if (force) {
			intc->flags[vector/32].isr_set = (1<<(vector%32));
		} else {
			intc->flags[vector/32].isr_clear = (1<<(vector%32));
		}
	}
}
