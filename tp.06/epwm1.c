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
 * Abstract:	AM335x PWMSS - ePWM 
 *
 * Purpose:	This module implements basic services to drive the AM335x  
 *		Enhanced Pulse-Width Modulation module (ePWM).
 *
 * Author: 	<author's>
 * Date: 	<date>
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <am335x_clock.h>
#include <am335x_mux.h>
#include "epwm1.h"

// define am335x pwmss controller registers
struct pwm_ctrl {
	uint32_t 	 pwmss[64];
	uint32_t	 ecap[32];
	uint32_t         eqep[32];
	struct epwm_ctrl epwm;
};

// am335x epwm1 controller memory mapped access register pointers
static volatile struct pwm_ctrl* pwm = (struct pwm_ctrl*)0x48302000;

// -----------------------------------------------------------------------------
// implementation of local methods
// -----------------------------------------------------------------------------

struct divider {
	uint32_t prd;		// period
	uint32_t clkdiv;	// clock prescale bits
	uint32_t hsdiv;		// high speed clock prescale bits
};

static struct divider get_divider(uint32_t f)
{
	int periode;
	if(f==0) periode=0;
	else periode = SYSCLK/f;
	int diviseur= (periode*TB_PRD_MAX-1)/TB_PRD_MAX;

	int i;
	for(i=0; i<ARRAY_SIZE(dividers)-1 && diviseur>dividers[i].div;i++);

	struct divider divider;
	divider.clkdiv = dividers[i].clkdiv;
	divider.hsdiv = dividers[i].hsdiv;
	divider.prd = (f==0)? 0 : dividers[i].div/f;

	return divider;
}

// -----------------------------------------------------------------------------
// implementation of public methods
// -----------------------------------------------------------------------------

void epwm1_init()
{
	am335x_clock_enable_epwm_module (AM335X_CLOCK_EPWM1);
	am335x_mux_setup_epwm_pins (AM335X_MUX_EPWM1);

	pwm->epwm.tbprd =0;
	pwm->epwm.tbcnt =0;
	pwm->epwm.tbctl =0;
	pwm->epwm.cmpa =0;
	pwm->epwm.cmpctl =0;
	pwm->epwm.aqctla =0;
}


// -----------------------------------------------------------------------------
//PREC: freq<=SYSCLK
void epwm1_set_frequency(uint32_t freq){
	struct divider div = get_divider(freq);

	pwm->epwm.tbprd = div.prd;  //période
	pwm->epwm.tbctl = 0;

	pwm->epwm.tbctl = 	TB_SYNC_DISABLE |
						(div.hsdiv<<TB_HSDIV_SHIFT)|
						(div.clkdiv<<TB_CLKDIV_SHIFT);

	epwm1_set_duty(50);
}

// -----------------------------------------------------------------------------

void epwm1_set_duty(uint32_t duty){
	if(duty==0){
		pwm->epwm.cmpa=0;
		pwm->epwm.aqctla= AQ_ZRO_CLEAR;
	}else if (duty>=100){
		duty=100;
		pwm->epwm.cmpa=0;
		pwm->epwm.aqctla=AQ_ZRO_SET;
	}else{
		pwm->epwm.cmpa=pwm->epwm.tbprd*duty/100;
		pwm->epwm.aqctla=0;
	}
}

