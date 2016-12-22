#pragma once
#ifndef DMTIMER1_H
#define DMTIMER1_H
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
 * Project: HEIA-FR / Embedded Systems 1 Laboratory
 *
 * Abstract:    Introduction to device driver development in C
 *
 * Purpose:  Implementation of a basic timer based on the AM335x DMTimer1 timer.
 *
 * Author:  Alan Sueur, Jonathan Rial
 * Date:    17.12.2016
 */

/**
 * method to initialize the resoures of the dmtimer
 * this method shall be called prior any other.
 */
void dmtimer1_init();

/**
 * method to get the value of the counter.
 *
 * @return value of the counter
 */
uint32_t dmtimer1_get_counter();


/**
 * method to get the frequency of the timer.
 *
 * @return frequency of the timer
 */
uint32_t dmtimer1_get_frequency();


/**
 * method to get the time spent with the following format : second+tenth_second.
 *
 * @return nb of seconds and tenth of seconds
 */
uint32_t dmtimer1_get_second_and_tenth();


/**
 * method to get the time spent in seconds.
 *
 * @return nb of seconds
 */
uint32_t dmtimer1_get_second();


/**
 * method to get the time spent in tenth of seconds.
 *
 * @return nb of tenth of seconds
 */
uint32_t dmtimer1_get_tenth_second();


/**
 * method to reset the timer.
 */
void dmtimer1_reset();

#endif
