/*
 * test.c
 *
 *  Created on: Nov 25, 2016
 *      Author: lmi
 */

#include <stdint.h>

//accès aux registres pour le timer:

#define TIOCP_CFG_SOFTRESET (1<<1)
#define TIOCP_CFG_AUTOIDLE (1<<0)
#define TIOCP_CFG_ENUMFREE (1<<5)

//déclaration:
struct dmtimer1_regs{
	uint32_t tidr;		//00
	uint32_t res1[3];	//04-0c		--> compenser les "trous" quand il y en a (voir le datasheet)
	uint32_t tiocp_cfg;	//10			(compléter l'offset entre les adresses)
	uint32_t tistat;
	uint32_t tisr;
	uint32_t tier;
};

static volatile struct dmtimer1_regs* timer1 = (struct dmtimer1_regs*)0x443e31000;

void f(){
	//utilisation:
	timer1->tiocp_cfg=0x10;			//write
	uint32_t a = timer1->tiocp_cfg;	//read
	timer1->tiocp_cfg |= 0x10;		//RMW -- set
	timer1->tiocp_cfg &= ~0x10;		//RMW -- clear

}
