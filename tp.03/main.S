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
 * Author: 	Jonathan Rial, Alan Sueur
 * Date: 	5 november 2016
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
#define DIG1	(1<<PIN_DIG1)
#define SEGA	(1<<PIN_SEGA)
#define SEGB	(1<<PIN_SEGB)
#define SEGC	(1<<PIN_SEGC)
#define SEGD	(1<<PIN_SEGD)
#define SEGE	(1<<PIN_SEGE)
#define SEGF	(1<<PIN_SEGF)
#define SEGG	(1<<PIN_SEGG)

// Timeout for sleep
#define TIMEOUT	0x8ffff


/*-- implementation of local methods ---------------------------------------- */
	.text
	.align 8
	// Configuration des ports GPIO
config_gpio:	nop
	/* config default pin state
	if(state==on)
		gpio.set = seg;
	else
		gpio.clear = seg;
	*/
	cmp 	r2, #1
	streq	r1, [r0,#SET]
	strne	r1, [r0,#CLEAR]

	/* config pad as output
	gpio.oe &= ~seg;
	*/
	ldr r4, [r0,#OE]
	bic	r4, r1
	str r4, [r0,#OE]

	/* config pad mux
	padmux[ofs] = 0x4f;
	*/
	ldr r4, =0x4f
	ldr r1, =PADMUX
	str r4,	[r1,r3]

	bx	lr

	// Sleep
sleep:	nop
1:		subs	r5, #1
		bpl		1b
		bx		lr

	// Turn on/off a seg
turn_seg:	nop
			cmp		r1, #0
			ldr		r1, =GPIO0
			streq	r0,	[r1,#CLEAR]
			strne	r0, [r1,#SET]
			bx		lr

	// Turn on a seg
turn_on:	nop
			mov		r10, r14 // save pointer for return
			mov		r1, #1
			bl		turn_seg
			bx		r10

	// Turn off a seg
turn_off:	nop
			mov		r10, r14 // save pointer for return
			mov		r1, #0
			bl		turn_seg
			bx		r10

/*-- implementation of public methods --------------------------------------- */
	.text
	.align 8
main:	nop
	// initialize the gpio module #0 and #2
	mov	r0, #0
	bl	am335x_gpio_init
	mov	r0, #2
	bl	am335x_gpio_init

	/*
	for all segs + dig
		config_gpio(gpio, seg, state, ofs)
	*/
	ldr		r8, =gpio_config
1:	ldmia	r8!, {r0-r3}
	cmp		r0, #0
	beq		2f
	bl		config_gpio
	b		1b
2:	nop

	/* Draw the snake (infinite loop)

	long[] snake = {SEGA, ..., 0};
	while(true) {
	 	int i = 0;
	 	while(true) {
	 		i++;
	 		if(snake[i] == 0) break;
	 		turnOn(snake[i]);
	 		sleep();
	 		turnOff(snake[i]);
	 	}
	 }
	*/
1:	ldr		r4,	=snake
2:	ldr		r0, [r4], #4
	cmp		r0, #0
	beq		1b
	bl		turn_on
	ldr		r5, =TIMEOUT
	bl		sleep
	bl		turn_off
	b		2b

// GPIO config table
	.section .rodata
	.align 4
gpio_config:
	.long GPIO0, SEGA, 0, PAD_OFS_SEGA
	.long GPIO0, SEGB, 0, PAD_OFS_SEGB
	.long GPIO0, SEGC, 0, PAD_OFS_SEGC
	.long GPIO0, SEGD, 0, PAD_OFS_SEGD
	.long GPIO0, SEGE, 0, PAD_OFS_SEGE
	.long GPIO0, SEGF, 0, PAD_OFS_SEGF
	.long GPIO0, SEGG, 0, PAD_OFS_SEGG
	.long GPIO2, DIG1, 1, PAD_OFS_DIG1
	.long 0, 0, 0, 0

// LEDs sequence to draw the snake
	.section .rodata
	.align 4
snake:
	.long SEGA
	.long SEGB
	.long SEGG
	.long SEGE
	.long SEGD
	.long SEGC
	.long SEGG
	.long SEGF
	.long 0
