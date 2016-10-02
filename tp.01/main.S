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
 * Author: 	Alan Sueur & Jonathan Rial
 * Date: 	October 2016
 */

// Export public symbols
	.global main, res, incr, i /* Déclare les symboles globaux */


// Declaration of the constants
#define LOOPS 8 /* Déclare une constant LOOPS avec la valeur */


// Initialized variables declation
		.data		/* Section pour les données initialisées */
		.align	8	/* Aligne la prochaine donnée ou instruction à une adresse multiple de 8 */
res :	.long	16	/* Crée une variable de taille long (32 bits) avec la valeur 16 */
incr :	.short	32	/* Crée une variable de taille short (16 bits) avec la valeur 32 */

// Uninitialized variables declation
	.bss		/* Section pour les données mises à zéro */
	.align	8	/* Aligne la prochaine donnée ou instruction à une adresse multiple de 8 */
i:	.space 	4	/* Crée un espace de stockage de 4 bytes avec l'étiquette i (initialisé à 0) */


// Assembler functions implementation
		.text 				/* Section pour le code */
main:	nop					/* Fait rien du tout (No Operation) */
		mov		r0, #LOOPS  /* Copie la valeur de la constante LOOPS dans r0 */
		ldr		r1, =incr   /* Récupère dans r1 l'adresse de stockage de la variable incr */
		ldrh	r1, [r1]	/* Récupère dans r1 le "half word" se trouvant à l'adresse stockée dans r1 */
		ldr		r3, =res	/* Récupère dans r3 l'adresse de stockage de la variable res */
		ldr		r4, =i		/* Récupère dans r4 l'adresse de stockage de la variable i */
		mov		r5, #0		/* Copie la valeur 0 dans r5 */
		str		r5, [r4]	/* Stocke r5 à l'adresse stockée dans r4 */
next :  ldr		r2, [r3]	/* Récupère dans r2 le "word" se trouvant à l'adresse stockée dans r3 */
		add		r2, r1		/* Additonne la valeur stockée dans r1 à la valeur stockée dans r2 */
		str		r2, [r3]	/* Stocke r2 à l'adresse stockée dans r3 */
		ldr		r5, [r4]	/* Récupère dans r5 le "word" se trouvant à l'adresse stockée dans r4 */
		add		r5, #1		/* Additonne 1 à la valeur stockée dans r5 */
		str		r5, [r4]	/* Stocke r5 à l'adresse stockée dans r4 */
		cmp		r5, r0		/* Compare la valeur stockée dans r5 à la valeur stcokée dans r0 */
		bne		next		/* Si la comparaison précédente n'est pas égale, aller à l'étiquette next */
1:		nop					/* Fait rien du tout (No Operation) */
		b		1b			/* Aller à l'étiquette 1 avant (b pour before) cette instruction */

/* Code équivalent en JAVA : */

/*
public class Main {

    public static final long LOOPS = 8;

    public static void main(String[] args) {
        long res = 16;
        short incr = 32;
        long i = 0;
        while(i != LOOPS) {
            res += incr;
            i++;
        }
    }
}
*/
