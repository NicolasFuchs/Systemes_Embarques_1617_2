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
#define MAX 100
#define true 1
#define false 0

.bss
v: .space MAX

// Implementation of assembler functions and methods
	.text
	.align	8

main:	nop
	// print banner...
	ldr	r0, =banner
	bl 	printf

//------- start to implement your code here---------

i:	mov r0, #2							// initialisation de i : i = 2
	b t1

1:  mov r1, r0, lsl #1					// initialisation de j : j = i*2
	b t2

2:  ldr r2, =v							// initialisation des cases du tableau v à false
	mov r3, #false
	strB r3,[r2,r1]

	add r1, r1, r0						// incrémentation de j : j += i

t2: cmp r1, #MAX						// test de la 2ème boucle : j < MAX
		blo 2b

	add r0, #1							// incrémentation de i : i++

t1: cmp r0, #MAX						// test de la 1ère boucle : i < MAX
	blo 1b


// Constant variables declation (rom data)
	.section .rodata

// String definition used for message outputs (printf)
banner:	.ascii "\n"
	.ascii "HEIA-FR - Embedded Systems 1 Laboratory\n"
	.ascii "An introduction the ARM's assembler language\n"
	.ascii "--> Program to generate and print all the primer numbers up to MAX\n"
	.asciz "\n"

format:	.asciz "%d\n"

