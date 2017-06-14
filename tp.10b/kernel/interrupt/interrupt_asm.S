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
 * Abstract: 	ARM Interrupt Handling - First-Level Interface
 *
 * Author: 	Daniel Gachet
 * Date: 	22.05.2017
 */

/*-- local symbols --------------------------------------------------------- */

#define INT_UNDEF	0	// undefined instruction
#define INT_SWI		1 	// software interrupt
#define INT_PREFETCH	2 	// prefetch abort (instruction prefetch)
#define INT_DATA	3 	// data abort (data access)
#define INT_IRQ		4	// hardware interrupt request
#define INT_FIQ		5	// hardware fast interrupt request

#ifdef FAST_HANDLER
#define AM335X_OCMC_SRAM_BASE_ADDR	(0x40300000)
#define VECTOR_BASE_ADDR 		(AM335X_OCMC_SRAM_BASE_ADDR)
#define STACK_BASE			(AM335X_OCMC_SRAM_BASE_ADDR + 0x1000)

#define IRQ_STACK_TOP			(STACK_BASE + 0x8000)
#define FIQ_STACK_TOP			(STACK_BASE + 0x6000)
#define ABORT_STACK_TOP			(STACK_BASE + 0x4000)
#define UNDEF_STACK_TOP			(STACK_BASE + 0x2000)

#else
#define VECTOR_BASE_ADDR 		vector_table_start
#define STACK_BASE			stacks
	.bss
	.align 4
und_s:	.space	0x2000
UNDEF_STACK_TOP:

abt_s:	.space 	0x2000
ABORT_STACK_TOP:

fiq_s:	.space	0x2000
FIQ_STACK_TOP:

irq_s:	.space 	0x2000
IRQ_STACK_TOP:
#endif



/*-- local methods  -------------------------------------------------------- */
	.text

/**
 * macro implementing first-level interrupt handler (1st stage) operations
 */
.macro flih_1st_stage offset, vector
	nop	
	.if \offset != 0		// adjust return address
	sub	lr, #\offset		// only if necessary
	.endif
	stmfd   sp!, {r0-r12,lr}	// save context and return address
	mov	r1, lr			// indicate return address
	mov	r0, #\vector		// indicate vector number
	ldr	r2, flih		// process interrupt
	blx	r2
	ldmfd   sp!,{r0-r12,pc}^	// restore the context (pc & cpsr)
.endm

/**
 * The methods have to be attached to the interrupt vectors 
 */
	.align 8
vector_table_start:
1:	b	1b		// reset
	b	undef_handler
	b	svc_handler
	b	prefetch_handler
	b	data_handler
1:	b	1b		// reserved
	b	irq_handler
	b	fiq_handler

undef_handler:		flih_1st_stage 0, INT_UNDEF
//svc_handler:		flih_1st_stage 0, INT_SWI
prefetch_handler:	flih_1st_stage 4, INT_PREFETCH
data_handler:		flih_1st_stage 4, INT_DATA
irq_handler:		flih_1st_stage 4, INT_IRQ
fiq_handler:		flih_1st_stage 4, INT_FIQ

	.global svc_handler
svc_handler:
	nop

	// switch to user stack if coming from usr mode
	mrs	r12, spsr
	tst	r12, #0x0f
	msreq	cpsr_c, #0xdf
	moveq	r12, sp
	msreq	cpsr_c, #0xd3
	moveq	sp, r12

	// save current context
	stmfd	sp!,{r4,r5,lr}
	mrs	r4, spsr
	mov	r5, r12

	// process syscall 
	ldr	r12, syscall		

	cmp	r12, #0
	moveq	r0, #-1
	blxne	r12

	// restore process context
	tst	r4, #0x0f
	msreq	cpsr_c, #0xdf
	moveq	sp, r5
	msreq	cpsr_c, #0xd3

	msr	spsr, r4
	ldmfd	sp!, {r4,r5,pc}^

flih:	 .long 0	// first-level interrupt handler (2nd stage)

syscall: .long 0
vector_table_end:


/*-- public methods -------------------------------------------------------- */

		.align 8
		.global interrupt_init_1st_stage
interrupt_init_1st_stage:
	nop

	// initialize all stack pointers
	mrs		r1, cpsr		// save mode
	msr     cpsr_c, #0xd1		// switch to fiq mode
	ldr		sp, =FIQ_STACK_TOP
	msr     cpsr_c, #0xd2		// switch to irq mode
	ldr		sp, =IRQ_STACK_TOP
	msr     cpsr_c, #0xd7		// switch to abort mode
	ldr		sp, =ABORT_STACK_TOP
	msr     cpsr_c, #0xdb		// switch to undef mode
	ldr		sp, =UNDEF_STACK_TOP
	msr     cpsr_c, r1 		// restore mode

	// cp15 set vector base address 
	ldr		r1, =VECTOR_BASE_ADDR
	mcr		p15, #0, r1, c12, c0, #0
	
	// install interrupt handling routing into vector table
	ldr		r1, =flih
	str		r0, [r1]

#ifdef FAST_HANDLER
	// copy low level interrupt handling code into vector table
	ldr	r1, =vector_table_start
	ldr	r2, =VECTOR_BASE_ADDR
1:	ldr	r0, [r1], #4
	str	r0, [r2], #4
	ldr	r0, =vector_table_end
	cmp	r0, r1
	bne	1b
#endif
	bx	lr

/* ------------------------------------------------------------------------- */

	.global interrupt_attach_syscall_handler
interrupt_attach_syscall_handler:
	nop
	ldr	r1, =syscall
	ldr	r2, =vector_table_start - VECTOR_BASE_ADDR
	sub	r1, r2

	str	r0, [r1]

	bx	lr

/* ------------------------------------------------------------------------- */

		.global interrupt_enable
interrupt_enable:
	nop
	mrs	r0, cpsr
	bic	r0, #0xc0
	msr	cpsr_c, r0
	bx	lr

/* ------------------------------------------------------------------------- */

		.global interrupt_disable
interrupt_disable:
	nop
	mrs	r0, cpsr
	orr	r1, r0, #0xc0
	msr	cpsr_c, r1
	bx	lr

