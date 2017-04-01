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
 * Project:	HEIA-FR / Embedded Systems 2 Laboratory
 *
 * Abstract: 	Interrupt handling demo and test program
 *
 * Purpose:	Main module to demonstrate and to test the TI AM335x
 *              hardware interrupt handling.
 *
 * Author: 	<Nicolas Fuchs & Alan Sueur>
 * Date: 	<27.03.2017>
 */

#include <stdio.h>
#include <stdbool.h>
#include <am335x_i2c.h>
#include <am335x_gpio.h>
#include "intc.h"
#include "thermo.h"

#define THERMO 		0x48
#define TEMP		0
#define CONFIG		1
#define T_LOW		2
#define T_HIGH		3

#define ALARM_GPIO	2
#define ALARM_PIN 	17

static thermo_alarm_ih_t thermo_alarm_ih;

static const char* lstr[] = {"t-high", "t-low"};

static void thermo_alarm(enum am335x_gpio_modules module, uint32_t pin, void* param) {
	(void)param;
	thermo_alarm_ih(thermo_is_alert_on());
}


void thermo_init(thermo_alarm_ih_t routine) {
	thermo_alarm_ih = routine;
	am335x_i2c_init(AM335X_I2C2, 400000);
	am335x_gpio_attach(ALARM_GPIO, ALARM_PIN, AM335X_GPIO_IRQ_BOTH, true, thermo_alarm, 0);
}

int thermo_get_temp() {
	uint8_t data[2] = {0x80, 0};
	int status = am335x_i2c_read (AM335X_I2C2, THERMO, TEMP, data, 2);
	int temp = (status == 0) ? (int)data[0] : -100;
	return temp;
}

bool thermo_is_alert_on() {
	static bool former_state = false;
	bool state = !am335x_gpio_get_state(ALARM_GPIO, ALARM_PIN);
	if (state != former_state) {
		printf ("state=%s\n", state ? "on" : "off");
		former_state = state;
	}
	return state;
}

int thermo_set_limit(enum thermo_limits limit, int temp) {
	uint8_t data[2] = {temp, 0};
	uint8_t lim = (limit == THERMO_T_HIGH) ? T_HIGH : T_LOW; 
	int status = am335x_i2c_write (AM335X_I2C2, THERMO, lim, data, 2);
	printf ("set limit[%s]=%d\n", lstr[limit], (int)((int8_t)data[0]));
	return status;
}

int thermo_get_limit(enum thermo_limits limit) 
{
	uint8_t data[2] = {0x80, 0};
	uint8_t lim = (limit == THERMO_T_HIGH) ? T_HIGH : T_LOW; 
	int status = am335x_i2c_read (AM335X_I2C2, THERMO, lim, data, 2);
	int temp = (status == 0) ? (int8_t)data[0] : -128;
	printf ("get limit[%s]=%d\n", lstr[limit], (int)((int8_t)data[0]));
	return temp;
}

