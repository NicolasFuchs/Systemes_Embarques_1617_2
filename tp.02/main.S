/*
 * Main_Junod.S
 *
 *  Created on: Oct 10, 2016
 *      Author: lmi
 */
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
 * Project:	HEIA-FR / Embedded Systems 1 Laboratory
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
#define MAX 100
#define true 1
#define false 0

	//déclaration du tableau v de booléens
	.bss
		v: .space MAX	//v est un tableau d'octets de taille MAX(=100) dont chaque case contiendra un booléen

// Implementation of assembler functions and methods
	.text
	.align	8

main:	nop
	// print banner...
	/*
	ldr	r0, =banner
	bl 	printf
*/

//------- start to implement your code here---------


	//initialisation du tableau -> toutes les cases à true
	mov r0,#true				//r0=true
	ldr r1, =v					//stocker l'adresse de départ de v dans r1
	mov r2,#MAX-1				//r2=99
1:	strB r0,[r1,r2]				//v[r2]=true
	sub r2,#1					//r2--
	cmp r2,#0					//si r2!=0
	bne 1b						//retourner à la ligne 1 (=while(r2>0))


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


	//print
	ldr r4,=v					//stocker l'adresse de départ de v dans r4
	mov r5,#0					//r5=0
4:	ldrb r6,[r4,r5]				//r6=v[r5]
	cmp r6,#false				//si r6=false
	bne 5						//aller à la ligne 2
	mov r1,r5					//r1=r5
	ldr r0,=format				//print "%d",r1   ---/!\ ne pas utiliser r0,r1,r2,r3 pour autre chose
	bl printf
	add r5,#1					//r5++
5:	cmp r5,#MAX					//si r5<MAX
	blo 4b						//retourner à la ligne 1

//------------deuxième boucle - version optimisée-------------------
/*
	//rendre tous les multiples non premiers
	mov r0,#false				//stocker la valeur de false dans r0
	ldr r1, =v					//stocker l'adresse de v dans r1 (est-ce nécessaire de le refaire?)
	mov r2, #0					//r2=0
1:	mov r3,[r1,r2]				//r3=v[r2]
	cmp r3,false				//si r3=false
	beq 3b						//aller à la ligne 3
	ldrb r3,[r2]				//r3=r2
2:	add r3,r2					//r3+=r2
	strB r0,[r1,r3]				//v[r3]=r0(=false)
	cmp r3,#MAX					//si r3<=MAX
	bls 2b						//retourner à la ligne 2 (=while(r3<=MAX))
3:	add r2,#1					//r2++
	cmp r2,#MAX					//si r2<=100
	bls 1b						//retourner à la ligne 1(=while(r2<=100))
*/
//-------------deuxième boucle - version java (pour la compréhension)---------------------------------------------------
/*
	for(int i=0;i<MAX;i++){
		if(v[i]==true){
			for(int j=i*2;j<MAX;j+=i){
				v[j]=false;
			}
		}
	}
*/
//---------end of our code--------------------------

// Constant variables declation (rom data)
	.section .rodata

// String definition used for message outputs (printf)
banner:	.ascii "\n"
	.ascii "HEIA-FR - Embedded Systems 1 Laboratory\n"
	.ascii "An introduction the ARM's assembler language\n"
	.ascii "--> Program to generate and print all the primer numbers up to MAX\n"
	.asciz "\n"

format:	.asciz "%d\n"
