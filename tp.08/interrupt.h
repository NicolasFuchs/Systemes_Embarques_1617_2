#pragma once
#ifndef INTERRUPT_H
#define INTERRUPT_H

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
 * Project:	HEIA-FR / Embedded Systems 2 Laboratory
 *
 * Abstract: 	Interrupt handling demo and test program
 *
 * Purpose:	Main module to demonstrate and to test the ARM Cortex-A8
 *              low level interrupt handling.
 *
 * Author: 	<Nicolas Fuchs et Alan Sueur>
 * Date: 	<13.03.17>
 */


/**
 * ARM interrupt vectors enumeration
 */
enum interrupt_vectors {
	INT_UNDEF,		///< undefined instruction
	INT_SVC,  		///< supervisor call (software interrupt)
	INT_PREFETCH, 	///< prefetch abort (instruction prefetch)
	INT_DATA,   	///< data abort (data access)
	INT_IRQ,   		///< hardware interrupt request
	INT_FIQ,   		///< hardware fast interrupt request
	INT_NB_VECTORS};

/**
 * Prototype of the interrupt handler
 *
 * @param vector interrupt vector
 * @param param parameter specified while attaching the interrupt handler 
 */
typedef void (*interrupt_handler_t) (
	enum interrupt_vectors vector,
	void* param);


/**
 * Method to initialize low level resources of the microprocessor.
 * At least a 8KiB of memory will be allocated for each interrupt vector
 */
extern void interrupt_init();


/**
 * Method to attach an interrupt handler to the interrupt vector table 
 *
 * @param vector interrupt vector
 * @param routine interrupt handler to attach to the specified vector
 * @param param parameter to be passed as argument while calling the the 
 *              specified interrupt handler
 * @return execution status, 0 if sussess, -1 if already attached
 */
extern int interrupt_attach (enum interrupt_vectors vector, 
		interrupt_handler_t routine,
		void* param);


/**
 * Method to detach an interrupt handler from the interrupt vector table
 *
 * @param vector interrupt vector
 */
extern void interrupt_detach (enum interrupt_vectors vector);


/**
 * Method to enable interrupt requests
 */
extern void interrupt_enable();


/**
 * Method to disable interrupt requests
 *
 * @return value of cpsr before disabling interrupt requests
 */
extern int interrupt_disable();


#endif
