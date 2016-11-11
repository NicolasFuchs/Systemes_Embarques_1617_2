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
 * Abstract:	Introduction the ARM's assembler language
 *
 * Purpose:	Advanced ARM assembler program implementing a simple serpentine.
 *		This program uses the GPIO0&2 I/O modules to get access to the 
 *		7-segment display of the Beaglebone Black HEIA-FR extension 
 *		board.
 *
 * Author: 	Charlotte Junod and Nicolas Fuchs
 * Date: 	21.10.16
 */

/* Export public symbols */
	.global main,init_mux,gpio_config,gpio_turn,sleep_val

/*-- constants declaration -------------------------------------------------- */

// PAD Multiplexer base address for gpio_0 & gpio_2
#define PADMUX		0x44e10000

// 7-segment gpio offset in pad mux
						//toutes les led
#define PAD_OFS_DIG1	0x890
#define PAD_OFS_SEGA	0x958
#define PAD_OFS_SEGB	0x95c
#define PAD_OFS_SEGC	0x980
#define PAD_OFS_SEGD	0x820
#define PAD_OFS_SEGE	0x824
#define PAD_OFS_SEGF	0x828
#define PAD_OFS_SEGG	0x82c

// gpio0 & gpio2 base address
#define GPIO0		0x44e07000
#define GPIO2		0x481ac000

// gpio register offset
#define OE		0x0134
#define CLEAR	0x0190
#define SET		0x0194

// 7-segment gpio pin
#define PIN_DIG1	2
#define PIN_SEGA	4
#define PIN_SEGB	5
#define PIN_SEGC	14
#define PIN_SEGD	22
#define PIN_SEGE	23
#define PIN_SEGF	26
#define PIN_SEGG	27

// 7-segment bitset
#define DIG1		(1<<PIN_DIG1)
#define SEGA		(1<<PIN_SEGA)
#define SEGB		(1<<PIN_SEGB)
#define SEGC		(1<<PIN_SEGC)
#define SEGD		(1<<PIN_SEGD)
#define SEGE		(1<<PIN_SEGE)
#define SEGF		(1<<PIN_SEGF)
#define SEGG		(1<<PIN_SEGG)

	.section .rodata
	.align 4
// variables declaration
init_mux:	.long 0x4f
sleep_val:	.long 1000000
gpio_config:
	.long GPIO2,DIG1,PAD_OFS_DIG1,1
	.long GPIO0,SEGA,PAD_OFS_SEGA,0
	.long GPIO0,SEGB,PAD_OFS_SEGB,0
	.long GPIO0,SEGC,PAD_OFS_SEGC,0
	.long GPIO0,SEGD,PAD_OFS_SEGD,0
	.long GPIO0,SEGE,PAD_OFS_SEGE,0
	.long GPIO0,SEGF,PAD_OFS_SEGF,0
	.long GPIO0,SEGG,PAD_OFS_SEGG,0
	.long 0,0,0,0

gpio_turn:
	.long GPIO0,SEGA
	.long GPIO0,SEGB
	.long GPIO0,SEGG
	.long GPIO0,SEGE
	.long GPIO0,SEGD
	.long GPIO0,SEGC
	.long GPIO0,SEGG
	.long GPIO0,SEGF
	.long 0,0

/*-- implementation of local methods ---------------------------------------- */
	.text
	.align 8

/*-- implementation of public methods --------------------------------------- */
	.text
	.align 8

main:	nop
	// print banner...
	//ldr	r0, =banner
	//bl 	printf

	// initialize the gpio module #0 and #2
	mov	r0, #0
	bl	am335x_gpio_init
	mov	r0, #2
	bl	am335x_gpio_init


//---------------------- Our code: ----------------------------------------------------
	//déroulement:

	//initialisation
	ldr r8,=gpio_config
init_loop:
	ldmia r8!,{r0-r3}
	cmp r1,#0
	beq exit_init_loop
	bl init
	b  init_loop
exit_init_loop: nop

serpentin:

	ldr r8,=gpio_turn
turn_loop:
	ldmia r8!,{r0-r1}
	cmp r1,#0
	beq exit_turn_loop
	bl trigger
	b turn_loop
exit_turn_loop:
	b serpentin

init:
	//Routine d'initialisation (init)
	/*
	* r0 = gpio
	* r1 = 1<<pin
	* r2 = padmux_offset
	* r3 = state
	*/
	push {lr}
	//init mux
	ldr r4,=PADMUX
	ldr r5,=0x4f
	str r5,[r4,r2]

	//init oe
	ldr r4,[r0,#OE]
	bic r4,r1
	str r4,[r0,#OE]

	cmp r3,#0
	strne r1,[r0,#SET]
	streq r1,[r0,#CLEAR]

	pop {pc}

trigger:
	//Routine d'activation des led (trigger)
	/*
	* r0 = gpio
	* r1 = 1<<pin
	*/
	push {lr}

	str r1,[r0,#SET]
	ldr r3,=sleep_val
	ldr r3,[r3]
	bl sleep
	str r1,[r0,#CLEAR]

	pop {pc}

sleep:
	//Routine pour attendre
	/*
	* r3=temps à attendre
	*/
	nop
1:	sub r3,#1
 	cmp r3,#0
	bne 1b
	bx lr

//---------------------- End of our code-----------------------------------------------

// String definition used for message outputs (printf)
	.section .rodata
banner:	.ascii "\n"
	.ascii "HEIA-FR - Embedded Systems 1 Laboratory\n"
	.ascii "An introduction the ARM's assembler language\n"
	.ascii "--> a serpentine on Beaglebone Black HEIA-FR extension board\n"
	.asciz "\n"

