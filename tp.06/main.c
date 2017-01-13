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
 *			main
 *
 * Author: 	Charlotte Junod et Nicolas Fuchs
 * Date: 	13.1.2017
 */

#include <stdio.h>
#include <stdbool.h>
#include "buzzer.h"
#include "thermo.h"
#include "buttons.h"
#include "seg7.h"
#include "wheel.h"
#include "leds.h"
// ----------------------------------------------------------------------------

//min freq=400, max freq=5000
#define FREQ(x) 68*x+400
#define INV_FREQ(x) (x-400)/68

static int crt_frequency=3800;

static void set_high_thermo(){
	printf("entering high seuil settings\n");
	buzzer_off();
	leds_turn_on(1);
	int crt_value = thermo_read_high_seuil();
	int low_seuil=thermo_read_low_seuil();
	seg7_display(crt_value);
	//réglages
	while(true){
		enum wheel_states state = wheel_get_state();
		if(state==RESET){
			while(wheel_get_state()==RESET);
			thermo_set_high_seuil(crt_value);
			printf("high thermo set to %d\n",crt_value);
			leds_turn_off(1);
			return;
		}
		else if(state==INCR && crt_value<99){
			crt_value++;
			printf("high seuil: %d\n",crt_value);
		}
		else if(state==DECR && crt_value>-99 && crt_value>low_seuil+1){
			crt_value--;
			printf("high seuil: %d\n",crt_value);
		}
		seg7_display(crt_value);
	}
}

static void set_low_thermo(){
	printf("entering high seuil settings\n");
	buzzer_off();
	leds_turn_on(2);
	int crt_value = thermo_read_low_seuil();
	int high_seuil=thermo_read_high_seuil();
	seg7_display(crt_value);
	//réglages
	while(true){
		enum wheel_states state = wheel_get_state();
		if(state==RESET){
			while(wheel_get_state()==RESET);
			thermo_set_low_seuil(crt_value);
			printf("low thermo set to %d\n",crt_value);
			leds_turn_off(2);
			return;
		}
		else if(state==INCR && crt_value<99&& crt_value<high_seuil-1){
			crt_value++;
			printf("low seuil: %d\n",crt_value);
		}
		else if(state==DECR && crt_value>-99){
			crt_value--;
			printf("low seuil: %d\n",crt_value);
		}
		seg7_display(crt_value);
	}
}


//fonction bonus
static void set_freq(){
	printf("entering frequence settings\n");
		buzzer_on();
		leds_turn_on(3);
		int crt_value = INV_FREQ(crt_frequency);
		seg7_display(crt_value);
		//réglages
		while(true){
			enum wheel_states state = wheel_get_state();
			if(state==RESET){
				while(wheel_get_state()==RESET);
				crt_frequency=FREQ(crt_value);
				buzzer_set_frequency(crt_frequency);
				printf("frequence set to %d\n",crt_frequency);
				leds_turn_off(3);
				return;
			}
			else if(state==INCR && crt_value<99){
				crt_value++;
				crt_frequency=FREQ(crt_value);
				buzzer_set_frequency(crt_frequency);
				printf("freq: %d\n",crt_value);
			}
			else if(state==DECR && crt_value>0){
				crt_value--;
				crt_frequency=FREQ(crt_value);
				buzzer_set_frequency(crt_frequency);
				printf("freq: %d\n",crt_value);
			}
			seg7_display(crt_value);
		}
}


int main() {
	// print program banner
	printf ("HEIA-FR - Embedded Systems 1 Laboratory\n"
		"An introduction to device driver and application development in C\n"
		"--> AM335x ePWM1 device driver\n"
		"--> I2C Thermometer (click board Thermo3)\n"
		"--> PWM Buzzer (click board Buzz)\n");


	buzzer_init();
	leds_init();
	thermo_init();
	buttons_init();
	wheel_init();
	seg7_init();

	buzzer_set_frequency(crt_frequency);
	thermo_set_high_seuil(29);
	thermo_set_low_seuil(28);

	// application...
	while(true) {
		//afficher la température
		//bouton activé -> faire les réglages
		if(is_button_pushed(1)) set_high_thermo();
		else if(is_button_pushed(2)) set_low_thermo();
		else if(is_button_pushed(3)) set_freq();
		seg7_display(thermo_read());
		if(thermo_is_alarm_on()) buzzer_on();
		else buzzer_off();
	}

	return 0;
}

