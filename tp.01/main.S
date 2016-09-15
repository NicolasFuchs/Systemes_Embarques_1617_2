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
 * Author: 	<author(s)>
 * Date: 	<date>
 */

/* Export public symbols */
	.global main, v, i


/* Constants declaration */
#define CONSTANT 10


/* Initialized variables declation */
	.data
	.align	8
v:	.short	CONSTANT


/* Uninitialized variables declation */
	.bss
	.align	8
i:	.space 	4


/* Implementation of assembler functions and methods */
	.text
main:	nop
	// print banner...
	ldr	r0, =banner
	bl 	printf

	/* start to implement your code here */


1:	nop
	b	1b

	bx	lr


// Constant variables declation (rom data)	
	.section .rodata

// String definition used for message outputs (printf)
banner:	.ascii "\n"
	.ascii "HEIA-FR - Embedded Systems 1 Laboratory\n"
	.ascii "An introduction to the development environment\n"
	.ascii "--> Simple ARM assembler program to experiment the Embedded System\n"
	.ascii "    Laboratory development environment under Git and Eclipse.\n"
	.asciz "\n"

