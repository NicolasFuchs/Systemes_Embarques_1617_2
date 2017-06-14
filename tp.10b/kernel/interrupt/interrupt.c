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
 * Abstract: 	ARM Interrupt Handling - First-Level Interface
 *
 * Authors: 	Daniel Gachet
 * Date: 	04.03.2017
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "interrupt.h"

#ifdef DEBUG
#define pr_debug(x...) printf(x)
#else
#define pr_debug(x...)
#endif 

/* First-Level interrupt handling ----------------------------------------------*/


/** 
  declare low level routines implemented in assembler file interrupt_asm.s 
 */
extern void interrupt_init_1st_stage(void(*)(enum interrupt_vectors));


/** 
  second-level interrupt handler (slih)
*/
struct slih{
    interrupt_handler_t routine;
    void* param;
};
static struct slih slih[INT_NB_VECTORS];


/**
  First-level interrupt handler (2nd stage) written in C and called
  from first-level interrupt handler (1st stage) written in assembler
 */
void flih (enum interrupt_vectors vector) 
{
    struct slih* handler = &slih[vector];
    if (handler->routine != 0) {
        handler->routine (vector, handler->param);
    } else {
        pr_debug("No interrupt handler defined for vector %d. Freezing!\n", vector);
        while(1);
    }
}


/* Public methods ------------------------------------------------------------*/

void interrupt_init() 
{
	interrupt_init_1st_stage(flih);
	memset (slih, 0, sizeof(slih));
}

int interrupt_attach (
	enum interrupt_vectors vector, 
	interrupt_handler_t routine,
    void* param)
{
	int status = -1;
	struct slih* handler = &slih[vector];
	if ((vector < INT_NB_VECTORS) && (handler->routine == 0)) {
		handler->routine = routine;
		handler->param = param;
		status = 0;
	}
	return status;
}

void interrupt_detach (enum interrupt_vectors vector) 
{
	if (vector < INT_NB_VECTORS) {
		slih[vector].routine = 0;
		slih[vector].param = 0;
	}
}

