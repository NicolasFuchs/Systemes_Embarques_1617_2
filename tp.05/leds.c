/*
 * leds.c
 *
 *  Created on: Dec 9, 2016
 *      Author: lmi
 */

#include "am335x_gpio.h"
#include <stdbool.h>
#include <stdint.h>


//GPIO1_12,13,14
#define GPIO AM335X_GPIO1
#define LED1 12
#define LED2 13
#define LED3 14


void leds_init(){
	am335x_gpio_init(GPIO);
	am335x_gpio_setup_pin_out(GPIO, LED1, false);
	am335x_gpio_setup_pin_out(GPIO, LED2, false);
	am335x_gpio_setup_pin_out(GPIO, LED3, false);
}

void leds_turn_on(int no){
	am335x_gpio_change_state(GPIO,11+no,true);
}

void leds_all_on(){
	am335x_gpio_change_state(GPIO,LED1,true);
	am335x_gpio_change_state(GPIO,LED2,true);
	am335x_gpio_change_state(GPIO,LED3,true);
}

void leds_turn_off(int no){
	am335x_gpio_change_state(GPIO,11+no,false);
}

void leds_all_off(){
	am335x_gpio_change_state(GPIO,LED1,false);
	am335x_gpio_change_state(GPIO,LED2,false);
	am335x_gpio_change_state(GPIO,LED3,false);
}
