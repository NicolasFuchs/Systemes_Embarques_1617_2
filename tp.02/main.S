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
 * Author: 	<author's>
 * Date: 	<date>
 */

// Export public symbols
	.global main

// Declaration of the constants
#define NUMBERS 10

// Implementation of assembler functions and methods
	.text
	.align	8

main:	nop
	// print banner...
	ldr	r0, =banner
	bl 	printf

	// start to implement your code here

	ldr	r4, =0
1:	ldr	r0, =format
	mov	r1, r4
	bl	printf
	add	r4, #1
	cmp	r4, #NUMBERS
	blo	1b

	// return 0
	ldr	r0, =0
	bx	lr


// Constant variables declation (rom data)
	.section .rodata

// String definition used for message outputs (printf)
banner:	.ascii "\n"
	.ascii "HEIA-FR - Embedded Systems 1 Laboratory\n"
	.ascii "An introduction the ARM's assembler language\n"
	.ascii "--> Program to generate and print all the primer numbers up to MAX\n"
	.asciz "\n"

format:	.asciz "%d\n"

