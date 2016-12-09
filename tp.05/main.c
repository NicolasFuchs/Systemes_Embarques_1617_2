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
 * Author: 	<author's>
 * Date: 	<date>
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "seg7.h"
#include "wheel.h"
#include "buttons.h"
#include "leds.h"
#include "dmtimer1.h"

uint64_t counter;

// ----------------------------------------------------------------------------
// main program...
// ----------------------------------------------------------------------------
void enter_def_state();
void enter_chrono_mode();
void chrono_start();
int chrono_stop();
void print_nb_in_tic(int cnt);
void enter_countdown_mode();
void countdown_start();
int countdown_stop();
void print_nb_in_ds(int cnt);
void reset();
void init();

void enter_def_state(){
	while(true) {
		if(is_button_pushed(1)) 		enter_chrono_mode();
		else if(is_button_pushed(2)) 	enter_countdown_mode();
	}
}

void enter_chrono_mode(){
	counter=0;
	chrono_start();
}

void chrono_start(){
	counter=0;
	int depVal=timer_getVal();
	int intervalle=0;
	while(true){
		intervalle=timer_getVal()-depVal;
		counter=counter+intervalle;
		depVal+=intervalle;
		print_nb_in_tic(counter);
		if(wheel_get_state()==RESET) depVal=chrono_stop();
		if(is_button_pushed(3)) reset();
	}
}

int chrono_stop(){
	while(true){
		if(wheel_get_state()==RESET){
			return timer_getVal();
		}
		if(is_button_pushed(3)) reset();
	}
}

void print_nb_in_tic(int cnt){
	cnt=(int)((double)cnt/(double)timer_get_frequency())*10; //cnt en ds
	if(cnt>=10) cnt/=10;
	if(cnt>99) cnt=99;
	seg7_display(cnt);
}

void enter_countdown_mode(){
	counter=0;
	while(true){
		if(wheel_get_state()==INCR && counter<99) counter++;
		if(wheel_get_state()==DECR && counter>0) counter--;
		seg7_display(counter);
		if(wheel_get_state()==RESET){
			counter*=10;
			countdown_start();
		}
		if(is_button_pushed(3)) reset();
	}
}

void countdown_start(){
	int depVal=timer_getVal();
	int intervalle=0;
	while(true){
		intervalle=timer_getVal()-depVal;
		depVal+=intervalle;
		if(counter>0)counter=counter-(intervalle*timer_get_frequency())*10;
		print_nb_in_ds(counter);
		if(wheel_get_state()==RESET) depVal=countdown_stop();
		if(is_button_pushed(3)) reset();
	}
}

int countdown_stop(){
	while(true){
		if(wheel_get_state()==RESET){
			return timer_getVal();
		}
		if(is_button_pushed(3)) reset();
	}
}

void print_nb_in_ds(int cnt){
	if(cnt>=10) cnt/=10;
	if(cnt>99) cnt=99;
	seg7_display(cnt);
}

void reset(){
	counter=0;
	init();
	enter_def_state();
}

void init(){
	seg7_init();
	buttons_init();
	wheel_init();
	leds_init();
	timer_init();
	counter=0;
}


int main()
{
	// print program banner
	printf ("HEIA-FR - Embedded Systems 1 Laboratory\n"
		"An introduction to device driver development in C\n"
		"--> AM335x DMTimer1 device driver\n");

	// initialization...

	// application...
	init();
	enter_def_state();
	return 0;
}

