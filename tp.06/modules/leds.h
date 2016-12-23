
#pragma once
#ifndef LEDS_H_
#define LEDS_H_

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
 * Purpose:	Demo program implementing a basic timer and countdown
 *		application, which is based on the AM335x DMTimer1 timer.
 *
 * Author: 	Charlotte Junod et Nicolas Fuchs
 * Date: 	15.12.16
 */

/* leds_init:
 * inits the three leds
 */
void leds_init();

/* leds_turn_on:
 * no: number of the led that we want to turn on
 */
void leds_turn_on(int no);

/* leds_all_on:
 * turns all the leds on
 */
void leds_all_on();

/* leds_turn_off:
 * no: number of the led that we want to turn off
 */
void leds_turn_off(int no);

/* leds_all_off:
 * turns all the leds off
 */
void leds_all_off();

#endif /* LEDS_H_ */
