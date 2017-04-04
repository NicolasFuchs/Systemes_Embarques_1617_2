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
 * Abstract: 	ARM Interrupt Handling - Low Level Interface
 *
 * Author: 	Daniel Gachet
 * Date: 	27.02.2016
 */

/*-- exports --------------------------------------------------------------- */
	.global interrupt_init_low_level		
	.global	interrupt_enable
	.global interrupt_disable

/*-- local symbols --------------------------------------------------------- */

#define INT_UNDEF	0	// undefined instruction
#define INT_SWI		1 	// software interrupt
#define INT_PREFETCH	2 	// prefetch abort (instruction prefetch)
#define INT_DATA	3 	// data abort (data access)
#define INT_IRQ		4	// hardware interrupt request
#define INT_FIQ		5	// hardware fast interrupt request

#define AM335X_OCMC_SRAM_BASE_ADDR	(0x40300000)
#define AM335X_VECTOR_BASE_ADDR		(AM335X_OCMC_SRAM_BASE_ADDR)
#define FIQ_STACK_TOP			(AM335X_OCMC_SRAM_BASE_ADDR + 0x9000)
#define IRQ_STACK_TOP			(AM335X_OCMC_SRAM_BASE_ADDR + 0x7000)
#define ABORT_STACK_TOP			(AM335X_OCMC_SRAM_BASE_ADDR + 0x5000)
#define UNDEF_STACK_TOP			(AM335X_OCMC_SRAM_BASE_ADDR + 0x3000)

/*-- local methods  -------------------------------------------------------- */
	.text

/**
 * macro implementing low level isr entry operations
 */
.macro lowlevel_handler offset, vector
	nop	
	sub	lr, #\offset		// adjust return address
	stmfd   sp!, {r0-r12,lr}	// save context and return address
	mov	r1, lr			// indicate return address
	mov	r0, #\vector		// indicate vector number
	ldr	r2, interrupt_handler	// process interrupt
	blx	r2
	ldmfd   sp!,{r0-r12,pc}^	// restore the context (pc & cpsr)
.endm

/**
 * The methods have to be attached to the interrupt vectors 
 */
vector_table_start:
	b	reset_handler
	b	undef_handler
	b	svc_handler
	b	prefetch_handler
	b	data_handler
	b	reserved_handler
	b	irq_handler
	b	fiq_handler

reset_handler:		b reset_handler
undef_handler:		lowlevel_handler 0, INT_UNDEF
svc_handler:		lowlevel_handler 0, INT_SWI
prefetch_handler:	lowlevel_handler 4, INT_PREFETCH
data_handler:		lowlevel_handler 4, INT_DATA
reserved_handler:	b reserved_handler
irq_handler:		lowlevel_handler 4, INT_IRQ
fiq_handler:		lowlevel_handler 4, INT_FIQ

interrupt_handler:	.long 0
vector_table_end:


/*-- public methods -------------------------------------------------------- */

		.align 8
interrupt_init_low_level:
	nop

	// initialize all stack pointers
	mrs	r1, cpsr		// save mode
	msr     cpsr_c, #0xd1		// switch to fiq mode
	ldr	sp, =FIQ_STACK_TOP	
	msr     cpsr_c, #0xd2		// switch to irq mode
	ldr	sp, =IRQ_STACK_TOP	
	msr     cpsr_c, #0xd7		// switch to abort mode
	ldr	sp, =ABORT_STACK_TOP	
	msr     cpsr_c, #0xdb		// switch to undef mode
	ldr	sp, =UNDEF_STACK_TOP
	msr     cpsr_c, r1 		// restore mode

	// cp15 set vector base address 
	ldr	r1, =AM335X_VECTOR_BASE_ADDR
	mcr	p15, #0, r1, c12, c0, #0
	
	// install interrupt handling routing into vector table
	ldr	r1, =interrupt_handler
	str	r0, [r1]	

	// copy low level interrupt handling code into vector table
	ldr	r1, =vector_table_start
	ldr	r2, =AM335X_VECTOR_BASE_ADDR
1:	ldr	r0, [r1], #4
	str	r0, [r2], #4
	ldr	r0, =vector_table_end
	cmp	r0, r1
	bne	1b

	bx	lr

/* ------------------------------------------------------------------------- */

interrupt_enable:
	nop
	mrs	r0, cpsr
	bic	r0, #0xc0
	msr	cpsr_c, r0
	bx	lr

/* ------------------------------------------------------------------------- */

interrupt_disable:
	nop
	mrs	r0, cpsr
	orr	r1, r0, #0xc0
	msr	cpsr_c, r1
	bx	lr

