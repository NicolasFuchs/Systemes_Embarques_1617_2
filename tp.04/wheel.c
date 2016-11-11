/*
 * wheel.c
 *
 *  Created on: Nov 11, 2016
 *      Author: lmi
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
	enum wheel_states state;

	//lire CHA
	//lire CHB
	bool cha = am335x_gpio_get_state(CHA_GPIO,CHA_PIN);
	bool chb = am335x_gpio_get_state(CHB_GPIO,CHB_PIN);
	bool sw = am335x_gpio_get_state(CHA_GPIO,SW_PIN);

	if(cha!=a_prev){
		if(prev_state==NONE) state=INCR;
	}
	if(chb!=b_prev){
		if(prev_state==NONE) state=DECR;
	}
	if(cha==chb && prev_state!=NONE){
		state=NONE;
	}

	a_prev=cha;
	b_prev=chb;
	prev_state=state;

	if(sw){
		state=RESET;
	}
	return state;
}


