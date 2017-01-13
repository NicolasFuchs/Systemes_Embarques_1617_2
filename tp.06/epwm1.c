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
 *			manage pwm module
 *
 * Author: 	Charlotte Junod et Nicolas Fuchs
 * Date: 	13.1.2017
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <am335x_clock.h>
#include <am335x_mux.h>
#include "epwm1.h"

// define am335x epwm controller registers
struct epwm_ctrl {
	// Time-Base Registers (TB)
	uint16_t tbctl;		// 00 -> reset value (?)
	uint16_t tbsts;		// 02
	uint16_t tbphshr;	// 04
	uint16_t tbphs;		// 06
	uint16_t tbcnt;		// 08 -> compteur
	uint16_t tbprd;		// 0a -> période
	uint16_t res1[1];	// 0c

	// Counter Compare Registers (CC)
	uint16_t cmpctl;	// 0e -> "shadowing" (?)
	uint16_t cmpahr;	// 10
	uint16_t cmpa;		// 12 -> counter compare
	uint16_t cmpb;		// 14

	// Action Qualifier Registers (AQ)
	uint16_t aqctla;	// 16 -> 0 -> actions disabled
	uint16_t aqctlb;	// 18
	uint16_t aqsfrc;	// 1a
	uint16_t aqcsfrc;	// 1c

	// Dead Band Registers (DB)
	uint16_t dbctl;		// 1e
	uint16_t dbred;		// 20
	uint16_t dbfed;		// 22

	// Trip-Zone Registers (TZ)
	uint16_t tzsel;		// 24
	uint16_t res2[1];	// 26
	uint16_t tzctl;		// 28
	uint16_t tzeint;	// 2a
	uint16_t tzflg;		// 2c
	uint16_t tzclr;		// 2e
	uint16_t tzfrc;		// 30

	// Event Trigger and Interrupt Registers (ET)
	uint16_t etsel;		// 32
	uint16_t etps;		// 34
	uint16_t etflg;		// 36
	uint16_t etclr;		// 38
	uint16_t etfrc;		// 3a

	// PWM-Chopper Register (PC)
	uint16_t pcctl;		// 3c

	// reserved...
	uint16_t res3[65];

	// High Resolution Register
	uint16_t hrcnfg;	// c0
};

// system clock frequency
#define SYSCLK		(100000000)

// TB register definition
#define TB_PRD_MAX	(65535)
#define TB_SYNC_DISABLE	(3<<4)
#define TB_HSDIV_SHIFT	(7)
#define TB_CLKDIV_SHIFT	(10)

// AQ register definition
#define AQ_ZRO_SET	(2<<0)
#define AQ_ZRO_CLEAR	(1<<0)
#define AQ_CAU_CLEAR	(1<<4)

// TB divider look-up table
static const struct {
	uint32_t div;
	uint32_t clkdiv;
	uint32_t hsdiv;
} dividers[] = {
	{   1, 0, 0}, //   1 *  1
	{   2, 0, 1}, //   1 *  2
	{   4, 0, 2}, //   1 *  4
	{   6, 0, 3}, //   1 *  6
	{   8, 0, 4}, //   1 *  8
	{  10, 0, 5}, //   1 * 10
	{  12, 0, 6}, //   1 * 12
	{  14, 0, 7}, //   1 * 14
	{  16, 1, 4}, //   2 *  8
	{  20, 1, 5}, //   2 * 10
	{  24, 1, 6}, //   2 * 12
	{  28, 1, 7}, //   2 * 14
	{  32, 2, 4}, //   4 *  8
	{  40, 2, 5}, //   4 * 10
	{  48, 2, 6}, //   4 * 12
	{  56, 2, 7}, //   4 * 14
	{  64, 3, 4}, //   8 *  8
	{  80, 3, 5}, //   8 * 10
	{  96, 3, 6}, //   8 * 12
	{ 112, 3, 7}, //   8 * 14
	{ 128, 4, 4}, //  16 *  8
	{ 160, 4, 5}, //  16 * 10
	{ 192, 4, 6}, //  16 * 12
	{ 224, 4, 7}, //  16 * 14
	{ 256, 5, 4}, //  32 *  8
	{ 320, 5, 5}, //  32 * 10
	{ 384, 5, 6}, //  32 * 12
	{ 448, 5, 7}, //  32 * 14
	{ 512, 6, 4}, //  64 *  8
	{ 640, 6, 5}, //  64 * 10
	{ 768, 6, 6}, //  64 * 12
	{ 896, 6, 7}, //  64 * 14
	{1024, 7, 4}, // 128 *  8
	{1280, 7, 5}, // 128 * 10
	{1536, 7, 6}, // 128 * 12
	{1792, 7, 7}, // 128 * 14
};
// define am335x pwmss controller registers
struct pwm_ctrl {
	uint32_t 	 pwmss[64];
	uint32_t	 ecap[32];
	uint32_t         eqep[32];
	struct epwm_ctrl epwm;
};

// am335x epwm1 controller memory mapped access register pointers
static volatile struct pwm_ctrl* pwm = (struct pwm_ctrl*)0x48302000;
static volatile struct epwm_ctrl* epwm = (struct epwm_ctrl*)0x48302000;

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
	int diviseur= (periode+TB_PRD_MAX-1)/TB_PRD_MAX;

	int i;
	for(i=0; i<ARRAY_SIZE(dividers)-1 && diviseur>dividers[i].div;i++);

	struct divider divider;
	divider.clkdiv = dividers[i].clkdiv;
	divider.hsdiv = dividers[i].hsdiv;
	divider.prd = (f==0)? 0 : (SYSCLK/dividers[i].div/f);

	return divider;
}

// -----------------------------------------------------------------------------
// implementation of public methods
// -----------------------------------------------------------------------------

void epwm1_init()
{
	printf("epwm init\n");
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
	printf("epwm set freq\n");
	struct divider div = get_divider(freq);
	pwm->epwm.tbprd = div.prd;  //période
	pwm->epwm.tbctl = 0;

	printf("dividers: %d %d\n",(int)div.hsdiv,(int)div.clkdiv);

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
		pwm->epwm.aqctla = AQ_ZRO_SET | AQ_CAU_CLEAR;
	}
}

