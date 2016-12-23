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
	return !am335x_gpio_get_state(GPIO,btnNb+14);
}


