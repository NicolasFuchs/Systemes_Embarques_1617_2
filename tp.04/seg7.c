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
#include "am335x_gpio.h"
#include <stdbool.h>
#include <stdint.h>

#define SEG_GPIO AM335X_GPIO0
#define DIG_GPIO AM335X_GPIO2

void seg7_init();
void seg7_display(int number);

struct gpio_init {
	enum am335x_gpio_modules module;
	uint32_t pin_nr;
	bool state;
};

static const struct gpio_init gpio_init[] = {
		{DIG_GPIO, 2, true},	//DIG1
		{DIG_GPIO, 3, true},	//DIG2

		{DIG_GPIO, 4, false},	//DP1
		{DIG_GPIO, 5, false},	//DP2

		{SEG_GPIO, 4, false},	//SEGA
		{SEG_GPIO, 5, false},	//SEGB
		{SEG_GPIO, 14, false},	//SEGC
		{SEG_GPIO, 22, false},	//SEGD
		{SEG_GPIO, 23, false},	//SEGE
		{SEG_GPIO, 26, false},	//SEGF
		{SEG_GPIO, 27, false},	//SEGG
};

bool display[10][7] = {								//indice = numéro affiché
		{true,true,true,true,true,true,false},		//0
		{false,true,true,false,false,false,false},	//1
		{true,true,false,true,true,false,true},		//2
		{true,true,true,true,false,false,true},		//3
		{false,true,true,false,false,true,true},	//4
		{true,false,true,true,false,true,true},		//5
		{true,false,true,true,true,true,true},		//6
		{true,true,true,false,false,false,false},	//7
		{true,true,true,true,true,true,true},		//8
		{true,true,true,true,false,true,true}		//9
};
void seg7_off() {
	int t = (sizeof gpio_init)/(sizeof(gpio_init[0]));
	for (int i = 0; i < t; i++) {
		am335x_gpio_setup_pin_out(gpio_init[i].module, gpio_init[i].pin_nr, false);
	}
}

void seg7_init() {
	am335x_gpio_init(SEG_GPIO);
	am335x_gpio_init(DIG_GPIO);
	int t = (sizeof gpio_init)/(sizeof(gpio_init[0]));
	for (int i = 0; i < t; i++) {
		am335x_gpio_setup_pin_out(gpio_init[i].module, gpio_init[i].pin_nr, gpio_init[i].state);
	}
};

void seg7_display(int number) {
	bool neg = false;
	if(number<0){
		neg=true;
		number = -number;
	}
	int leftdig = number/10;
	int rightdig = number%10;
	seg7_off();
	am335x_gpio_setup_pin_out(DIG_GPIO, 3, true);	//DIG2
	for(int i=4;i<11;i++){
		am335x_gpio_change_state(gpio_init[i].module,gpio_init[i].pin_nr, display[rightdig][i-4]);
	}
	if (neg) am335x_gpio_change_state(DIG_GPIO, 5, true);
	for(int i=100;i>0;i--);			//attendre
	seg7_off();
	if(number>=10){
		am335x_gpio_setup_pin_out(DIG_GPIO, 2, true);	//DIG1
		for(int i=4;i<11;i++){
			am335x_gpio_change_state(gpio_init[i].module, gpio_init[i].pin_nr, display[leftdig][i-4]);
		}
	}



}


