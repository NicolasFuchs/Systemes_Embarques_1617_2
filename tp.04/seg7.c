/*
 * seg7.c
 *
 *  Created on: Nov 11, 2016
 *      Author: lmi
 */
#include "am335x_gpio.h"

#define SEG_GPIO AM335X_GPIO0
#define DIG_GPIO AM335X_GPIO2
#define DIG1_PIN 2
#define DIG2_PIN 3
#define A_PIN 4
#define B_PIN 5
#define C_PIN 14
#define D_PIN 22
#define E_PIN 23
#define F_PIN 26
#define G_PIN 27

struct gpio_init {
	enum am335x_gpio_modules module;
	uint32_t pin_nr;
	bool state;
};

static const struct gpio_init gpio_init[] = {
		{DIG_GPIO, 2, false},	//DIG1
		{DIG_GPIO, 3, false},	//DIG2

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

void seg7_init() {
	am335x_gpio_init(SEG_GPIO);
	am335x_gpio_init(DIG_GPIO);
	am335x_gpio_setup_pin_out(CHA_GPIO,CHA_PIN,AM335X_GPIO_PIN_IN,AM335X_GPIO_PULL_NONE);
	am335x_gpio_setup_pin_out(CHB_GPIO,CHB_PIN,AM335X_GPIO_PIN_IN,AM335X_GPIO_PULL_NONE);
	am335x_gpio_setup_pin_out(SW_GPIO,SW_PIN,AM335X_GPIO_PIN_IN,AM335X_GPIO_PULL_NONE);
}

void seg7_display(int number) {
	int leftdig = number/10;
	int rightdig = number%10;
	if (number < 10) {

	} else {

	}
}
