#pragma once
#ifndef TIMER_H
#define TIMER_H
/**
 * Copyright 2017 University of Applied Sciences Western Switzerland / Fribourg
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
 * This module is based on the software library developped by Texas Instruments
 * Incorporated - http://www.ti.com/ for its AM335x starter kit.
 *
 * Project:	HEIA-FR / Embedded Systems 2 Laboratory
 *
 * Abstract: 	AM335x DMTimer2 to DMTimer7
 *
 * Purpose:	This module implements basic services to drive the AM335x Timers
 *
 * Author: 	Daniel Gachet
 * Date: 	19.03.2017
 */

/**
 * List of available timers
 */
enum timer_timers {TIMER_2, TIMER_3, TIMER_4,
		   TIMER_5, TIMER_6, TIMER_7, 
		   NB_OF_TIMERS};

/**
 * Prototype of the interrupt service routine
 *
 * @param timer AM335x timer id
 * @param param application specific parameter 
 */
typedef void (*timer_isr_t) (enum timer_timers timer, void* param);

/**
 * method to initialize the AM335x Timer resources.
 * should be called prior any other methods.
 */
extern void timer_init();

/**
 * method to configure a AM335x Timer for a specific period and to attach an 
 * application specific interrupt service routine
 *
 * @param timer timer number to which the ISR should be attached
 * @param period timer period in milliseconds
 * @param routine application specific interrupt service routine
 * @param param parameter passed as argument to the ISR when called
 * @return execution status (0=success, -1=error)
 */
extern int timer_attach(
	enum timer_timers timer, 
	uint32_t period, 
	timer_isr_t routine,
	void* param);


/**
 * method to reset the AM335x Timer and to detach the application specific 
 * interrupt service routine
 *
 * @param timer timer number to which the ISR should be detached
 */
extern void timer_detach(enum timer_timers timer);

#endif
