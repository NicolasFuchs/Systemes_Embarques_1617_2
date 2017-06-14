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
 * Abstract:	AM335x PWMSS - ePWM
 *
 * Purpose:	This module implements basic services to drive the AM335x
 *		Enhanced Pulse-Width Modulation module (ePWM).
 *
 * Author: 		Daniel Gachet	(First author)
 * 				Jigé Pont
 * 				Kevin Francetti
 * Date: 		17.01.2017
 * Last update: 17.01.2017
 */

#ifndef EPWM1_H_
#define EPWM1_H_

#include <stdint.h>

void epwm1_init();
void epwm1_set_frequency(uint32_t freq);
void epwm1_set_duty(uint32_t duty);

#endif /* EPWM1_H_ */
