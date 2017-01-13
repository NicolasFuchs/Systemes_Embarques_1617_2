#pragma once
#ifndef EPWM1_H
#define EPWM1_H

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
 * Abstract:	Introduction to device driver development in C
 *
 * Purpose:	Program for TP06 Systèmes embarqués
 *			manage pwm module
 *
 * Author: 	Charlotte Junod et Nicolas Fuchs
 * Date: 	13.1.2017
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <am335x_clock.h>
#include <am335x_mux.h>

// macro to compute number of elements of an array
#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

// -----------------------------------------------------------------------------
// implementation of local methods
// -----------------------------------------------------------------------------


static struct divider get_divider(uint32_t f);

// -----------------------------------------------------------------------------
// implementation of public methods
// -----------------------------------------------------------------------------

void epwm1_init();


// -----------------------------------------------------------------------------
//PREC: freq<=SYSCLK
void epwm1_set_frequency(uint32_t freq);
// -----------------------------------------------------------------------------

void epwm1_set_duty(uint32_t duty);


#endif
