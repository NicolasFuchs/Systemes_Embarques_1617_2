/*
 * buzzer.c
 *
 *  Created on: Dec 23, 2016
 *      Author: lmi
 */

#include "epwm1.c"

void buzzer_init(){

}

void buzzer_set_frequency(int f){

}

void buzzer_set_cycle(int c){
	epwm->cmpa = epwm->tbprd*duty/100;
}
