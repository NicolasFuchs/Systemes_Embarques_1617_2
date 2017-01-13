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
 * Purpose:	Program for TP06 Systèmes embarqués
 *			manage thermometer
 *
 * Author: 	Charlotte Junod et Nicolas Fuchs
 * Date: 	13.1.2017
 */

#include <stdbool.h>
#include <stdio.h>
#include "am335x_i2c.h"
#include "am335x_gpio.h"

#define THERMO 	0x48
#define TEMP 	0
#define CONFIG	1
#define T_LOW	2
#define T_HIGH	3
#define ALARM_GPIO 	2
#define ALARM_PIN	17


void thermo_init(){
	am335x_i2c_init(AM335X_I2C2,400000);
	am335x_gpio_setup_pin_in(ALARM_GPIO,ALARM_PIN,AM335X_GPIO_PULL_NONE, false);
}

int thermo_read(){
	uint8_t data[2]={0x80,0};
	int status = am335x_i2c_read(AM335X_I2C2, THERMO,TEMP, data,2);
	if(status==0) return (int)data[0];
	else return -100;
}

bool thermo_is_alarm_on(){
	static bool old_state=false;
	bool state = !am335x_gpio_get_state(ALARM_GPIO,ALARM_PIN);
	if(state!=old_state){
		printf("State changed: now %s\n", state?"on":"off");
		old_state=state;
	}
	return state;
}

int thermo_read_low_seuil(){
	uint8_t data[2] ={0x80,0};
	int status=am335x_i2c_read(AM335X_I2C2,THERMO,T_LOW,data,2);
	int tmp= status==0? (int8_t)data[0] : -100;
	printf("low limit is %d\n",tmp);
	return tmp;
}

int thermo_read_high_seuil(){
	uint8_t data[2] ={0x80,0};
	int status=am335x_i2c_read(AM335X_I2C2,THERMO,T_HIGH,data,2);
	int tmp= status==0? (int8_t)data[0] : -100;
	printf("high limit is %d\n",tmp);
	return tmp;
}

int thermo_set_low_seuil(int tmp){
	uint8_t data[2]={tmp,0};
	int status=am335x_i2c_write(AM335X_I2C2,THERMO,T_LOW,data,2);
	printf("low limit set to %d\n",tmp);
	return status;
}

int thermo_set_high_seuil(int tmp){
	uint8_t data[2]={tmp,0};
		int status=am335x_i2c_write(AM335X_I2C2,THERMO,T_HIGH,data,2);
		printf("high limit set to %d\n",tmp);
		return status;
}
