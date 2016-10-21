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
 * Abstract:	Introduction to the development environment
 *
 * Purpose:	Simple ARM assembler program to experiment the Embedded System
 *		Laboratory development environment under Git and Eclipse.
 *
 * Author: 	Daniel Gachet
 * Date: 	19.09.2016
 */

// Export public symbols
	.global main, i, res, incr

// Declaration of the constants
#define LOOPS	8

// Declation of initialized variables
	.data
	.align 	8
res:	.long	16
incr:	.short 	32

// Declation of uninitialized variables
	.bss
	.align 	8
i:	.space 	4

// Implementation of assembler functions and methods
	.text
	.align	8
main:	nop
	// print banner...
	ldr	r0, =banner
	bl 	printf

	// start to implement your code here

	mov	r0, #LOOPS
	ldr	r1, =incr
	ldrh	r1, [r1]
	ldr	r3, =res
	ldr	r4, =i
	mov	r5, #0
	str	r5, [r4]
next:	ldr	r2, [r3]
	add	r2, r1
	str	r2, [r3]
	ldr	r5, [r4]
	add	r5, #1
	str	r5, [r4]
	cmp	r5, r0
	bne	next

	bl	opt1
	bl	opt2

1:	nop
	b	1b

	bx 	lr

// Constant variables declation (rom data)	
	.section .rodata

// String definition used for message outputs (printf)
banner:	.ascii "\n"
	.ascii "HEIA-FR - Embedded Systems 1 Laboratory\n"
	.ascii "An introduction to the development environment\n"
	.ascii "--> Simple ARM assembler program to experiment the Embedded System\n"
	.ascii "    Laboratory development environment under Git and Eclipse.\n"
	.asciz "\n"

	/* algorithm

	#define LOOPS 8
	long res = 16;
	short incr = 32;
	for (int i=0; i<LOOPS; i++) 
		res+=incr;

	*/
	
	.text
	.align 8

	// 1st optinization level w/o modifying the algorithm
opt1:	mov	r0, #LOOPS	// load number of loops to be done
	ldr	r1, =incr	// load incr value into r1
	ldrh	r1, [r1]
	ldr	r3, =res	// load initial value of res into r2
	ldr	r2, [r3]
1:	
	add	r2, r1		// r2 += r1 --> res += incr
	subs	r0, #1
	bne	1b
	str	r2, [r3]	// store result into res

	bx	lr


	// 2st optinization level w/ modification of the algorithm
opt2:	mov	r0, #LOOPS	// load number of loops to be done
	ldr	r1, =incr	// load incr value into r1
	ldrh	r1, [r1]
	ldr	r3, =res	// load initial value of res into r2
	ldr	r2, [r3]
	mla	r2, r1, r0, r2	// res += (incr * LOOPS);
	str	r2, [r3]	// store result

	bx 	lr

