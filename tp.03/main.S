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
 * Author: 	<author's>
 * Date: 	<date>
 */

/* Export public symbols */
	.global main

/*-- constants declaration -------------------------------------------------- */

// PAD Multiplexer base address for gpio_0 & gpio_2
#define PADMUX		0x44e10000

// 7-segment gpio offset in pad mux
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
#define CLEAR		0x0190
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


/*-- implementation of local methods ---------------------------------------- */
	.text
	.align 8

/*-- implementation of public methods --------------------------------------- */
	.text
	.align 8

main:	nop
	// print banner...
	ldr	r0, =banner
	bl 	printf

	// initialize the gpio module #0 and #2
	mov	r0, #0
	bl	am335x_gpio_init
	mov	r0, #2
	bl	am335x_gpio_init


	// while(true);
1:	nop
	b	1b

// String definition used for message outputs (printf)
	.section .rodata
banner:	.ascii "\n"
	.ascii "HEIA-FR - Embedded Systems 1 Laboratory\n"
	.ascii "An introduction the ARM's assembler language\n"
	.ascii "--> a serpentine on Beaglebone Black HEIA-FR extension board\n"
	.asciz "\n"

