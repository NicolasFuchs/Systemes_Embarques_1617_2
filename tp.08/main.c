/**
 * Copyright 2017 University of Applied Sciences Western Switzerland / Fribourg
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
 * Project:	HEIA-FR / Embedded Systems 2 Laboratory
 *
 * Abstract: 	Interrupt handling demo and test program
 *
 * Purpose:	Main module to demonstrate and to test the ARM Cortex-A8 
 *              low level interrupt handling.
 *
 * Author: 	<Nicolas Fuchs et Alan Sueur>
 * Date: 	<13.03.17>
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "interrupt.h"
#include "exception.h"

#include "interrupt.h"
#include "exception.h"

int main ()
{
	printf ("\n");
	printf ("HEIA-FR - Embedded Systems 2 Laboratory\n");
	printf ("Low Level Interrupt Handling on ARM Cortex-A8\n");
	printf ("---------------------------------------------\n");

    // initialize low level resourcces
    interrupt_init();
    exception_init();

	printf ("Test data abort with a miss aligned access\n");
	long l = 0;
	long* pl = (long*)((char*)&l+1);	// char* est utilisé pour augmenté l'adresse de 1 et non de 4
	*pl = 2;

	printf ("Test supervisor call instruction / software interrupt\n");
	__asm__ ("svc #1;");

	printf ("Test a invalid instruction\n");
	__asm__ (".word 0xffffffff;");

	printf ("Test a prefetch abort\nThis method will never return...\n");
	__asm__ ("mov pc,#0x00000000;");

	while(1);
	return 0;
}
