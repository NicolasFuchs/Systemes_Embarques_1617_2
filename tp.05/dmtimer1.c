/*
 * dmtimer1.c
 *
 *  Created on: Nov 25, 2016
 *      Author: lmi
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define TIOCP_CFG_ENUMFREE	(1<<3)	// 5
#define TIOCP_CFG_SOFTRESET	(1<<1)	// 2
#define TIOCP_CFG_AUTOIDLE	(1<<0)	// 1

struct dmtimer1_regs {
	uint32_t tidr;		// 00
	uint32_t res1[3];	// 04-0c (espace vide)
	uint32_t tiocp_cfg;	// 10
	uint32_t istat;		// 14
	uint32_t tisr;		// 18
	uint32_t tier;		// 1c
};

static volatile struct dmtimer1_regs* timer1 = (struct dmtier1_regs*)0x44e31000;

void f() {
	timer1->tiocp_cfg = TIOCP_CFG_SOFTRESET;	// W
	(*timer1).tiocp_cfg = 0x10;		// W
	uint32_t a = timer1->tiocp_cfg;	// R
	timer1->tiocp_cfg |= 0x20;		//RMW -- set
	timer1->tiocp_cfg &= ~0x40;		//RMW -- clear
}

void dmtimer1_init() {
	static bool is_initialized = false;
	if (is_initialized) {
		return;
	}
	timer1->tiocp_cfg = TIOCP_CFG_SOFTRESET;
	while ((timer1->tistat & TISTAT_RESETDONE) == 0);
	timer1->tldr = 0;
	timer1->tcrr = 0;
	timer1->ttgr = 0;
	timer1->tclr = TCLR_AR | TCLR_ST;

}
