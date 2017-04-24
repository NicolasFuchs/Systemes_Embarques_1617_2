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
 * Project:	HEIA-FR / Embedded Systems 2 Laboratory
 *
 * Abstract: 	ARM Interrupt Handling - Low Level Interface
 *
 * Authors: 	Daniel Gachet
 * Date: 	27.02.2016
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "interrupt.h"

/* Low level interrupt handling ----------------------------------------------*/


/** 
  declare low level routines implemented in assembler file interrupt_asm.s 
 */
extern void interrupt_init_low_level(void(*)(enum interrupt_vectors, void*));


/** 
  interrupt service routines 
*/
struct int_vector_entry {
    interrupt_isr_t handler;
    void* param;
};
static struct int_vector_entry int_vector_table[INT_NB_VECTORS];


/**
  Low level interrupt handler written in C and called from assembler file 
 */
void interrupt_process (enum interrupt_vectors vector, void* address) 
{
    struct int_vector_entry* node = &int_vector_table[vector];
    if (node->handler != 0) {
        node->handler (vector, address, node->param);
    } else {
        printf("No interrupt handler defined for vector %d. Freezing!", vector);
        while(1);
    }
}


/* Public methods ------------------------------------------------------------*/

void interrupt_init() 
{
	interrupt_init_low_level(interrupt_process);
	memset (int_vector_table, 0, sizeof(int_vector_table));
}

int interrupt_attach (
	enum interrupt_vectors vector, 
	interrupt_isr_t routine,
        void* param) 
{
	int status = -1;
	struct int_vector_entry* node = &int_vector_table[vector];
	if ((vector < INT_NB_VECTORS) && (node->handler == 0)) {
		node->handler = routine;
		node->param = param;
		status = 0;
	}
	return status;
}

void interrupt_detach (enum interrupt_vectors vector) 
{
	if (vector < INT_NB_VECTORS) {
		int_vector_table[vector].handler = 0;
	}
}

