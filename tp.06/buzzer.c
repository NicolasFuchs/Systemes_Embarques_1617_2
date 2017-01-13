/*
 * buzzer.c
 *
 *  Created on: Dec 23, 2016
 *      Author: lmi
 */

#include "epwm1.h"
#include "buzzer.h"

void buzzer_init(){
	epwm1_init();
}

void buzzer_set_frequency(int f){
	epwm1_set_frequency(f);
}

void buzzer_on(int c){
	epwm1_set_duty(50);
}

void buzzer_off(){
	epwm1_set_duty(0);
}
