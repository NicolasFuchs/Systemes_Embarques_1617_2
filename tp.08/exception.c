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
 */

/**
 * initialization method
>>>>>>> 703e908dc61d6387cfd59165e0ce9af99905897c
 */

#include <stdio.h>
#include "exception.h"
#include "interrupt.h"

static void exception_handler (enum interrupt_vectors vector, void* param)
{
    printf("exception (%d): %s\n", vector, (char*)param);
    while(vector==INT_PREFETCH);
}

extern void exception_init()
{
    interrupt_attach(INT_UNDEF,    exception_handler, "UNDEF exception raised");
    interrupt_attach(INT_SVC,      exception_handler, "SVC exception raised");
    interrupt_attach(INT_PREFETCH, exception_handler, "PREFETCH exception raised");
    interrupt_attach(INT_DATA,     exception_handler, "DATA exception raised");
}
