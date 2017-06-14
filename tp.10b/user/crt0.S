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
 * Abstract:	AM335x - Startup code
 *
 * Purpose:	AM335x startup code for HEIA-FR kernel
 *
 * Author: 	Daniel Gachet
 * Date: 	22.05.2017
 */

#include "syscall.h"

	.text
	.global _start
	.section .text_startup

_start:	nop
	ldr	lr, =end  /* init lr to avoid errmsg on gdb */

	// initialize stack pointer
	ldr	sp, =__c_stack_top__

        // clear the .bss section (zero init)
	mov	r0, #0
        ldr	r1, =__bss_start__
        ldr	r2, =_end
loop:	cmp	r1, r2
	strlo	r0, [r1], #4
        blo     loop

	// initialize stdlib
	ldr	r0,=_init
	blx	r0

	// enter main program
	ldr	r0, =_base_args
	add	r1, r0, #4
	ldr	r0, [r0]
	ldr	r2, =main
        blx	r2

	// exit process
	mov	r4, #SYSCALL_EXIT
	svc	#SYSCALL_ID

end:	b	end
