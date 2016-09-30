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
	.global main, res, incr, i


/* Constants declaration */
#define LOOPS 8


/* Initialized variables declation */
		.data
		.align	8
res :	.long	16
incr :	.short	32


/* Uninitialized variables declation */
	.bss
	.align	8 // oblige l'assembleur à compléter avec des octets (inutilisés) pour retrouver une adresse multiple de 4
i:	.space 	4 // 4  = long


/* Implementation of assembler functions and methods */
		.text
main:	nop
		mov		r0, #LOOPS  // copie les données de #LOOPS vers r0
		ldr		r1, =incr   // stocke l'adresse de la donnée sur le registre
		ldrh	r1, [r1]	// stocke les 16 premiers bits de la valeur de r1 vers r1
		ldr		r3, =res	// stocke le long res sur le registre r3
		ldr		r4, =i		// stocke
		mov		r5, #0
		str		r5, [r4]
next :  ldr		r2, [r3]
		add		r2, r1 // ajoute r1 à r2
		str		r2, [r3]
		ldr		r5, [r4]
		add		r5, #1
		str		r5, [r4]
		cmp		r5, r0
		bne		next
		// print banner...
		// ldr	r0, =banner
		// bl 	printf

	/* start to implement your code here */
1:	nop
	b	1b

	// bx	lr


// Constant variables declation (rom data)	
	.section .rodata

// String definition used for message outputs (printf)
banner:	.ascii "\n"
	.ascii "HEIA-FR - Embedded Systems 1 Laboratory\n"
	.ascii "An introduction to the development environment\n"
	.ascii "--> Simple ARM assembler program to experiment the Embedded System\n"
	.ascii "    Laboratory development environment under Git and Eclipse.\n"
	.asciz "\n"

