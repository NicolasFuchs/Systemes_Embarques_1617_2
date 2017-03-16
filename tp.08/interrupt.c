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
<<<<<<< HEAD
 * Project:	HEIA-FR / Embedded Systems 2 Laboratory
 *
 * Abstract: 	Interrupt handling demo and test program
 *
 * Purpose:	Main module to demonstrate and to test the ARM Cortex-A8
 *              low level interrupt handling.
 *
 * Author: 	<Nicolas Fuchs et Alan Sueur>
 * Date: 	<13.03.17>
=======
 * Project: HEIA-FR / Embedded Systems 2 Laboratory
 *
 * Abstract:    Interrupt handling demo and test program
 *
 * Purpose: Main module to demonstrate and to test the ARM Cortex-A8
 *              low level interrupt handling.
 *
 * Author:  <Nicolas Fuchs et Alan Sueur>
 * Date:    <13.03.17>
>>>>>>> 703e908dc61d6387cfd59165e0ce9af99905897c
 */

#include <string.h>
#include <stdio.h>
#include "interrupt.h"

// Permet de créer une phase de debug et une de production
// Si DEBUG est initialisé. Lancer le program : ... + DEBUG=1
#ifdef DEBUG
#define pr_debug(x...) printf (x)
#else
#define pr_debug(x...)
#endif

extern void interrupt_init_1st_stage(void (*)(enum interrupt_vectrors vector));


// second level interrupt handler
struct slih {
    interrupt_handler_t routine;
    void* param;
};
static struct slih slih[INT_NB_VECTORS];


static void flih(enum interrupt_vectors vector)
{
    if (slih[vector].routine != 0)
        slih[vector].routine(vector, slih[vector].param);
    else {
        pr_debug("no interrupt handler defined for vectro %d. Freezing!\n", vector);
        while(1);
    }
}


extern void interrupt_init()
{
    interrupt_init_1st_stage(flih);
    memset(slih, 0, sizeof(slih));
}


extern int interrupt_attach (enum interrupt_vectors vector, interrupt_handler_t routine, void* param)
{
    int status = -1;
    if ((vector < INT_NB_VECTORS) && (slih[vector].routine == 0)) {
        slih[vector].routine = routine;
        slih[vector].param = param;
        status = 0;
    }
    return status;
}


extern void interrupt_detach (enum interrupt_vectors vector)
{
    // Pour éviter d'écrire où on ne voudrait pas
    if (vector < INT_NB_VECTORS) {
        // Un pointeur mis à zéro est réinitialisé par définition.
        slih[vector].routine = 0;
        slih[vector].param = 0;
    }
}

// those 2 methods will implemented directly in the assembler file...
//extern void interrupt_enable();
//extern int interrupt_disable();
>>>>>>> 703e908dc61d6387cfd59165e0ce9af99905897c
