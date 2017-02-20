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

        .data

fmt_r:  .asciz "r%d  = 0x%lx, %lu\n"
fmt_c:  .asciz "cpsr = 0x%08lx, %10lu\n"
fmt_s:  .asciz "spsr = 0x%08lx, %10lu\n"

        .text
        .global dump_regs
        .global get_regs
        .global get_main_id
        .global get_silicon_id

#define DUMP_REGS_LOCAL_VARIABLE_SIZE 18
#define DUMP_REGS_OFFSET_CPSR 16
#define DUMP_REGS_OFFSET_SPSR 17
#define REG_SP 13

dump_regs:
        push    {r4,lr}                 // save 2 registers
        sub     sp, #DUMP_REGS_LOCAL_VARIABLE_SIZE*4
        stmia   sp, {r0-r15}

        mrs     r0, cpsr                // save cpsr at offset 16
        str     r0, [sp, #DUMP_REGS_OFFSET_CPSR*4]

        mrs     r0, spsr                // save spsr at offset 17
        str     r0, [sp, #DUMP_REGS_OFFSET_SPSR*4]

        ldr     r0, [sp, #REG_SP*4]         // fix sp
        add     r0, #(2 + DUMP_REGS_LOCAL_VARIABLE_SIZE)*4
        str     r0, [sp, #REG_SP*4]

        mov     r4, #0                  // for (int i=0; i<16; i++)

1:      ldr     r0, =fmt_r              // format
        mov     r1, r4                  // register no
        ldr     r2, [sp, r4, lsl #2]    // register value
        mov     r3, r2                  // copy value to r3
        bl      printf
        add     r4, #1
        cmp     r4, #16
        blo     1b

        ldr     r0, =fmt_c              // format
        ldr     r1, [sp, #DUMP_REGS_OFFSET_CPSR*4]
        mov     r2, r1                  // copy value to r2
        bl      printf

        ldr     r0, =fmt_s              // format
        ldr     r1, [sp, #DUMP_REGS_OFFSET_SPSR*4]
        mov     r2, r1                  // copy value to r2
        bl      printf

        add     sp, #DUMP_REGS_LOCAL_VARIABLE_SIZE*4
        pop     {r4,pc}                 // restore R4 and return


get_regs:
        stmia   r0!, {r0-r15}
        mrs     r1, cpsr
        str     r1, [r0], #4
        mrs     r1, spsr
        str     r1, [r0], #4
        bx      lr


get_main_id:
        mrc   p15, 0, r0, c0, c0, 0
        bx    lr
        

get_silicon_id:
        mrc     p15, 1, r0, c0, c0, 7
        bx      lr