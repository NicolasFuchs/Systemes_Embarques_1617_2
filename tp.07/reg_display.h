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

#pragma once
#ifndef REG_DISPLAY_H_
#define REG_DISPLAY_H_

#include <stdint.h>

struct regs {
    uint32_t reg[16];
    uint32_t SPSR;
    uint32_t CPSR;
};

/**
 * Permet de récupérer les registres, de les stocker sur la pile et d'afficher leur valeur
 */
extern void dump_regs();

/**
 * Mets les valeurs des registres voulues dans la structure regs
 *
 * @param struct regs Le pointeur de la structure afin qu'elle puisse être modifiée.
 */
extern void get_regs(struct regs *r);

/**
 * Récupère le main id.
 *
 * @return le main id.
 */
extern uint32_t get_main_id();

/**
 * Récupère le silicon id.
 *
 * @return le silicon id.
 */
extern uint32_t get_silicon_id();

#endif
