/*
 * buttons.h
 *
 *  Created on: Dec 9, 2016
 *      Author: lmi
 */

#include "am335x_gpio.h"
#include <stdbool.h>
#include <stdint.h>

void buttons_init();

bool is_button_pushed(int btnNb);

