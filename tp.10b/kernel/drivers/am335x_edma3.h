#pragma once
#ifndef AM335X_EDMA3_H
#define AM335X_EDMA3_H
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
 * Project:	HEIA-FR / Embedded Systems 1+2 Laboratory
 *
 * Abstract: 	AM335x EDMA3 Driver 
 *
 * Purpose:	This module implements basic services to drive the AM335x 
 * 		EDMA3 module.
 *
 * Author: 	Romain Plouzin / Daniel Gachet
 * Date: 	03.08.2016
 */

#include <stdio.h>
#include <stdlib.h>
#include "stdint.h"

enum am335x_edma3_controller {
	AM335X_EDMA3CC,
	AM335X_EDMA3TC0,
	AM335X_EDMA3TC1,
	AM335X_EDMA3TC2,
};

enum am335x_edma3_trigger_mode {
	AM335X_EDMA3_TRIG_MODE_MANUAL,
	AM335X_EDMA3_TRIG_MODE_QDMA,
	AM335X_EDMA3_TRIG_MODE_EVENT,
};

enum am335x_edma_channel_type {
	AM335X_EDMA3_CHANNEL_TYPE_DMA, 
	AM335X_EDMA3_CHANNEL_TYPE_QDMA,
};

/**
* \brief EDMA3 Parameter RAM Set in User Configurable format
*
* This is a mapping of the EDMA3 PaRAM set provided to the user
* for ease of modification of the individual fields
*/
struct edma3cc_param_entry {
       /** OPT field of PaRAM Set */
       unsigned int opt;

       /**
        * \brief Starting byte address of Source
        * For FIFO mode, srcAddr must be a 256-bit aligned address.
        */
       unsigned int srcAddr;

       /**
        * \brief Number of bytes in each Array (ACNT)
        */
       unsigned short aCnt;

       /**
        * \brief Number of Arrays in each Frame (BCNT)
        */
       unsigned short bCnt;

       /**
        * \brief Starting byte address of destination
        * For FIFO mode, destAddr must be a 256-bit aligned address.
        * i.e. 5 LSBs should be 0.
        */
       unsigned int destAddr;

       /**
        * \brief Index between consec. arrays of a Source Frame (SRCBIDX)
        */
       short  srcBIdx;

       /**
        * \brief Index between consec. arrays of a Destination Frame (DSTBIDX)
        */
       short  destBIdx;

       /**
        * \brief Address for linking (AutoReloading of a PaRAM Set)
        * This must point to a valid aligned 32-byte PaRAM set
        * A value of 0xFFFF means no linking
        */
       unsigned short linkAddr;

       /**
        * \brief Reload value of the numArrInFrame (BCNT)
        * Relevant only for A-sync transfers
        */
       unsigned short bCntReload;

       /**
        * \brief Index between consecutive frames of a Source Block (SRCCIDX)
        */
       short  srcCIdx;

       /**
        * \brief Index between consecutive frames of a Dest Block (DSTCIDX)
        */
       short  destCIdx;

       /**
        * \brief Number of Frames in a block (CCNT)
        */
       unsigned short cCnt;

       /**
        * \brief  This field is Reserved. Write zero to this field.
        */
       unsigned short rsvd;

};

#define EDMA3CC_OPT_TCC_SHIFT (0x0000000Cu)
#define EDMA3CC_OPT_TCC (0x0003F000u)
#define EDMA3CC_OPT_TCINTEN_SHIFT (0x00000014u)


extern void am335x_edma3_init(
	enum am335x_edma3_controller ctl, 
	uint32_t queNum);

extern void am335x_edma3_channel_init (
	enum am335x_edma3_controller ctl, 
	enum am335x_edma_channel_type type,
	uint32_t channel,
	uint32_t queNum,
	void (*callback)(void*param),
	void* param);

extern void am335x_edma3_set_param(
	enum am335x_edma3_controller controller,							
	uint32_t chNum, 
	struct edma3cc_param_entry* newPaRAM);

extern uint8_t am335x_edma3_enable_transfer(
	enum am335x_edma3_controller controller, 
	uint32_t chNum,
	enum am335x_edma3_trigger_mode trigMode);

extern void am335x_edma_completion_isr();
extern void am335x_edma_error_isr();


#endif
