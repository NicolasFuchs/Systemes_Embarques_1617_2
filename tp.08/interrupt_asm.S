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
 * Project:	HEIA-FR / Embedded Systems 1 Laboratory
 *
 * Abstract: 	An introduction the ARM's assembler language
 *
 * Purpose:	Program to generate and print all the primer numbers up to MAX.
 *
 * Author: 	<Nicolas Fuchs et Alan Sueur>
 * Date: 	<13.03.17>
 */

#define INT_UNDEF       0
#define INT_SVC         1
#define INT_PREFETCH    2
#define INT_DATA        3
#define INT_IRQ         4
#define INT_FIQ         5

#define FIQ_STACK_TOP       fiq_stack_top
#define IRQ_STACK_TOP       irq_stack_top
#define ABORD_STACK_TOP     abord_stack_top
#define UNDEF_STACK_TOP     undef_stack_top

#define VECTOR_BASE_ADDR vector_table

.macro flih_1st_stage offset, vector
    nop
    sub     lr, #\offset
    stmfd   sp!,{r0-r12,lr}

    sub     r1, lr, #4 // possibilité
    mov     r0, #\vector
    ldr     r2, flih
    blx     r2

    ldmfd   sp!,{r0-r12,pc}^ // circonflexe voulu
.endm


    .bss
    .align 8
fiq_stack:           .space 0x2000
fiq_stack_top:
irq_stack:           .space 0x2000
irq_stack_top:
abord_stack:         .space 0x2000
abord_stack_top:
undef_stack:         .space 0x2000
undef_stack_top:


    .text
    .align 8
vector_table:
1:  b   1b              // reset
    b   undef_handler
    b   svc_handler
    b   prefetch_handler
    b   data_handler
1:  b   1b              // reserved
    b   irq_handler
    b   fiq_handler


undef_handler:      flih_1st_stage 0, INT_UNDEF
svc_handler:        flih_1st_stage 0, INT_SVC
prefetch_handler:   flih_1st_stage 4, INT_PREFETCH
data_handler:       flih_1st_stage 4, INT_DATA
irq_handler:        flih_1st_stage 4, INT_IRQ
fiq_handler:        flih_1st_stage 4, INT_FIQ


flih:   .long 0


    .global interrupt_init_1st_stage // utilisable depuis l'extérieur
interrupt_init_1st_stage:
    nop
    ldr r1, =flih
    str r0, [r1]

    ldr r1, =VECTOR_BASE_ADDR
    mcr p15, #0, r1, c12, c0, #0

    mrs r1, cpsr
    msr cpsr_c, #0xd1 // fiq
    ldr sp, =FIQ_STACK_TOP

    msr cpsr_c, #0xd2 // irq
    ldr sp, =IRQ_STACK_TOP

    msr cpsr_c, #0xd7 // abord
    ldr sp, =ABORD_STACK_TOP

    msr cpsr_c, #0xdb // undef
    ldr sp, =UNDEF_STACK_TOP

    msr cpsr, r1

    bx lr


    .global interrupt_enable
interrupt_enable:
    nop
    bx lr


    .global interrupt_disable
interrupt_disable:
    nop
    bx lr
