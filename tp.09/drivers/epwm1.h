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
 * This module is based on the software library developped by Texas Instruments
 * Incorporated - http://www.ti.com/ for its AM335x starter kit.
 *
 * Project:	HEIA-FR / Embedded Systems 1 Laboratory
 *
 * Abstract:	AM335x PWMSS - ePWM1
 *
 * Purpose:	This module implements basic services to drive the AM335x  
 *		Enhanced Pulse-Width Modulation module 1 (ePWM1).
 *
 * Author: 	Daniel Gachet
 * Date: 	18.12.2016
 */

#include <stdint.h>

/**
 * method to initialize the AM335x ePWM1 resources.
 * should be called prior any other methods.
 */
extern void epwm1_init();

/** 
 * method to set the PWM frequency o
 * @param freq PWM frequency in Hz
 */
extern void epwm1_set_frequency(uint32_t freq);

/**
 * method to define the duty cycle
 * @param duty duty cycle in %
 */
extern void epwm1_set_duty(uint32_t duty);

#endif
