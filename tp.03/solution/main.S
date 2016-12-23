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
 * Author: 	Daniel Gachet
 * Date: 	10.10.2016
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

// sleep times
#define TIME_ON		0x30000
#define TIME_OFF	0x00000


/*-- implementation of local methods ---------------------------------------- */

	.text
	.align 8
/**
 * method to configure a gpio pin as output
 * @param r0 gpio base address
 * @param r1 pin number
 * @param r2 default output value
 * @param r3 pad offset
 */
config_gpio_out_pin:
	nop

	// set default output value
	cmp	r2, #1
	streq	r1, [r0, #SET]
	strne	r1, [r0, #CLEAR]

	// configure GPIO pin as output
	ldr	r2, [r0, #OE]
	bic	r2, r1
	str	r2, [r0, #OE]

	// configure PAD-Multiplexer as gpio
	ldr	r2, =PADMUX
	mov	r1, #0x4f
	str	r1, [r2, r3]

	bx	lr

/**
 * method to sleep for a given time
 * @param time time to sleep
 */
sleep:
	nop
1:	subs	r0, #1
	bpl	1b
	bx	lr

/**
 * method to turn on/off a set of LED
 * @param r0 set of LED
 * @param r1 state (1 --> on, 0 --> 0ff)
 */
turn_leds:
	nop
	cmp	r1, #0
	ldr	r1, =GPIO0
	streq	r0, [r1, #CLEAR]
	strne	r0, [r1, #SET]
	bx	lr

/**
 * method to switch LED on and off
 * @param r0 set of LED
 */
switch_leds_on_off:
	nop
	push	{r4, lr}
	mov	r4, r0

	mov	r1, #1
	bl	turn_leds
	ldr	r0, =TIME_ON
	bl	sleep

	mov	r0, r4
	mov	r1, #0
	bl	turn_leds
	ldr	r0, =TIME_OFF
	bl	sleep

	pop	{r4, pc}


/*-- implementation of public methods --------------------------------------- */

main:	nop
	// print banner...
	ldr	r0, =banner
	bl 	printf

	// initialize the gpio module #0 and #2
	mov	r0, #0
	bl	am335x_gpio_init
	mov	r0, #2
	bl	am335x_gpio_init

	/* configure gpio pins as output
	 *    const struct gpio_config* config = &gpio_config[0];
	 *    while(config->pin != 0) {
	 * 	struct gpio_config cfg = *config++;
	 *	config_gpio_out_pin (cfg.gpio, cfg.pin, cfg.is_on, cfg.ofs);
	 *    }
	 */
	ldr	r5,=gpio_config
1:	ldmia	r5!,{r0-r3}
	cmp	r0, #0
	blne	config_gpio_out_pin
	bne	1b

	/* serpentine main loop
	 *    while(true) {
	 *	const uint32_t* set = &serpentine[0];
 	 * 	while(true) {
	 *	  long leds = *set++;
	 *	  if (leds == 0) break;
	 *	  switch_leds_on_off(leds);
	 *	}
	 *    }
	 */
1:	ldr	r4, =serpentine
2:	ldr	r0, [r4], #4
	cmp	r0, #0
	beq	1b
	bl	switch_leds_on_off
	b	2b

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

// GPIO config table (null terminated)
	.section .rodata
	.align 	4
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

// serpentine lookup table (null terminated)
	.section .rodata
	.align 	4
serpentine:	
	.long SEGA
	.long SEGB
	.long SEGC
	.long SEGD 
	.long SEGE
	.long SEGF 

	.long SEGA
	.long SEGB
	.long SEGG 
	.long SEGE 
	.long SEGD 
	.long SEGC
	.long SEGG
	.long SEGF 

	.long SEGE
	.long SEGD
	.long SEGC
	.long SEGB

	.long 0 

