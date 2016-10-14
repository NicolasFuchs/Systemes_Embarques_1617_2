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
 * Author: 	Jonathan Rial & Alan Sueur
 * Date: 	14 october 2016
 */

// Export public symbols
	.global main

// Declaration of the constants
#define MAX 100

// Initialized variables declation
		.data
		.align	8
true:	.byte	0xFF
false:	.byte	0x00

// Uninitialized variables declation
		.bss
		.align
array:	.space 	MAX

// Implementation of assembler functions and methods
	.text
	.align	8

main:	nop

// 1st mark all numbers as prime number
		mov		r4, #0
		ldr		r5,	=true
		ldrb	r5,	[r5]
		ldr		r6,	=array
loop1:	cmp 	r4,	#MAX
		bhs		out1
		strb	r5,	[r6,r4]
		add		r4,	#1
		b		loop1
out1:	nop

// 2nd mark all multiples as not a prime number
		mov		r4, #2
		mov		r7, #2
		ldr		r5, =false
		ldrb	r5, [r5]
loop2:	cmp 	r4,	#MAX
		bhs		out2
		mul		r8, r4, r7
loop3:	cmp		r8, #MAX
		bhs		out3
		strb	r5,	[r6,r8]
		add		r8,	r4
		b		loop3
out3:	add		r4,	#1
		b		loop2
out2:	nop

// 3rd print all prime numbers
		mov		r4, #2
		ldr		r5,	=true
		ldrb	r5,	[r5]
loop4:	cmp		r4,	#MAX
		bhs		finish
		ldrb	r9, [r6,r4]
		cmp		r9, r5
		beq		prt
next:	add		r4,	#1
		b		loop4

prt:	mov		r1, r4
		ldr		r0, =format
		bl		printf
		b		next

finish:	nop
		b		finish

// Constant variables declation (rom data)
	.section .rodata

// String definition used for message outputs (printf)
format:	.asciz "%d\n"
