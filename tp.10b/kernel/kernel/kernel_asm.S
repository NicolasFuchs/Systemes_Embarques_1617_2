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
 * Project:		HEIA-FR / Embedded Systems 1 Laboratory
 * 						Personal Operating System
 *
 * Abstract:	Implementation of a basic cooperative Operating System
 *
 * Purpose:		Implementation of lowlevel services
 *
 * Author:		Daniel Gachet (first author)
 * 				Alan Sueur
 * 				Nicolas Fuchs
 *
 * Date: 		27.04.2017
 * Last update:	28.04.2017
 *
 */

/* ------------------------------------------------------------------------- */
/* Export public symbols */
	.global kernel_transfert
	.global kernel_interrupt_enable
	.global	kernel_interrupt_disable

	.bss
stack: .space 0x4000
	stack_top:

	.text


/* void kernel_transfer(struct tcb* former, uint32_t psr, struct tcb* new);
 * r0 = regions
 * r1 = number of regions
 * r2 = tcb address of the former thread (former)
 * r1 = tcb address of the new thread (new)
 */
kernel_transfert:
	nop
	stmia	r2, {r0-r12,sp,lr}  	// store r0-r12,sp,lr in former context (r0 is the lowest reg in list so r0 not incremented)
	mrs	r4, cpsr					// load cpsr in r2
	str	r4, [r2, #15*4]				// store r2 after the last (regs[15]) of tcb (cpsr)

	mov r4, r3
	ldr	sp, =stack_top
	bl mmu_configure_regions

	ldr	r2, [r4, #15*4]				// load cpsr of new context in r2
	msr	cpsr, r2					// store r2 in cpsr
	ldmia	r4, {r0-r12,sp,pc}		// load new context in r0-r12,sp,pc (r1 in ,list so not incremented)
/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
kernel_interrupt_enable:
	nop
	mrs	r0, cpsr
	bic	r0, #0xc0	// 0xc0 = bit 7 and bit 6 (IRQ and FIQ) (FIQ not active)
	msr	cpsr_c, r0
	bx	lr
/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
kernel_interrupt_disable:
	nop
	mrs	r0, cpsr
	orr	r0, #0xc0 // 0xc0 = bit 7 and bit 6
	msr	cpsr_c, r0
	bx	lr
/* ------------------------------------------------------------------------- */
