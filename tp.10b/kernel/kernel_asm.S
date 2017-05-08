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
 * Project:	HEIA-FR / Embedded Systems 2 
 *
 * Abstract: 	Introduction to Operating System
 *
 * Purpose:	Implementation of lowlevel services
 *
 * Autĥor:	Daniel Gachet / HEIA-FR
 * Date:	31.03.2017
 */

/* Export public symbols */
	.global kernel_transfer
	.global kernel_interrupt_enable
	.global	kernel_interrupt_disable

	.text
/* void kernel_transfer(struct tcb* former, uint32_t psr, struct tcb* new);
 * r0 = tcb address of the former thread (former)
 * r1 = tcb address of the new thread (new)
 */
kernel_transfer:
	nop
	stmia	r0,{r0-r12,sp,lr}
	mrs	r2, cpsr
	str	r2,[r0, #15*4]

	ldr	r2,[r1, #15*4]
	msr	cpsr, r2
	ldmia	r1, {r0-r12,sp,pc}

kernel_interrupt_enable:
	nop
	mrs	r0, cpsr
	bic	r0, #0xc0
	msr	cpsr_c, r0
	bx	lr

kernel_interrupt_disable:
	nop
	mrs	r0, cpsr
	orr	r0, #0xc0
	msr	cpsr_c, r0
	bx	lr


