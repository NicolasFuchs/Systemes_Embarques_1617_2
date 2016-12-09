/*
 * dmtimer1.c
 *
 *  Created on: Dec 5, 2016
 *      Author: lmi
 */

#include <stdbool.h>
#include <stdint.h>
#include "dmtimer1.h"
#include <am335x_clock.h>

//accès aux registres pour le timer:

#define TISTAT_RESETDONE 1
#define TIOCP_CFG_SOFTRESET (1<<1)
#define TIOCP_CFG_AUTOIDLE 1
#define TIOCP_CFG_ENUMFREE (1<<5)
#define TCLR_AR (1 << 1)
#define TCLR_ST 1

#define abs(x) x<0?-x:x

//déclaration:
struct dmtimer1_regs{
	uint32_t tidr;		//00 - 03
	uint32_t res1[3];	//04 - 0f    		--> compenser les "trous" quand il y en a (voir le datasheet)
	uint32_t tiocp_cfg;	//10 - 13			(compléter l'offset entre les adresses)
	uint32_t tistat;	//14 - 17
	uint32_t tisr;		//18 - 1b
	uint32_t tier;		//1c - 1e
	uint32_t twer;		//20 - 23
	uint32_t tclr;		//24 - 27
	uint32_t tcrr;		//28 - 2b
	uint32_t tldr;		//2c - 2ftimer1->tcrr;
	uint32_t ttgr;		//30 - 33
	uint32_t twps;		//34 - 37
	uint32_t tmar;		//38 - 3b
	uint32_t tcar1;		//3c - 3f
	uint32_t tsicr;		//40 - 43
	uint32_t tcar2;		//44 - 47
	uint32_t tpir;		//48 - 4b
	uint32_t tnir;		//4c - 4f
	uint32_t tcvr;		//50 - 53
	uint32_t tocr;		//54 - 57
	uint32_t towr;		//58 - 5b
};

static volatile struct dmtimer1_regs* timer1 = (struct dmtimer1_regs*)0x443e31000;

void timer_init(){
	am335x_clock_enable_timer_module(AM335X_CLOCK_TIMER1);
	timer1->tiocp_cfg = TIOCP_CFG_SOFTRESET;
	while((timer1->tistat & TISTAT_RESETDONE)==0);
	timer1->ttgr = 0;
	timer1->tldr = 0;
	timer1->tcrr = 0;
	timer1->tclr = TCLR_AR | TCLR_ST;
}

uint32_t timer_getVal(){
	return timer1->tcrr;
}

uint32_t timer_get_frequency(){
	return 24000000;
}
