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
 * Purpose:	Demo program implementing a basic timer and countdown
 *		application, which is based on the AM335x DMTimer1 timer.
 *
 * Author: 	Charlotte Junod et Nicolas Fuchs
 * Date: 	15.12.16
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
