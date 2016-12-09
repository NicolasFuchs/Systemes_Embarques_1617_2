/*
 * buttons.c
 *
 *  Created on: Dec 5, 2016
 *      Author: lmi
 */

#include "am335x_gpio.h"
#include <stdbool.h>
#include <stdint.h>

#define GPIO AM335X_GPIO1
#define BTN1 15
#define BTN2 16
#define BTN3 17
//1,2,3 -> 15,16,17

//fonctions
void buttons_init(){
	am335x_gpio_init(GPIO);
	am335x_gpio_setup_pin_in(GPIO,BTN1,AM335X_GPIO_PULL_NONE,false);
	am335x_gpio_setup_pin_in(GPIO,BTN2,AM335X_GPIO_PULL_NONE,false);
	am335x_gpio_setup_pin_in(GPIO,BTN3,AM335X_GPIO_PULL_NONE,false);
}

bool is_button_pushed(int btnNb){
	return am335x_gpio_get_state(GPIO,btnNb+14);
}




