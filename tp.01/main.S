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
 * Author: 	Charlotte JUNOD, Nicolas FUCHS
 * Date: 	23.9.16
 */


//-- Prof version
/*
// Export public symbols
	.global main, res, incr, i			//déclaration des variables


// Constants declaration
#define LOOPS 8							//définition d'une variable macro


// Initialized variables declation
		.data
		.align	8						//option pour les registres
res:	.long 16						//affectation de la variable res: un long de valeur 16
incr: 	.short 32						//affectation de la variable incr: un short de valeur 32


// Uninitialized variables declation
	.bss
	.align 8							//option pour les registres
i:	.space 4							//affectation de la variable i: pas de type défini, mais on réserve un espace de 4 octets

//Assembler functions implementation
	.text
main:	nop								//pas d'opérations dans le main

	mov 	r0,#LOOPS					//affectation de la valeur de LOOPS(=8) dans r0
	ldr 	r1,=incr					//affectation de la valeur de l'adresse de incr dans r1
	ldrh	r1, [r1]					//affectation de la valeur trouvée à l'adresse indiquée par r1 dans r1
	ldr		r3,=res						//affectation de la valeur de l'adresse de res dans r3
	ldr		r4,=i						//affectation de la valeur de l'adresse de i dans r4
	mov		r5,#0						//affectation de la valeur 0 dans r0
	str		r5,[r4]						//affectation de la valeur tde r5 à l'adresse indiquée par r4
next:ldr	r2,[r3]						//affectation de la valeur de l'adresse indiquée par r3 dans r2
	add		r2,r1						//r2=r2+r1
	str		r2,[r3]						//affectation de la valeur tde r2 à l'adresse indiquée par r3
	ldr		r5,[r4]						//affectation de la valeur trouvée à l'adresse indiquée par r4 dans r5
	add		r5,#1						//r5=r5+1
	str		r5,[r4]						//affectation de la valeur tde r5 à l'adresse indiquée par r4
	cmp		r5, r0						//comparaison entre r5 et r0
	bne		next						//si la comparaison est fausse, retourner à la ligne next
1:	nop									//pas d'opération
	b		1b
*/



//	-- Our version --
// Export public symbols
	.global main, res, incr


// Constants declaration
#define LOOPS 8


// Initialized variables declation
		.data
		.align	8
res:	.long 16
incr: 	.short 32


// Uninitialized variables declation
	.bss
	.align 8

//Assembler functions implementation
	.text
main:	nop

		mov 	r0,#LOOPS
		ldr 	r1,=incr
		ldrh	r1, [r1]
		ldr		r2,=res
		ldr		r3,[r2]
		mov		r4,#0
next:	add 	r3,r1
		add 	r4,#1
		cmp		r4,r0
		bne		next
		str r3,[r2]
1:		nop
		b		1b


// -- Code équivalent en langage évolué (Java)
/*
int loops=8;
long res=16;
short incr=32;

for(int i=0;i<=loops;i++){
	res+=incr;
}
*/


// Constant variables declation (rom data)	
	.section .rodata

// String definition used for message outputs (printf)
banner:	.ascii "\n"
	.ascii "HEIA-FR - Embedded Systems 1 Laboratory\n"
	.ascii "An introduction to the development environment\n"
	.ascii "--> Simple ARM assembler program to experiment the Embedded System\n"
	.ascii "    Laboratory development environment under Git and Eclipse.\n"
	.asciz "\n"*/

