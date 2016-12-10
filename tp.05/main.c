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
void enter_chrono_mode();
void chrono_start();
int chrono_stop();
void print_nb_in_tic(int cnt);
void enter_countdown_mode();
void countdown_start();
int countdown_stop();
void reset();
void init();

void enter_chrono_mode(){
	leds_all_off();
	leds_turn_on(1);
	counter=0;
	chrono_start();
}

void chrono_start(){
	printf("chrono start\n");
	while(wheel_get_state()==RESET);
	counter=0;
	int depVal=timer_getVal();
	int intervalle=0;
	while(true){
		enum wheel_states state = wheel_get_state();
		if(is_button_pushed(3)|| depVal<0) return;
		else if(state==RESET){
			depVal=chrono_stop();
			while(wheel_get_state()==RESET);
			printf("chrono resume\n");
		}
		else{
			intervalle=timer_getVal()-depVal;
			counter=counter+intervalle;
			depVal+=intervalle;
			print_nb_in_tic(counter);
		}
	}
}

int chrono_stop(){
	printf("chrono stop\n");
	while(wheel_get_state()==RESET);
	while(true){
		print_nb_in_tic(counter);
		if(wheel_get_state()==RESET){
			return timer_getVal();
		}
		if(is_button_pushed(3)) return -1;
	}
}

void print_nb_in_tic(int cnt){
	double dcnt=((double)cnt/(double)timer_get_frequency())*10;
	cnt=(int)dcnt; //cnt en ds
	//printf("print: %d %f",cnt,dcnt);
	if(cnt<0)cnt=0;
	if(cnt>=10) cnt/=10;
	if(cnt>99) cnt=99;
	//printf("%d\n",cnt);
	seg7_display(cnt);
}

void enter_countdown_mode(){
	printf("enter countdown mode\n");
	leds_all_off();
	leds_turn_on(2);
	counter=0;
	while(true){
		enum wheel_states state = wheel_get_state();
		if(state==INCR && counter<99) counter++;
		else if(state==DECR && counter>0) counter--;
		else if(state==RESET){
			printf("counter: %d",(int)counter);
			//  /!\ OVERFLOW!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			counter*=timer_get_frequency();
			printf("counter: %d",(int)counter);
			countdown_start();
		}
		if(is_button_pushed(3)) return;
		seg7_display(counter);
	}
}

void countdown_start(){
	printf("countdown start\n");
	while(wheel_get_state()==RESET);
	int depVal=timer_getVal();
	int intervalle=0;
	while(true){
		enum wheel_states state = wheel_get_state();
		if(state==RESET){
			depVal=countdown_stop();
			while(wheel_get_state()==RESET);
			printf("chrono resume\n");

		}
		intervalle=timer_getVal()-depVal;
		if(counter>0)counter-=intervalle;
		depVal+=intervalle;
		print_nb_in_tic(counter);
		if(is_button_pushed(3) || depVal<0) return;
	}
}

int countdown_stop(){
	printf("countdown stop\n");
	while(wheel_get_state()==RESET);
	while(true){
		enum wheel_states state = wheel_get_state();
		print_nb_in_tic(counter);
		if(state==RESET){
			return timer_getVal();
		}
		else if(is_button_pushed(3)) return -1;
	}
}

void reset(){
	printf("Reset!\n");
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
	printf("Default mode\n");
	while(true){
		leds_all_on();
		seg7_display(-88);
		if(is_button_pushed(1)){
			enter_chrono_mode();
			chrono_start();
			reset();
			printf("Default mode\n");
		}else if(is_button_pushed(2)){
			enter_countdown_mode();
			countdown_start();
			reset();
			printf("Default mode\n");
		}
	}
	return 0;
}
