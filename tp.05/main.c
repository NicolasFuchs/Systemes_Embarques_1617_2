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
int cmode;

// ----------------------------------------------------------------------------
// main program...
// ----------------------------------------------------------------------------
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

void enter_chrono_mode(){
	cmode=10;
	leds_all_off();
	leds_turn_on(1);
	counter=0;
	chrono_start();
}

void chrono_start(){
	cmode=11;
	counter=0;
	int depVal=timer_getVal();
	int intervalle=0;
	while(true){
		intervalle=timer_getVal()-depVal;
		counter=counter+intervalle;
		depVal+=intervalle;
		print_nb_in_tic(counter);
		if(wheel_get_state()==RESET) depVal=chrono_stop();
		if(is_button_pushed(3)||depVal<0) return;
	}
}

int chrono_stop(){
	cmode=12;
	while(true){
		print_nb_in_tic(counter);
		if(wheel_get_state()==RESET){
			return timer_getVal();
		}
		if(is_button_pushed(3)) return -1;
	}
}

void print_nb_in_tic(int cnt){
	cnt=(int)((double)cnt/(double)timer_get_frequency())*10; //cnt en ds
	if(cnt>=10) cnt/=10;
	if(cnt>99) cnt=99;
	seg7_display(cnt);
}

void enter_countdown_mode(){
	printf("enter countdown mode\n");
	cmode=20;
	leds_all_off();
	leds_turn_on(2);
	counter=0;
	while(true){
		enum wheel_states state = wheel_get_state();
		if(state==INCR && counter<99) counter++;
		else if(state==DECR && counter>0) counter--;
		else if(state==RESET){
			counter*=10;		// ???
			countdown_start();
		}
		if(is_button_pushed(3)) return;
		seg7_display(counter);
	}
}

void countdown_start(){
	printf("countdown start\n");
	cmode=21;
	int depVal=timer_getVal();
	int intervalle=0;
	while(true){
		if(wheel_get_state()==RESET){
			depVal=countdown_stop();
		}else{
			intervalle=timer_getVal()-depVal;
			if(counter>0) counter=counter-(intervalle/timer_get_frequency())*10;
			depVal+=intervalle;
			//COUNTER DOES NOT MOVE
			print_nb_in_ds(counter);
			printf("test %d",(int)counter);
		}
		if(is_button_pushed(3) || depVal<0) return;
	}
}

int countdown_stop(){
	printf("countdown stop\n");
	cmode=22;
	while(true){
		print_nb_in_ds(counter);
		if(wheel_get_state()==RESET){
			return timer_getVal();
		}
		if(is_button_pushed(3)) return -1;
	}
}

void print_nb_in_ds(int cnt){
	if(cnt>=10) cnt/=10;
	if(cnt>99) cnt=99;
	printf("count: %d\n",cnt);
	seg7_display(cnt);
}

void reset(){
	printf("Reset!\n");
	cmode=100;
	init();
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
	while(true){
		printf("Default mode\n");
		leds_all_on();
		cmode=0;
		seg7_display(0);
		if(is_button_pushed(1)){
			enter_chrono_mode();
			chrono_start();
			reset();
		}else if(is_button_pushed(2)){
			enter_countdown_mode();
			countdown_start();
			reset();
		}
	}
	return 0;
}
