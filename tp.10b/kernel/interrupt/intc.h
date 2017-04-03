#pragma once
#ifndef INTC_H
#define INTC_H
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
 * Abstract:	INTC - ARM Interrupt Controller 
 *
 * Purpose:	Module to deal with the ARM Interrupt Controller (INTC) of 
 * 		the TI AM335x.
 *
 *   		This module provides services to attach/detach an interrupt 
 *		handler to each individual interrupt source of the TI AM335x.
 * 		Only the default priority scheme of the TI AM335x is currently 
 *              implemented and all the sources will generate an IRQ.
 *
 * Author: 	Daniel Gachet
 * Date: 	21.03.2016
 */

#include <stdbool.h>


/**
 * List of available interrupt sources attached to the INTC.
 * This list provides the vector of each interrupt source according
 * to the TI AM335x Technical Reference Manual at chapter 6.
 *
 * A spurious interrupt vector has been added to enable application
 * software to treat unknown interrupts.
 */
enum intc_vectors {
	INTC_EDMACOMPINT=12,
	INTC_EDMAERRINT=14,

	INTC_MMC1=28,
	INTC_MMC2,

	INTC_GPIO2A=32,
	INTC_GPIO2B,

	INTC_CPSW_RX=41,
	INTC_CPSW_TX,

	INTC_GPIO3A=62,
	INTC_GPIO3B,
	INTC_MMC0=64,

	INTC_TIMER2=68,
	INTC_TIMER3,

	INTC_TIMER4=92,
	INTC_TIMER5,
	INTC_TIMER6,
	INTC_TIMER7,
	INTC_GPIO0A,
	INTC_GPIO0B,
	INTC_GPIO1A,
	INTC_GPIO1B,

	INTC_SPURIOUS,
	INTC_NB_OF_VECTORS=128
};

/**
 * Prototype of the interrupt service routine
 *
 * @param vector interrupt vector which raised the interrupt
 * @param param free user specified parameter
 */
typedef void (*intc_service_routine_t) (enum intc_vectors vector, void* param);


/**
 * initialization method
 * should be called prior any other method of this module
 */
extern void intc_init();


/**
 * method to attach an application specific interrupt service routine to 
 * the specified vector
 *
 * @param vector interrupt vector to which the application ISR should be attached
 * @param type interrupt type (normal interrupt, fast interrupt)
 * @param routine application specific interrupt service routine
 * @param param application specific parameter passed to the ISR during call
 *
 * @return execution status: 0 <=> success, -1 <=> error
 */
extern int intc_attach (
	enum intc_vectors vector, 
	intc_service_routine_t routine, 
	void* param);

/**
 * method to detach application specific interrupt service routine from the 
 * specified vector
 *
 * @param vector interrupt vector from which the application ISR should be detached
 */
extern void intc_detach (enum intc_vectors vector);


/**
 * method to force/simulate an interrupt request
 *
 * @param vector interrupt vector for which the interrupt should be forced/simulated
 * @param force true to force the interrupt request, false otherwise
 */
extern void intc_force (enum intc_vectors vector, bool force);
 
#endif

