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
 * Project:     HEIA-FR / Embedded Systems 1 Laboratory
 *
 * Abstract:    Introduction to the development environment
 *
 * Purpose:     Simple ARM assembler program to experiment the Embedded System
 *              Laboratory development environment under Git and Eclipse.
 *
 * Author:      Nicolas Fuchs, Alan Sueur
 * Date:        20.02.2017
 */

#include <stdio.h>
#include <stdint.h>
#include "reg_display.h"

// ----------------------------------------------------------------------------

int main() {
    dump_regs();
    printf("\n");

    struct regs r;
    get_regs(&r);
    for (int i = 0; i < 16; i++)
        printf("r%-2d  = 0x%08lx, %10lu\n", i, r.reg[i], r.reg[i]);

    printf("cpsr = 0x%08lx, %10lu\n", r.CPSR, r.CPSR);
    for (int i = 31; i >= 0; i--)
        printf ((r.CPSR & (1 << i)) ? "1 " : "0 ");
    printf("\n");

    printf("spsr = 0x%08lx, %10lu\n", r.SPSR, r.SPSR);
    for (int i = 31; i >= 0; i--)
        printf ((r.SPSR & (1 << i)) ? "1 " : "0 ");
    printf("\n");

    printf("main id   : 0x%08lx\n", get_main_id());
    printf("silicon id: 0x%08lx\n", get_silicon_id());

    return 0;
}
