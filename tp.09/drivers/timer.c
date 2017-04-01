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

#include <stdint.h>
#include <am335x_clock.h>

#include "timer.h"
#include "intc.h"

/* -- Internal types and constant definition -------------------------------- */

// DMTimer IRQSTTATUS register bit definition
#define IRQSTATUS_MAT_IT_FLAG	(1<<0)
#define IRQSTATUS_OVF_IT_FLAG	(1<<1)

// DMTimer TIOCP_CFG register bit definition
#define TIOCP_CFG_SOFTRESET 	(1<<0)

// DMTimer IRQENABLE register bit definition
#define IRQENABLE_MAT_IT_FLAG	(1<<0)
#define IRQENABLE_OVF_IT_FLAG	(1<<1)

// DMTimer TCLR register bit definition
#define TCLR_ST					(1<<0)
#define TCLR_AR					(1<<1)
#define TCLR_CE					(1<<6)
#define TCLR_TRG_OVF			(1<<10)

/**
 * DMTimer Register Definition (timer 2 to 7)
 */
struct timer_ctrl {
	uint32_t tidr;		// 00
	uint32_t res1[3];	// 04-0c
	uint32_t tiocp_cfg;	// 10
	uint32_t res2[3];	// 14-1c
	uint32_t irq_eoi;	// 20
	uint32_t irqstatus_raw;	// 24
	uint32_t irqstatus;	// 28
	uint32_t irqenable_set;	// 2c
	uint32_t irqenable_clr;	// 30
	uint32_t irqwakeen;	// 34
	uint32_t tclr;		// 38
	uint32_t tcrr;		// 3c
	uint32_t tldr;		// 40
	uint32_t ttgr;		// 44
	uint32_t twps;		// 48
	uint32_t tmar;		// 4c
	uint32_t tcar1;		// 50
	uint32_t tsicr;		// 54
	uint32_t tcar2;		// 58
};

/**
 * DMTimer registers
 */
static volatile struct timer_ctrl* timer_ctrl[] = {
	(volatile struct timer_ctrl*) 0x48040000, 
	(volatile struct timer_ctrl*) 0x48042000,
	(volatile struct timer_ctrl*) 0x48044000, 
	(volatile struct timer_ctrl*) 0x48046000,
	(volatile struct timer_ctrl*) 0x48048000, 
	(volatile struct timer_ctrl*) 0x4804a000,
};

/**
 * DMTimer ISR Handler Structure Definition
 */
struct timer_isr_handlers {
	timer_isr_t routine;	/* application specific interrupt routine */
	void* param;		/* application specific parameter */
	uint32_t period;	/* timer period in milliseconds */
	enum timer_timers timer;	/* timer in use */
	volatile struct timer_ctrl* ctrl;	/* pointer to timer registers */
};
static struct timer_isr_handlers handlers[NB_OF_TIMERS];


/**
 * DMTimer Number to INTC Interrupt Vector Translation Table
 */
static const enum intc_vectors timer2intc[] = {
	INTC_TIMER2, INTC_TIMER3, INTC_TIMER4,
	INTC_TIMER5, INTC_TIMER6, INTC_TIMER7,
};

static const enum am335x_clock_timer_modules timer2clock[] = {
	AM335X_CLOCK_TIMER2, AM335X_CLOCK_TIMER3,
	AM335X_CLOCK_TIMER4, AM335X_CLOCK_TIMER5,
	AM335X_CLOCK_TIMER6, AM335X_CLOCK_TIMER7
};


/* -- Internal methods definition ------------------------------------------- */

static void timer_isr(enum intc_vectors vector, void* param) {
	(void)vector;
	struct timer_isr_handlers* oref = (struct timer_isr_handlers*) param;
	oref->ctrl->irqstatus = IRQSTATUS_OVF_IT_FLAG;
	oref->routine(oref->timer, oref->param);
}


/* -- Public methods definition --------------------------------------------- */

void timer_init() {
}

/* -------------------------------------------------------------------------- */

int timer_attach(enum timer_timers timer, uint32_t period, timer_isr_t routine, void* param) {
	timer_init();

	int status = -1;

	struct timer_isr_handlers* oref = &(handlers[timer]);
	volatile struct timer_ctrl* ctrl = timer_ctrl[timer];

	if ((timer < NB_OF_TIMERS) && (oref->routine == 0)) {
		am335x_clock_enable_timer_module (timer2clock[timer]);

		oref->routine = routine;
		oref->param = param;
		oref->period = period * 24000;
		oref->ctrl = ctrl;
		oref->timer = timer;

		ctrl->tiocp_cfg = TIOCP_CFG_SOFTRESET; // force a software reset
		while ((ctrl->tiocp_cfg & TIOCP_CFG_SOFTRESET) != 0);

		status = intc_attach(timer2intc[timer], timer_isr, oref);
	}

	if (status == 0) {
		ctrl->tmar = 0;
		ctrl->tldr = -oref->period;
		ctrl->tcrr = -oref->period;
		ctrl->tidr = -oref->period;
		ctrl->irqenable_set = IRQENABLE_OVF_IT_FLAG;
		ctrl->tclr = TCLR_AR | TCLR_ST;
	}

	return status;
}

/* ------------------------------------------------------------------------- */

void timer_detach(enum timer_timers timer) {
	if (timer >= NB_OF_TIMERS) return;

	volatile struct timer_ctrl* ctrl = timer_ctrl[timer];
	ctrl->irqenable_set = IRQENABLE_OVF_IT_FLAG;
	ctrl->tiocp_cfg = TIOCP_CFG_SOFTRESET;


	intc_detach(timer2intc[timer]);

	struct timer_isr_handlers* oref = &(handlers[timer]);
	oref->routine = 0;
	oref->param = 0;
	oref->period = 0;
	oref->ctrl = 0;
}
