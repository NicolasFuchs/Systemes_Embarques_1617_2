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
 *			manage epwm1 functions
 *
 * Author: 	Charlotte Junod et Nicolas Fuchs
 * Date: 	13.1.2017
 */

#include "epwm1.h"
#include "buzzer.h"

#define FREQ 3800

void buzzer_init(){
	epwm1_init();
	epwm1_set_frequency(FREQ);
	buzzer_off();
}

void buzzer_set_frequency(int f){
	epwm1_set_frequency(f);
}

void buzzer_on(){
	epwm1_set_duty(50);
}

void buzzer_off(){
	epwm1_set_duty(0);
}
