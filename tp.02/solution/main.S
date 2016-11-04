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
 * Author: 	Daniel Gachet
 * Date: 	29.09.2016
 */


// Export public symbols
	.global main, is_a_prime_number, is_not_a_prime

/* Declaration of the constants */
#define MAX	100
#define true	1
#define false	0

/* Implementation of assembler functions and methods */
	.text
	.align	8

main:	nop
	// print banner...
	ldr	r0, =banner
	bl 	printf

	/* start to implement your code here */
	bl	algo
	bl	opt1
	bl	opt2

1:	nop
	b 	1b

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


// --- 1st algo....
	.section .rodata
algo_banner: .asciz "\nalgorithm...\n"

	.bss
is_a_prime_number: .space MAX

	.text
	.align 8
algo:	push	{lr}
	ldr	r0, =algo_banner
	bl	printf

	ldr	r7, =MAX
	/* 1st mark all numbers as prime number
	 * 	for (int i=0; i<MAX; i++) {
	 *		is_a_prime_number[i] = true;
	 *	}
	 */
	ldr	r0, =true
	ldr	r1, =is_a_prime_number
	ldr	r2, =0
	b	2f
1:	add	r3, r1, r2
	strb	r0, [r3]
	add	r2, #1
2:	cmp	r2, r7
	blo	1b

	/* 2nd mark all multiples as not a prime number
	 *	for (int i=2; i<MAX; i++) {
	 *		for (int j=i*2; j<MAX; j+=i) {
	 *			is_a_prime_number[j] = false;
	 *		}
	 *	}
	 */
	ldr	r0, =false
	ldr	r1, =is_a_prime_number
	ldr	r2, =2
	b	4f
1:	lsl	r3, r2, #1	// j=i*2
	b	3f
2:	add	r4, r1, r3	
	strb	r0, [r4]
	add	r3, r2		// j+=i
3:	cmp	r3, r7		// j<MAX
	blo	2b
	add	r2, #1		// i++
4:	cmp	r2, r7		// i<MAX
	blo	1b

	/* 3rd print all prime numbers
	 *	for (int i=2; i<MAX; i++) {
	 *		if (is_a_prime_number[i]) {
	 *			printf ("%d\n", i);
	 *		}
	 *	}
	 */
	ldr	r4, =is_a_prime_number
	ldr	r5, =2
	b	2f
1:	add	r1, r4, r5
	ldrb	r0, [r1]
	cmp	r0, #true
	ldr	r0, =format
	mov	r1, r5
	bleq	printf
	add	r5, #1
2:	cmp	r5, r7
	blo	1b

	pop	{pc}


// --- 1st optimization....
	.section .rodata
opt1_banner: .asciz "\n1st optimization...\n"

	.bss
	.align 8
is_not_a_prime:	.space MAX

	.text
	.align	8
opt1:	push 	{lr}
	ldr	r0, =opt1_banner
	bl	printf

	/* 	for (int i=2; i<MAX; i++) {
	 *		if (!is_not_a_prime[i]) {
	 *			printf ("%d\n", i); 
	 *			if ((i*i) < MAX) {
	 *				for (int j=i*2; j<MAX; j+=i) {
	 *					is_not_a_prime[j] = true;
	 *				}
	 *			}
	 *		}
	 *	}
	 */
	ldr	r4, =is_not_a_prime
	ldr	r5, =2
	ldr	r6, =true
	ldr	r7, =MAX
	b	5f

1:	ldrb	r0, [r4, r5]
	cmp	r0, #false
	bne	4f		// if(!is_not_a_prime[i])...
	ldr	r0, =format
	mov	r1, r5
	bl	printf

	mul	r0, r5, r5	// if ((i*i) < MAX)...
	cmp	r0, r7
	bhs	4f

	lsl	r0, r5, #1	// j=i*2
	b	3f
2:	strb	r6, [r4, r0]
	add	r0, r5		// j+=i
3:	cmp	r0, r7		// j<MAX
	blo	2b

4:	add	r5, #1		// i++
5:	cmp	r5, r7		// i<MAX
	blo	1b

	pop	{pc}


// --- 2nd optimization....
	.section .rodata
opt2_banner: .asciz "\n2nd optimization...\n"

	.bss
	.align	8
is_not_a_prime2: .space (MAX+7)/8

	.text
	.align	8

opt2:	push 	{lr}
	ldr	r0, =opt2_banner
	bl	printf

	/* 	for (int i=2; i<MAX; i++) {
	 *		if (is_not_a_prime[i/8] & (1<<i%8) == 0) {
	 *			printf ("%d\n", i); 
	 *			if ((i*i) < MAX) {
	 *				for (int j=i*2; j<MAX; j+=i) {
	 *					is_not_a_prime[j/8] |= 1<<j%8;
	 *				}
	 *			}
	 *		}
	 *	}
	 */
	ldr	r4, =is_not_a_prime2
	ldr	r5, =2
	ldr	r6, =1
	ldr	r7, =MAX
	b	5f

1:	ldrb	r0, [r4, r5, lsr #3]
	and	r1, r5, #7
	tst	r0, r6, lsl r1
	bne	4f		// if (is_not_a_prime[i/8] & (1<<i%8) == 0) 
	ldr	r0, =format
	mov	r1, r5
	bl	printf

	mul	r0, r5, r5	// if ((i*i) < MAX)...
	cmp	r0, r7
	bhs	4f

	lsl	r0, r5, #1	// j=i*2
	b	3f
2:	ldrb	r2, [r4, r0, lsr #3]
	and	r1, r0, #7
	orr	r2, r6, lsl r1
	strb	r2, [r4, r0, lsr #3]
	add	r0, r5		// j+=i
3:	cmp	r0, r7		// j<MAX
	blo	2b

4:	add	r5, #1		// i++
5:	cmp	r5, r7		// i<MAX
	blo	1b

	pop	{pc}

