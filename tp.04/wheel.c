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
 * Abstract:	Introduction the C programming language
 *
 * Purpose:	Simple C program implementing basic access to the GPIO modules
 *		of the AM3358 microprocessor to control the wheel and the
 * 		7-segment display of the HEIA-FR extension card of the
 *		Beaglebone Black board
 *
 * Author: 	Charlotte Junod and Nicolas Fuchs
 * Date: 	25.11.2016
 */

#include <stdio.h>
#include <stdbool.h>
#include "am335x_gpio.h"

#define CHA_GPIO AM335X_GPIO2
#define CHA_PIN 1
#define CHB_GPIO AM335X_GPIO1
#define CHB_PIN 29
#define SW_GPIO AM335X_GPIO0
#define SW_PIN 2

enum wheel_states {NONE,INCR,DECR,RESET};

static bool a_prev = 0;
static bool b_prev = 0;
static enum wheel_states prev_state = NONE;

void wheel_init(){
	am335x_gpio_init(SW_GPIO);
	am335x_gpio_init(CHB_GPIO);
	am335x_gpio_init(CHA_GPIO);
	am335x_gpio_setup_pin(CHA_GPIO,CHA_PIN,AM335X_GPIO_PIN_IN,AM335X_GPIO_PULL_NONE);
	am335x_gpio_setup_pin(CHB_GPIO,CHB_PIN,AM335X_GPIO_PIN_IN,AM335X_GPIO_PULL_NONE);
	am335x_gpio_setup_pin(SW_GPIO,SW_PIN,AM335X_GPIO_PIN_IN,AM335X_GPIO_PULL_NONE);
}

enum wheel_states wheel_get_state(){
	enum wheel_states state = NONE;

	//lire CHA
	//lire CHB
	bool cha = am335x_gpio_get_state(CHA_GPIO,CHA_PIN);
	bool chb = am335x_gpio_get_state(CHB_GPIO,CHB_PIN);
	bool sw = am335x_gpio_get_state(SW_GPIO,SW_PIN);

	if(cha!=a_prev){
		if(prev_state==NONE) prev_state=INCR;
	}
	if(chb!=b_prev){
		if(prev_state==NONE) prev_state=DECR;
	}
	if(cha==chb && prev_state!=NONE){
		state=prev_state;
		a_prev=cha;
		b_prev=chb;
		prev_state=NONE;
	}

	if(!sw){
		state=RESET;
	}
	return state;
}


