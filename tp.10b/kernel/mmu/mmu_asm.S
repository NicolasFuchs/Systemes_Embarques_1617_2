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
 * This module is based on the software library developped by Texas Instruments
 * Incorporated - http://www.ti.com/ for its AM335x starter kit.
 *
 * Project:	HEIA-FR / Embedded Systems 1+2 Laboratory
 *
 * Abstract: 	AM335x - Cortex-A8 - Memory Management Unit
 *
 * Purpose:	This module implements low level services to drive the MMU
 *
 * Author: 	Daniel Gachet
 * Date: 	07.05.2016
 */
	.text

#define C1_CONTROL_M	(1<<0)	// MMU enable bit
#define C1_CONTROL_TRE	(1<<28)	// MMU TEX Remap 

// extern void mmu_enable();
	.global mmu_enable
mmu_enable:
	mrc	p15, 0, r0, c1, c0, 0
	orr	r0, #(C1_CONTROL_TRE | C1_CONTROL_M)
	mcr	p15, 0, r0, c1, c0, 0
	bx	lr


// extern void mmu_disable();
	.global mmu_disable
mmu_disable:
	mcr	p15, 0, r0, c8, c7, 0 // invalidate tlb
	mrc	p15, 0, r0, c1, c0, 0
	bic	r0, #C1_CONTROL_M
	mcr	p15, 0, r0, c1, c0, 0
	bx	lr


// extern void mmu_ll_set_ttrb0(void* addr);
	.global mmu_ll_set_ttrb0
mmu_ll_set_ttrb0:
	nop
	mcr	p15, 0, r0, c2, c0, 0 // Write Translation Table Base Register
	bx	lr


// extern void mmu_ll_set_dacr(uint32_t val);
	.global mmu_ll_set_dacr
mmu_ll_set_dacr:
	nop
	mcr	p15, 0, r0, c3, c0, 0
	bx	lr

// extern void  mmu_tlb_invalidate();
	.global mmu_ll_tlb_invalidate
mmu_ll_tlb_invalidate:
	nop
	mcr     p15, #0, r0, c8, c7, #0   // r0 value will be ignored
	dsb
	isb
	bx	lr


