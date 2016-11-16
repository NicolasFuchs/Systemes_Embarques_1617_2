/*
 * seg7.c
 *
 *  Created on: Nov 11, 2016
 *      Author: lmi
 */
#include "am335x_gpio.h"
#include <stdbool.h>
#include <stdint.h>

#define SEG_GPIO AM335X_GPIO0
#define DIG_GPIO AM335X_GPIO2

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

bool display[] = {
	{true,true,true,true,true,true,false},
	{false,true,true,false,false,false,false},
	{true,true,false,true,true,false,true},
	{true,true,true,true,false,false,true},
	{false,true,true,false,false,false,true},
	{true,false,true,true,false,true,false},
	{true,false,true,true,true,true,true},
	{true,true,true,false,false,false,false},
	{true,true,true,true,true,true,true},
	{true,true,true,true,false,false,true}
};

void seg7_init() {
	am335x_gpio_init(SEG_GPIO);
	am335x_gpio_init(DIG_GPIO);
	int t = sizeof(gpio_init/sizeof(gpio_init[0]));
	for (int i = 0; i < t; i++) {
		am335x_gpio_setup_pin_out(gpio_init[i].module, gpio_init[i].pin_nr, gpio_init[i].state);
	}
};

void seg7_display(int number) {
	//int leftdig = number/10;
	//int rightdig = number%10;
	switch(number%10) {
		case 0 : break;
		case 1 : break;
		case 2 : break;
		case 3 : break;
		case 4 : break;
		case 5 : break;
		case 6 : break;
		case 7 : break;
		case 8 : break;
		case 9 : break;
		if (number > 9) {
			am335x_gpio_setup_pin_out();
		}
	}
};
