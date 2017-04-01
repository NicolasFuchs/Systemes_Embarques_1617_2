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
 * Abstract:	Click Board Buzzer
 *
 * Purpose:	This module implements basic services to drive the click
 *		board BUZZ.
 *
 * Author: 	Daniel Gachet
 * Date: 	18.12.2016
 */

#include <stdbool.h>

#include "epwm1.h"
#include "buzzer.h"

void buzzer_init() {
	epwm1_init();
	epwm1_set_duty(0);
	epwm1_set_frequency(3800);
}

void buzzer_switch_on() {
	epwm1_set_duty(50);		
}

void buzzer_switch_off() {
	epwm1_set_duty(0);		
}

