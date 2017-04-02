/**
* \file  edma.c
*
*
*/
/* Driver APIs */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <am335x_clock.h>

#include "am335x_edma3.h"
#include "am335x_edma3_hw.h"
#include "am335x_edma3_cc.h"
#include "am335x_edma3_tc.h"

/****************************************************************************/
/*                         GLOBAL VARIABLES                                 */
/****************************************************************************/
#define REGION_ID 0

typedef struct am335x_edma_shadow_region {
	uint32_t er;		//0
	uint32_t erh;		//4
	uint32_t ecr;		//8
	uint32_t ecrh;		//c
	uint32_t esr;		//10
	uint32_t esrh;		//14
	uint32_t cer;		//18
	uint32_t cerh;		//1c
	uint32_t eer;		//20
	uint32_t eerh;		//24
	uint32_t eecr;		//28
	uint32_t eecrh;		//2c
	uint32_t eesr;		//30
	uint32_t eesrh;		//34
	uint32_t ser;		//38
	uint32_t serh;		//3c
	uint32_t secr;		//40
	uint32_t secrh;		//44
	uint32_t _res0[2];	//48 - 4c (reserved)
	uint32_t ier;		//50
	uint32_t ierh;		//54
	uint32_t iecr;		//58
	uint32_t iecrh;		//5c
	uint32_t iesr;		//60
	uint32_t iesrh;		//64
	uint32_t ipr;		//68
	uint32_t iprh;		//6c
	uint32_t icr;		//70
	uint32_t icrh;		//74
	uint32_t _res1[2];	//78 - 7c
	uint32_t qer;		//80
	uint32_t qeer;		//84
	uint32_t qeecr;		//88
	uint32_t qeesr;		//8c
	uint32_t qser;		//90
	uint32_t qsecr;		//94
	uint32_t _res2[90];	//98 - 1fc (reserved)
}am33x_edma_shadow_region_t;

struct am335x_edma_ctrl{
	uint32_t pid; 			//0
	uint32_t cccfg;			//4
	uint32_t _res0[2];		//8 - c 		(reserved)
	uint32_t sysconfig;		//10
	uint32_t _res1[59];		//14 - fc 		(reserved)
	uint32_t dchmap[64];	//100 - 1fc
	uint32_t qchmap[8];		//200 - 21c
	uint32_t _res2[8];		//220 - 23c		(reserved)
	uint32_t dmaqnum[8];	//240 - 25c
	uint32_t qdmaqnum;		//260
	uint32_t _res3[8];		//264 - 280		(reserved)
	uint32_t quepri;		//284
	uint32_t _res4[30];		//288 - 2fc		(reserved)
	uint32_t emr;			//300
	uint32_t emrh;			//304
	uint32_t emcr;			//308
	uint32_t emcrh;			//30c
	uint32_t qemr;			//310
	uint32_t qemcr;			//314
	uint32_t ccerr;			//318
	uint32_t ccerrclr;		//31c
	uint32_t eeval;			//320
	uint32_t _res5[7];		//324 - 33c		(reserved)
	uint32_t drae0;			//340
	uint32_t draeh0;		//344
	uint32_t drae1;			//348
	uint32_t draeh1;		//34c
	uint32_t drae2;			//350
	uint32_t draeh2;		//354
	uint32_t drae3;			//358
	uint32_t draeh3;		//35c
	uint32_t drae4;			//360
	uint32_t draeh4;		//364
	uint32_t drae5;			//368
	uint32_t draeh5;		//36c
	uint32_t drae6;			//370
	uint32_t draeh6;		//374
	uint32_t drae7;			//378
	uint32_t draeh7;		//37c
	uint32_t qrae[8];		//380 - 398
	uint32_t _res19[24];	//39c - 3fc
	uint32_t q0e[16];		//400 - 43c
	uint32_t q1e[16];		//440 - 47c
	uint32_t q2e[16];		//480 - 4bc
	uint32_t _res6[80];		//4c0 - 600		(reserved)
	uint32_t qstat[3];		//600 - 608
	uint32_t _res7[5];		//60c - 61c		(reserved)
	uint32_t qwmthra; 		//620
	uint32_t _res8[7];		//624 - 63c
	uint32_t ccstat;		//640
	uint32_t _res9[111];	//644 - 7fc		(reserved)
	uint32_t mpfar;			//800
	uint32_t mpfsr;			//804
	uint32_t mpfcr;			//808
	uint32_t mppag;			//80c
	uint32_t mppa[8];		//810 - 82c
	uint32_t _res10[500];	//830 - ffc		(reserved)
	uint32_t er;			//1000
	uint32_t erh;			//1004
	uint32_t _res11[2];		//1008 - 100c	(reserved)
	uint32_t esr;			//1010
	uint32_t esr_h;			//1014
	uint32_t cer;			//1018
	uint32_t cerh;			//101c
	uint32_t eer;			//1020
	uint32_t eerh;			//1024
	uint32_t eecr;			//1028
	uint32_t eecrh;			//102c
	uint32_t eesr;			//1030
	uint32_t eesrh;			//1034
	uint32_t ser;			//1038
	uint32_t serh;			//103c
	uint32_t _res12[4];		//1040 - 104c	(reserved)
	uint32_t ier;			//1050
	uint32_t ierh;			//1054
	uint32_t iecr;			//1058
	uint32_t iecrh;			//105c
	uint32_t iesr;			//1060
	uint32_t iesrh;			//1064
	uint32_t ipr;			//1068
	uint32_t iprh;			//106c
	uint32_t icr;			//1070
	uint32_t icrh;			//1074
	uint32_t ieval;			//1078
	uint32_t _res13;		//107c	-------	(reserved)
	uint32_t qer;			//1080
	uint32_t qeer;			//1084
	uint32_t qeecr;			//1088
	uint32_t qeesr;			//108c
	uint32_t qser;			//1090
	uint32_t qsecr;			//1094
	uint32_t _res14[986];	//1098 - 1ffc	(reserved)
	am33x_edma_shadow_region_t shadow_r[8];//2000 - 2ffc
	uint32_t _res18[1024];					//3000 - 3ffc  (reserved)
	struct edma3cc_param_entry PaRAM[256];		//4000 - 5ffc
};


// pointers for MMC register's base addresses
static volatile struct am335x_edma_ctrl* edma_ctrl[] = {
		(struct am335x_edma_ctrl*)(0x49000000),	// EDMA3CC
		(struct am335x_edma_ctrl*)(0x49800000),	// EDMA3TC0
		(struct am335x_edma_ctrl*)(0x49900000),	// EDMA3TC1
		(struct am335x_edma_ctrl*)(0x49a00000),	// EDMA3TC2
};

// EDMA callback function array
static void (*cb_routine[EDMA3_NUM_TCC]) (void* param);
static void* cb_param[EDMA3_NUM_TCC];

/****************************************************************************/
/*                     API FUNCTION DEFINITIONS                             */
/****************************************************************************/

/*
** This function is used as a callback from EDMA3 Completion Handler.
*/
static void am335x_edma_callback(uint32_t tccNum, uint32_t status)
{
	(void)status;
	am335x_edma_disable_transfer(AM335X_EDMA3CC, tccNum, EDMA3_TRIG_MODE_EVENT);
	if (cb_routine[tccNum] != 0) 
		cb_routine[tccNum](cb_param[tccNum]);
}

/**
*  @brief   EDMA3 Initialization
*
*  This function initializes the EDMA3 Driver
*  Clears the error specific registers (EMCR/EMCRh, QEMCR, CCERRCLR) &
*  initialize the Queue Number Registers
*
*  @param  baseAdd                  Memory address of the EDMA instance used.\n
*
*  @param  queNum                   Event Queue Number to which the channel
*                                   will be mapped (valid only for the
*                                   Master Channel (DMA/QDMA) request).\n
*
*  @return None
*
*  \note   The regionId is the shadow region(0 or 1) used and the,
*          Event Queue used is either (0 or 1). There are only four shadow
*          regions and only two event Queues
*/
void am335x_edma3_init (enum am335x_edma3_controller ctl, uint32_t queNum)
{
	volatile struct am335x_edma_ctrl *edma = edma_ctrl[ctl];
	uint16_t count = 0;
	uint16_t i = 0;

	// enable edma3 clock module
	am335x_clock_enable_edma_module();


	// Clear the Event miss Registers                                       
	edma->emcr = EDMA3_SET_ALL_BITS;
	edma->emcrh = EDMA3_SET_ALL_BITS;

	edma->qemcr = EDMA3_SET_ALL_BITS;

	// Clear CCERR register                                                 
	edma->ccerrclr = EDMA3_SET_ALL_BITS;

	// FOR TYPE EDMA
	// Enable the DMA (0 - 64) channels in the DRAE and DRAEH registers 
	edma->drae0 = EDMA3_SET_ALL_BITS;
  	edma->draeh0 = EDMA3_SET_ALL_BITS;

	for(i = 0; i < 64; i++)
	{
		// All events are one to one mapped with the channels 
		edma->dchmap[i] = i << 5;
	}

	// Initialize the DMA Queue Number Registers                           
	for (count = 0; count < 64; count++)
	{
	    edma->dmaqnum[count >> 3] &= ~(0x7u << ((count % 8u) * 4u));
	    edma->dmaqnum[count >> 3] |= (0x7u & queNum) << ((count % 8u) * 4u);
	}

	// FOR TYPE QDMA 
	// Enable the DMA (0 - 64) channels in the QRAE register
	edma->qrae[REGION_ID] = (0xFFFFFFFF & 0xFF);
	

	// Initialize the QDMA Queue Number Registers                          
	for (count = 0; count < 64; count++)
	{
	   edma->qdmaqnum &= ~(0x7u << (count * 4u));
	   edma->qdmaqnum |= (0x7u & queNum) << (count * 4u);
	}
}

void am335x_edma3_channel_init (
	enum am335x_edma3_controller ctl, 
	enum am335x_edma_channel_type type,
	uint32_t channel,
	uint32_t queNum,
	void (*callback)(void*param),
	void* param)
{
	// Request DMA Channel and TCC for MMCSD Transmit
	am335x_edma_request_channel(ctl, type, channel, channel, queNum);

	// Registering Callback Function for TX
	cb_routine[channel] = callback;
	cb_param[channel] = param;
}


/**
 * @brief   Copy the user specified PaRAM Set onto the PaRAM Set associated
 *          with the logical channel (DMA/Link).
 *
 * This API takes a PaRAM Set as input and copies it onto the actual PaRAM Set
 * associated with the logical channel. OPT field of the PaRAM Set is written
 * first and the CCNT field is written last.
 *
 *
 * @param   baseAdd                Memory address of the EDMA instance used.\n
 *
 * @param   chNum                  Logical Channel whose PaRAM set is
 *                                 requested.\n
 *
 * @param   newPaRAM               Parameter RAM set to be copied onto existing
 *                                 PaRAM.\n
 *
 * @return  None
 */
void am335x_edma3_set_param (enum am335x_edma3_controller controller,
							uint32_t chNum,
                   			struct edma3cc_param_entry* new_param)
{
	volatile struct am335x_edma_ctrl *edma = edma_ctrl[controller];
    uint32_t param_id = chNum; // PaRAM mapped to channel Number
    uint32_t i = 0;
    uint32_t *sr = (uint32_t *)new_param;
    volatile uint32_t *ds;

    //ds = (uint32_t *)(baseAdd + EDMA3CC_OPT(param_id));
    ds = (uint32_t*)&edma->PaRAM[param_id].opt;

    for(i=0; i < EDMA3CC_PARAM_ENTRY_FIELDS; i++)
    {
        *ds = *sr;
        ds++;
        sr++;
    }
}

/**
 *  @brief   Retrieve existing PaRAM set associated with specified logical
 *           channel (DMA/Link).
 *
 *  @param   baseAdd                Memory address of the EDMA instance used.\n
 *
 *  @param   chNum                  Logical Channel whose PaRAM set is
 *                                  requested.\n
 *
 *  @param   currPaRAM              User gets the existing PaRAM here.\n
 *
 *  @return  None
 */
void am335x_edma_get_param(enum am335x_edma3_controller controller, uint32_t param_id,
                   			struct edma3cc_param_entry* currPaRAM)
{
	volatile struct am335x_edma_ctrl *edma = edma_ctrl[controller];
    uint32_t i = 0;
    uint32_t *sr;
    uint32_t *ds = (uint32_t *)currPaRAM;

    //sr = (uint32_t *)(baseAdd + EDMA3CC_OPT(PaRAMId));
    sr = (uint32_t*)&edma->PaRAM[param_id].opt;

    for(i=0; i<EDMA3CC_PARAM_ENTRY_FIELDS; i++)
    {
        *ds++ = *sr++;
    }
}

/**
 *  @brief    Start EDMA transfer on the specified channel.
 *
 *  There are multiple ways to trigger an EDMA3 transfer. The triggering mode
 *  option allows choosing from the available triggering modes: Event,
 *  Manual or QDMA.
 *
 *  In event triggered, a peripheral or an externally generated event triggers
 *  the transfer. This API clears the Event and Event Miss Register and then
 *  enables the DMA channel by writing to the EESR.
 *
 *  In manual triggered mode, CPU manually triggers a transfer by writing a 1
 *  in the Event Set Register ESR. This API writes to the ESR to start the
 *  transfer.
 *
 *  In QDMA triggered mode, a QDMA transfer is triggered when a CPU (or other
 *  EDMA3 programmer) writes to the trigger word of the QDMA channel PaRAM set
 *  (auto-triggered) or when the EDMA3CC performs a link update on a PaRAM set
 *  that has been mapped to a QDMA channel (link triggered). This API enables
 *  the QDMA channel by writing to the QEESR register.
 *
 *  @param  baseAdd         Memory address of the EDMA instance used.\n
 *
 *  @param  chNum           Channel being used to enable transfer.\n
 *
 *  @param  trigMode        Mode of triggering start of transfer (Manual,
 *                          QDMA or Event).\n
 *
 *  trigMode can have values:
 *        EDMA3_TRIG_MODE_MANUAL\n
 *        EDMA3_TRIG_MODE_QDMA\n
 *        EDMA3_TRIG_MODE_EVENT\n
 *
 *  @return  retVal         TRUE or FALSE depending on the param passed.\n
 *
 */
uint8_t am335x_edma3_enable_transfer(enum am335x_edma3_controller controller,
                                 	uint32_t chNum,
                                	enum am335x_edma3_trigger_mode trigMode)
{
    uint8_t retVal = 0;
    switch (trigMode)
    {
        case EDMA3_TRIG_MODE_MANUAL :
            if (chNum < SOC_EDMA3_NUM_DMACH)
            {
            	am335x_edma_set_event(controller, chNum);
                retVal = 1;
            }
           break;

        case EDMA3_TRIG_MODE_QDMA :
            if (chNum < SOC_EDMA3_NUM_QDMACH)
            {
            	am335x_edma_enable_qdma_event(controller, chNum);
                retVal = 1;
            }
        break;

        case EDMA3_TRIG_MODE_EVENT :
            if (chNum < SOC_EDMA3_NUM_DMACH)
            {
                // clear SECR & EMCR to clean any previous NULL request   
            	am335x_edma3_clear_missed_event(controller, chNum);

                // Set EESR to enable event                              
            	am335x_edma_enable_dma_event(controller, chNum);
                retVal = 1;
            }
        break;

        default :
            retVal = 1;
        break;
    }
    return retVal;
}

/**
 *  @brief   Enables the user to Clear any missed event
 *
 *  @param   baseAdd                Memory address of the EDMA instance used.\n
 *
 *  @param   chNum                  Allocated channel number.\n
 *
 *  @return  None
 */
void am335x_edma3_clear_missed_event(enum am335x_edma3_controller controller, uint32_t chNum)
{
	volatile struct am335x_edma_ctrl *edma = edma_ctrl[controller];
    
    if(chNum < 32)
    {
         //clear SECR to clean any previous NULL request                         
         edma->shadow_r[REGION_ID].secr = (0x01u << chNum);
         //clear EMCR to clean any previous NULL request                        
         edma->emcr |= (0x01u <<  chNum);
    }
    else
    {
    	edma->shadow_r[REGION_ID].secrh = (0x01u << (chNum - 32));
         //clear EMCRH to clean any previous NULL request                       
         edma->emcrh |= (0x01u <<  (chNum - 32));
    }
}

/**
* @brief   This API return the revision Id of the peripheral.
*
* @param   baseAdd     Memory address of the EDMA instance used.\n
*
*  @return  None
*/
uint32_t am335x_edma_get_periph_id(enum am335x_edma3_controller controller)
{
	volatile struct am335x_edma_ctrl *edma = edma_ctrl[controller];
    return (edma->pid);
}

/**
 * @brief  Enable channel to Shadow region mapping
 *
 * This API allocates DMA/QDMA channels or TCCs, and the same resources are
 * enabled in the shadow region specific register (DRAE/DRAEH/QRAE).
 * Here only one shadow region is used since, there is only one Master.
 *
 *  @param   baseAdd     Memory address of the EDMA instance used.\n
 *
 *  @param   chtype      (DMA/QDMA) Channel
 *                        For Example: For DMA it is,
 *                        EDMA3_CHANNEL_TYPE_DMA.\n
 *
 *  @param   chNum       Allocated channel number.\n
 *
 *  chtype can have values
 *        EDMA3_CHANNEL_TYPE_DMA\n
 *        EDMA3_CHANNEL_TYPE_QDMA
 *
 *  @return  None
 */
void am335x_edma_channel2shadow_enable(enum am335x_edma3_controller controller,
		                              uint32_t chType,
		                              uint32_t chNum)
{
	volatile struct am335x_edma_ctrl *edma = edma_ctrl[controller];

    // Allocate the DMA/QDMA channel 
    if (EDMA3_CHANNEL_TYPE_DMA == chType)
    {
         // FOR TYPE EDMA
         if(chNum < 32)
         {
              // Enable the DMA channel in the DRAE registers 
              edma->drae0 |= (0x01u << chNum);
         }
         else
         {
              // Enable the DMA channel in the DRAEH registers 
              edma->draeh0 |= (0x01u << (chNum - 32));
         }

    }
    else if (EDMA3_CHANNEL_TYPE_QDMA == chType)
    {
        // FOR TYPE QDMA 
        // Enable the QDMA channel in the DRAE/DRAEH registers 
        edma->qrae[REGION_ID] |= 0x01u << chNum;
    }
}

/**
 * @brief  Disable channel to Shadow region mapping
 *
 * This API allocates DMA/QDMA channels or TCCs, and the same resources are
 * enabled in the shadow region specific register (DRAE/DRAEH/QRAE).
 * Here only one shadow region is used since, there is only one Master.
 *
 * @param   baseAdd   Memory address of the EDMA instance used.\n
 *
 * @param   chtype    (DMA/QDMA) Channel
 *                      For Example: For DMA it is,
 *                      EDMA3_CHANNEL_TYPE_DMA.\n
 *
 * @param   chNum      Allocated channel number.\n
 *
 *  chtype can have values
 *        EDMA3_CHANNEL_TYPE_DMA\n
 *        EDMA3_CHANNEL_TYPE_QDMA
 *
 *  @return  None
 */
void am335x_edma_channel2shadow_disable(enum am335x_edma3_controller controller,
		                               uint32_t chType,
		                               uint32_t chNum)
{
	volatile struct am335x_edma_ctrl *edma = edma_ctrl[controller];

    // Allocate the DMA/QDMA channel
    if (EDMA3_CHANNEL_TYPE_DMA == chType)
    {
        // FOR TYPE EDMA
         if(chNum < 32)
         {
              // Enable the DMA channel in the DRAE registers
              edma->drae0 &= ~(0x01u << chNum);
         }
         else
         {
              // Enable the DMA channel in the DRAEH registers
              edma->draeh0 &= ~(0x01u << (chNum - 32));
         }
    }
    else if (EDMA3_CHANNEL_TYPE_QDMA == chType)
    {
        // FOR TYPE QDMA
        // Enable the QDMA channel in the DRAE/DRAEH registers
        edma->qrae[chNum] &= ((~0x01u) << chNum);
    }
}

/**
 *  @brief   This function maps DMA channel to any of the PaRAM sets
 *           in the PaRAM memory map.
 *
 *  @param   baseAdd   Memory address of the EDMA instance used.
 *
 *  @param   channel   The DMA channel number required to be mapped.
 *
 *  @param   paramSet  It specifies the paramSet to which DMA channel
 *                     required to be mapped.
 *
 *  @return  None
 */
void am335x_edma_channel2param(enum am335x_edma3_controller controller,
								uint32_t channel,
	                            uint32_t paramSet)
{
	volatile struct am335x_edma_ctrl *edma = edma_ctrl[controller];

    edma->dchmap[channel] = paramSet << 5;
}

/**
 *  @brief  Map channel to Event Queue
 *
 *  This API maps DMA/QDMA channels to the Event Queue
 *
 *  @param  baseAdd    Memory address of the EDMA instance used.\n
 *
 *  @param  chtype     (DMA/QDMA) Channel
 *                     For Example: For QDMA it is
 *                     EDMA3_CHANNEL_TYPE_QDMA.\n
 *
 *  @param  chNum      Allocated channel number.\n
 *
 *  @param  evtQNum    Event Queue Number to which the channel
 *                     will be mapped (valid only for the
 *                     Master Channel (DMA/QDMA) request).\n
 *
 *  chtype can have values
 *        EDMA3_CHANNEL_TYPE_DMA\n
 *        EDMA3_CHANNEL_TYPE_QDMA
 *
 *  @return  None
 */
void am335x_edma_channel2eventQ_map(enum am335x_edma3_controller controller,
			                      uint32_t chType,
			                      uint32_t chNum,
			                      uint32_t evtQNum)
{
	volatile struct am335x_edma_ctrl *edma  = edma_ctrl[controller];

    if (EDMA3_CHANNEL_TYPE_DMA == chType)
    {
        // Associate DMA Channel to Event Queue                            
        edma->dmaqnum[chNum >> 3u] &= EDMA3CC_DMAQNUM_CLR(chNum);

        edma->dmaqnum[chNum >> 3u] |= EDMA3CC_DMAQNUM_SET((chNum), evtQNum);
    }
    else if (EDMA3_CHANNEL_TYPE_QDMA == chType)
    {
        // Associate QDMA Channel to Event Queue                            
        edma->qdmaqnum |= EDMA3CC_QDMAQNUM_SET(chNum, evtQNum);
    }
}

/**
 *  @brief  Remove Mapping of channel to Event Queue
 *
 *  This API Unmaps DMA/QDMA channels to the Event Queue allocated
 *
 *  @param  baseAdd    Memory address of the EDMA instance used.\n
 *
 *  @param  chtype     (DMA/QDMA) Channel
 *                     For Example: For DMA it is
 *                     EDMA3_CHANNEL_TYPE_DMA.\n
 *
 *  @param  chNum      Allocated channel number.\n
 *
 *  chtype can have values
 *        EDMA3_CHANNEL_TYPE_DMA\n
 *        EDMA3_CHANNEL_TYPE_QDMA
 *
 *  @return  None
 */
void am335x_edma_channel2eventQ_demap(enum am335x_edma3_controller controller,
			                        uint32_t chType,
			                        uint32_t chNum)
{
	volatile struct am335x_edma_ctrl *edma = edma_ctrl[controller];

    if (EDMA3_CHANNEL_TYPE_DMA == chType)
    {
        // Unmap DMA Channel to Event Queue                                
        edma->dmaqnum[chNum >> 3u] |= EDMA3CC_DMAQNUM_CLR(chNum);
    }
    else if (EDMA3_CHANNEL_TYPE_QDMA == chType)
    {
        // Unmap QDMA Channel to Event Queue                              
        edma->qdmaqnum |= EDMA3CC_QDMAQNUM_CLR(chNum);
    }
}

/**
 *  @brief  Enables the user to map a QDMA channel to PaRAM set
 *          This API Needs to be called before programming the paRAM sets for
 *          the QDMA Channels.Application needs to maitain the paRAMId
 *          provided by this API.This paRAMId is used to set paRAM and get
 *          paRAM. Refer corresponding API's for more details.
 *
 *  @param  baseAdd                  Memory address of the EDMA instance used.\n
 *
 *  @param  chNum                    Allocated QDMA channel number.\n
 *
 *  @param  paRaMID                  PaRAM Id to which the QDMA channel will be
 *                                   mapped to.
 *
 *  @return None
 *
 *  Note : The PaRAMId requested must be greater than 32(SOC_EDMA3_NUM_DMACH).
 *         and lesser than SOC_EDMA3_NUM_DMACH + chNum  Because, the first
 *         32 PaRAM's are directly mapped to first 32 DMA channels and (32 - 38)
 *         for QDMA Channels. (32 - 38) is assigned by driver in this API.
 *
 */
void am335x_edma_qdma_channel_2_param(enum am335x_edma3_controller controller,
			                           uint32_t chNum,
			                           uint32_t *paRAMId)
{
	volatile struct am335x_edma_ctrl *edma = edma_ctrl[controller];
    
    // First 32 channels are for DMA only                                    
    if ((SOC_EDMA3_NUM_DMACH + chNum) == (*paRAMId))
    {
        // Map Parameter RAM Set Number for specified channelId             
        edma->qchmap[chNum] &= EDMA3CC_QCHMAP_PAENTRY_CLR;
        edma->qchmap[chNum] |= EDMA3CC_QCHMAP_PAENTRY_SET(*paRAMId);
    }
    else
    {
    (*paRAMId) = (SOC_EDMA3_NUM_DMACH + chNum);
        // Map Parameter RAM Set Number for specified channelId             
        edma->qchmap[chNum] &= EDMA3CC_QCHMAP_PAENTRY_CLR;
        edma->qchmap[chNum] |= EDMA3CC_QCHMAP_PAENTRY_SET(*paRAMId);
    }
}

/**
 * @brief  Assign a Trigger Word to the specified QDMA channel
 *
 * This API sets the Trigger word for the specific QDMA channel in the QCHMAP
 * Register. Default QDMA trigger word is CCNT.
 *
 * @param  baseAdd             Memory address of the EDMA instance used.\n
 *
 * @param  chNum               QDMA Channel which needs to be assigned
 *                             the Trigger Word
 *
 * @param  trigWord            The Trigger Word for the QDMA channel.
 *                             Trigger Word is the word in the PaRAM
 *                             Register Set which, when written to by CPU,
 *                             will start the QDMA transfer automatically.
 *
 * @return  None
 */
void am335x_edma_set_triggword_2_qdma(enum am335x_edma3_controller controller,
			                          uint32_t chNum,
			                          uint32_t trigWord)
{
	volatile struct am335x_edma_ctrl *edma = edma_ctrl[controller];

     // Set the Trigger Word 
     edma->qchmap[chNum] |= EDMA3CC_QCHMAP_TRWORD_SET(trigWord);
}


/**
 *  @brief   Enables the user to Clear any QDMA missed event
 *
 *
 *  @param   baseAdd                Memory address of the EDMA instance used.\n
 *
 *  @param   chNum                  Allocated channel number.\n
 *
 *  @return  None
 */
void am335x_edma3_qdma_clear_missed_event(enum am335x_edma3_controller controller,
                         			 	uint32_t chNum)
{
	volatile struct am335x_edma_ctrl *edma = edma_ctrl[controller];

    // clear SECR to clean any previous NULL request                         
	edma->shadow_r[REGION_ID].qsecr = (0x01u << chNum);

    // clear EMCR to clean any previous NULL request                       
    edma->qemcr |= (0x01u <<  chNum);
}

/**
 *  @brief   Enables the user to Clear any Channel controller Errors
 *
 *  @param   baseAdd              Memory address of the EDMA instance used.\n
 *
 *  @param   Flags                Masks to be passed.\n
 *
 *  Flags can have values:
 *
 *  EDMA3CC_CLR_TCCERR            Clears the TCCERR bit in the EDMA3CC
 *                                ERR Reg\n
 *  EDMA3CC_CLR_QTHRQ0            Queue threshold error clear for queue 0.\n
 *  EDMA3CC_CLR_QTHRQ1            Queue threshold error clear for queue 1.
 *
 *  @return  None
 */
void am335x_edma_clear_ccerr(enum am335x_edma3_controller controller, uint32_t flags)
{
	volatile struct am335x_edma_ctrl *edma = edma_ctrl[controller];
    
    // (CCERRCLR) - clear channel controller error register              
    edma->ccerrclr = flags;
}

/**
 *  @brief   Enables the user to Set an event. This API helps user to manually
 *           set events to initiate DMA transfer requests.
 *
 *  @param   baseAdd                Memory address of the EDMA instance used.\n
 *
 *  @param   chNum                  Allocated channel number.\n
 *
 *  @return  None
 *
 *  Note :   This API is generally used during Manual transfers.\n
 */
void am335x_edma_set_event(enum am335x_edma3_controller controller, uint32_t chNum)
{
	volatile struct am335x_edma_ctrl *edma = edma_ctrl[controller];

    if(chNum < 32)
    {
         // (ESR) - set corresponding bit to set a event
    	edma->shadow_r[REGION_ID].esr |= (0x01u <<  chNum);
    }
    else
    {
         // (ESRH) - set corresponding bit to set a event
    	edma->shadow_r[REGION_ID].esrh |= (0x01u << (chNum - 32));
    }
}

/**
 *  @brief   Enables the user to Clear an event.
 *
 *  @param   baseAdd                Memory address of the EDMA instance used.\n
 *
 *  @param   chNum                  Allocated channel number.\n
 *
 *  @return  None
 *
 *  Note :   This API is generally used during Manual transfers.\n
 */
void am335x_edma_clear_event(enum am335x_edma3_controller controller, uint32_t chNum)
{
	volatile struct am335x_edma_ctrl *edma = edma_ctrl[controller];
    if(chNum < 32)
    {
         // (ECR) - set corresponding bit to clear a event
    	edma->shadow_r[REGION_ID].ecr |= (0x01u <<  chNum);
    }
    else
    {
         // (ECRH) - set corresponding bit to clear a event
    	edma->shadow_r[REGION_ID].ecrh |= (0x01u << (chNum - 32));
    }
}

/**
 *  @brief   Enables the user to enable an DMA event.
 *
 *  @param   baseAdd                Memory address of the EDMA instance used.\n
 *
 *  @param   chNum                  Allocated channel number.\n
 *
 *  @return  None
 *
 *  Note :   Writes of 1 to the bits in EESR sets the corresponding event
 *           bits in EER. This is generally used for Event Based transfers.\n
 */
void am335x_edma_enable_dma_event(enum am335x_edma3_controller controller, uint32_t chNum)
{
	volatile struct am335x_edma_ctrl *edma = edma_ctrl[controller];

	if(chNum < 32)
    {
         // (EESR) - set corresponding bit to enable DMA event
    	edma->shadow_r[REGION_ID].eesr |= (0x01u <<  chNum);
    }
    else
    {
         // (EESRH) - set corresponding bit to enable DMA event
    	edma->shadow_r[REGION_ID].eesrh |= (0x01u << (chNum - 32));
    }
}

/**
 *  @brief   Enables the user to Disable an DMA event.
 *
 *  @param   baseAdd                Memory address of the EDMA instance used.\n
 *
 *  @param   chNum                  Allocated channel number.\n
 *
 *  @return  None
 *
 *  Note :   Writes of 1 to the bits in EECR clear the corresponding event bits
 *           in EER; writes of 0 have no effect.. This is generally used for
 *           Event Based transfers.\n
 */
void am335x_edma_disable_dma_event(enum am335x_edma3_controller controller, uint32_t chNum)
{
	volatile struct am335x_edma_ctrl *edma = edma_ctrl[controller];

    if(chNum < 32)
    {
         // (EECR) - set corresponding bit to disable event
    	edma->shadow_r[REGION_ID].eecr |= (0x01u <<  chNum);
    }
    else
    {
        // (EECRH) - set corresponding bit to disable event
    	edma->shadow_r[REGION_ID].eecrh |= (0x01u <<  chNum);
    }
}

/**
 *  @brief   Enables the user to enable an QDMA event.
 *
 *  @param   baseAdd                Memory address of the EDMA instance used.\n
 *
 *  @param   chNum                  Allocated channel number.\n
 *
 *  @return  None
 *
 *  Note :   Writes of 1 to the bits in QEESR sets the corresponding event
              bits in QEER.\n
 */
void am335x_edma_enable_qdma_event(enum am335x_edma3_controller controller, uint32_t chNum)
{
	volatile struct am335x_edma_ctrl *edma = edma_ctrl[controller];

    // (QEESR) - set corresponding bit to enable QDMA event
	edma->shadow_r[REGION_ID].qeesr = (0x01u << chNum);
}

/**
 *  @brief   Enables the user to disable an QDMA event.
 *
 *  @param   baseAdd                Memory address of the EDMA instance used.\n
 *
 *  @param   chNum                  Allocated channel number.\n
 *
 *  @return  None
 *
 *  Note :   Writes of 1 to the bits in QEECR clears the corresponding event
              bits in QEER.\n
 */
void am335x_edma_disable_qdma_event(enum am335x_edma3_controller controller,
                         uint32_t chNum)
{
	volatile struct am335x_edma_ctrl *edma = edma_ctrl[controller];

    // (QEESR) - set corresponding bit to enable QDMA event
	edma->shadow_r[REGION_ID].qeecr = (0x01u << chNum);
}

/**
 *  @brief   This returns EDMA3 CC error status.
 *
 *  @param   baseAdd                Memory address of the EDMA instance used.\n
 *
 *  @return  value                  Status of the Interrupt Pending Register
 *
 */
uint32_t am335x_edma_get_ccerr_status (enum am335x_edma3_controller controller)
{
	volatile struct am335x_edma_ctrl *edma = edma_ctrl[controller];

    return (uint32_t)edma->ccerr;
}

/**
 *  @brief   This function returns interrupts status of those events
 *           which is less than 32.
 *
 *  @param   baseAdd                Memory address of the EDMA instance used.\n
 *
 *  @return  value                  Status of the Interrupt Pending Register
 *
 */
uint32_t am335x_edma_get_interrupt_status (enum am335x_edma3_controller controller)
{
	volatile struct am335x_edma_ctrl *edma = edma_ctrl[controller];

    return (uint32_t)edma->shadow_r[REGION_ID].ipr;
}

/**
 *  @brief   This function returns interrupts status of those events
 *           which is greater than 32.
 *
 *  @param   baseAdd                Memory address of the EDMA instance used.\n
 *
 *  @return  value                  Status of the Interrupt Pending Register
 *
 */
uint32_t am335_edma_get_interrupt_status_high(enum am335x_edma3_controller controller)
{
    volatile struct am335x_edma_ctrl *edma = edma_ctrl[controller];

    return (uint32_t)edma->shadow_r[REGION_ID].iprh;
}

/**
 *  @brief   This returns error interrupt status for those events whose
 *           event number is less than 32.
 *
 *  @param   baseAdd                Memory address of the EDMA instance used.\n
 *
 *  @return  value                  Status of the Interrupt Pending Register
 *
 */
uint32_t am335x_edma_get_err_interrupt_status (enum am335x_edma3_controller controller)
{
	volatile struct am335x_edma_ctrl *edma = edma_ctrl[controller];

    return (uint32_t)edma->emr;
}

/**
 *  @brief   This returns error interrupt status for those events whose
 *           event number is greater than 32.
 *
 *  @param   baseAdd                Memory address of the EDMA instance used.\n
 *
 *  @return  value                  Status of the Interrupt Pending Register
 *
 */
uint32_t am335x_edma_get_err_interr_status_high(enum am335x_edma3_controller controller)
{
	volatile struct am335x_edma_ctrl *edma = edma_ctrl[controller];

    return (uint32_t)edma->emrh;
}

/**
 *  @brief   This returns QDMA error interrupt status.
 *
 *  @param   baseAdd            Memory address of the EDMA instance used.\n
 *
 *  @return  value              Status of the QDMA Interrupt Pending Register
 *
 */
uint32_t am335x_edma_get_qedma_err_interrupt_status(enum am335x_edma3_controller controller)
{
    volatile struct am335x_edma_ctrl *edma = edma_ctrl[controller];

    return (uint32_t)edma->qemr;
}

/**
 *  @brief   Enables the user to enable the transfer completion interrupt
 *           generation by the EDMA3CC for all DMA/QDMA channels.
 *
 *  @param   baseAdd                Memory address of the EDMA instance used.\n
 *
 *  @param   chNum                  Allocated channel number.\n
 *
 *  @return  None
 *
 *  Note :   To set any interrupt bit in IER, a 1 must be written to the
 *           corresponding interrupt bit in the interrupt enable set register.
 */
void am335x_edma_enable_interrupt_event(enum am335x_edma3_controller controller, uint32_t chNum)
{
	volatile struct am335x_edma_ctrl *edma = edma_ctrl[controller];

    if(chNum < 32)
    {
         // Interrupt Enable Set Register (IESR)
    	edma->shadow_r[REGION_ID].iesr |= (0x01u <<  chNum);
    }
    else
    {
         // Interrupt Enable Set Register (IESRH)
    	edma->shadow_r[REGION_ID].iesrh |= (0x01u << (chNum - 32));
    }
}

/**
 *  @brief   Enables the user to clear CC interrupts
 *
 *  @param   baseAdd                Memory address of the EDMA instance used.\n
 *
 *  @param   chNum                  Allocated channel number.\n
 *
 *  @return  None
 *
 *  Note :   Writes of 1 to the bits in IECR clear the corresponding interrupt
 *           bits in the interrupt enable registers (IER); writes of 0 have
 *           no effect.\n
 */
void am335x_edma_disable_interrupt_event(enum am335x_edma3_controller controller, uint32_t chNum)
{
	volatile struct am335x_edma_ctrl *edma = edma_ctrl[controller];

    if(chNum < 32)
    {
         // Interrupt Enable Clear Register (IECR)
    	edma->shadow_r[REGION_ID].iecr |= (0x01u <<  chNum);
    }
    else
    {
         // Interrupt Enable Clear Register (IECRH)
    	edma->shadow_r[REGION_ID].iecrh |= (0x01u << (chNum - 32));
    }
}

/**
 *  @brief   Enables the user to Clear an Interrupt.
 *
 *  @param   baseAdd                Memory address of the EDMA instance used.
 *
 *  @param   value                  Value to be set to clear the Interrupt Status.
 *
 *  @return  None
 *
 */
void am335x_edma_clear_interrupt(enum am335x_edma3_controller controller, uint32_t value)
{
	volatile struct am335x_edma_ctrl *edma = edma_ctrl[controller];

    if(value < 32)
    {
    	edma->shadow_r[REGION_ID].icr = (1u << value);
    }
    else
    {
    	edma->shadow_r[REGION_ID].icrh = (1u << (value - 32));
    }
}


/**
 * @brief   Retrieve existing PaRAM set associated with specified logical
 *          channel (QDMA).
 *
 * @param   baseAdd                Memory address of the EDMA instance used.\n
 *
 * @param   chNum                  Logical Channel whose PaRAM set is
 *                                 requested.\n
 *
 * @param   currPaRAM              User gets the existing PaRAM here.\n
 *
 * @return  None
 */
void am335x_edma_get_qdma_param(enum am335x_edma3_controller controller,
                       			uint32_t param_id,
                       			struct edma3cc_param_entry* currPaRAM)
{
	volatile struct am335x_edma_ctrl *edma = edma_ctrl[controller];
    uint32_t i = 0;
    uint32_t *sr;
    uint32_t *ds = (uint32_t *)currPaRAM;

    //sr = (uint32_t *)(baseAdd + EDMA3CC_OPT(paRAMId));
    sr = (uint32_t*)&edma->PaRAM[param_id].opt;

    for(i=0;i<EDMA3CC_PARAM_ENTRY_FIELDS;i++)
    {
        *ds=*sr;
        ds++;
        sr++;
    }
}


/**
 * @brief   Copy the user specified PaRAM Set onto the PaRAM Set associated
 *          with the logical channel (QDMA only).
 *
 * This API takes a PaRAM Set as input and copies it onto the actual PaRAM Set
 * associated with the logical channel. OPT field of the PaRAM Set is written
 * first and the CCNT field is written last.
 *
 *
 * @param   baseAdd                Memory address of the EDMA instance used.\n
 *
 * @param  paRaMID                 PaRAM Id to which the QDMA channel is
 *                        mapped to.
 *
 * @param   newPaRAM               Parameter RAM set to be copied onto existing
 *                                 PaRAM.\n
 *
 * @return  None
 */
void am335x_edma_set_qdma_param(enum am335x_edma3_controller controller,
		                       uint32_t param_id,
		                       struct edma3cc_param_entry* newPaRAM)
{
	volatile struct am335x_edma_ctrl *edma = edma_ctrl[controller];
    uint32_t i = 0;
    uint32_t *sr = (uint32_t *)newPaRAM;
    uint32_t *ds;

    //ds = (uint32_t *)(baseAdd + EDMA3CC_OPT(paRAMId));
    ds = (uint32_t*)&edma->PaRAM[param_id].opt;

    for(i=0;i<EDMA3CC_PARAM_ENTRY_FIELDS;i++)
    {
        *ds=*sr;
         ds++;
         sr++;
    }
}

/**
 * @brief   Set a particular PaRAM set entry of the specified PaRAM set
 *
 * @param   baseAdd           Memory address of the EDMA instance used.\n
 *
 * @param   paRaMID           PaRAM Id to which the QDMA channel is
 *                            mapped to.
 *
 * @param   paRAMEntry        Specify the PaRAM set entry which needs
 *                            to be set.
 *
 * @param   newPaRAMEntryVal  The new field setting. Make sure this field is
 *                            packed for setting certain fields in paRAM.
 *
 *  paRAMEntry can have values:
 *
 *  EDMA3CC_PARAM_ENTRY_OPT
 *  EDMA3CC_PARAM_ENTRY_SRC
 *  EDMA3CC_PARAM_ENTRY_ACNT_BCNT
 *  EDMA3CC_PARAM_ENTRY_DST
 *  EDMA3CC_PARAM_ENTRY_SRC_DST_BIDX
 *  EDMA3CC_PARAM_ENTRY_LINK_BCNTRLD
 *  EDMA3CC_PARAM_ENTRY_SRC_DST_CIDX
 *  EDMA3CC_PARAM_ENTRY_CCNT
 *
 * @return  None
 *
 * \note    This API should be used while setting the PaRAM set entry
 *          for QDMA channels. If EDMA3QdmaSetPaRAMEntry() used,
 *          it will trigger the QDMA channel before complete
 *          PaRAM set entry is written.
 */
void am335x_edma_set_qdma_param_entry(enum am335x_edma3_controller controller,
		                            uint32_t param_id,
		                            uint32_t paRAMEntry,
		                            uint32_t newPaRAMEntryVal)
{
	volatile struct am335x_edma_ctrl *edma = edma_ctrl[controller];

    if((paRAMEntry > EDMA3CC_PARAM_ENTRY_OPT) || (paRAMEntry < EDMA3CC_PARAM_ENTRY_CCNT))
    {
        //HWREG(baseAdd + EDMA3CC_OPT(paRAMId) +  (uint32_t)(paRAMEntry * 0x04)) = newPaRAMEntryVal;
        edma->PaRAM[param_id].opt = newPaRAMEntryVal;
    }
}

/**
 * @brief   Get a particular PaRAM entry of the specified PaRAM set
 *
 * @param   baseAdd           Memory address of the EDMA instance used.\n
 *
 * @param   paRaMID           PaRAM Id to which the QDMA channel is
 *                            mapped to.
 *
 * @param   paRAMEntry        Specify the PaRAM set entry which needs
 *                            to be read.
 *
 *  paRAMEntry can have values:
 *
 *  EDMA3CC_PARAM_ENTRY_OPT
 *  EDMA3CC_PARAM_ENTRY_SRC
 *  EDMA3CC_PARAM_ENTRY_ACNT_BCNT
 *  EDMA3CC_PARAM_ENTRY_DST
 *  EDMA3CC_PARAM_ENTRY_SRC_DST_BIDX
 *  EDMA3CC_PARAM_ENTRY_LINK_BCNTRLD
 *  EDMA3CC_PARAM_ENTRY_SRC_DST_CIDX
 *  EDMA3CC_PARAM_ENTRY_CCNT
 *
 * @return  paRAMEntryVal     The value of the paRAM field pointed by the
 *                            paRAMEntry.
 *
 * \note    This API should be used while reading the PaRAM set entry
 *          for QDMA channels. And the paRAMEntryVal is a packed value for
 *          certain fields of paRAMEntry.The user has to make sure the value
 *          is unpacked appropriately.
 *          For example, the third field is A_B_CNT. Hence he will have to
 *          unpack it to two 16 bit fields to get ACNT and BCNT.
 */
uint32_t am335x_edma_get_qdma_param_entry(enum am335x_edma3_controller controller,
		                                      uint32_t param_id,
		                                      uint32_t paRAMEntry)
{
	volatile struct am335x_edma_ctrl *edma = edma_ctrl[controller];
    uint32_t paRAMEntryVal = 0;

    if((paRAMEntry > EDMA3CC_PARAM_ENTRY_OPT) || (paRAMEntry < EDMA3CC_PARAM_ENTRY_CCNT))
    {
        paRAMEntryVal = edma->PaRAM[param_id].opt;
    }
    return(paRAMEntryVal);
}

/**
 *  @brief Request a DMA/QDMA/Link channel.
 *
 *  Each channel (DMA/QDMA/Link) must be requested  before initiating a DMA
 *  transfer on that channel.
 *
 *  This API is used to allocate a logical channel (DMA/QDMA/Link) along with
 *  the associated resources. For DMA and QDMA channels, TCC and PaRAM Set are
 *  also allocated along with the requested channel.
 *
 *  User can request a specific logical channel by passing the channel number
 *  in 'chNum'.
 *
 *  For DMA/QDMA channels, after allocating all the EDMA3 resources, this API
 *  sets the TCC field of the OPT PaRAM Word with the allocated TCC. It also sets
 *  the event queue for the channel allocated. The event queue needs to be
 *  specified by the user.
 *
 *  For DMA channel, it also sets the DCHMAP register.
 *
 *  For QDMA channel, it sets the QCHMAP register and CCNT as trigger word and
 *  enables the QDMA channel by writing to the QEESR register.
 *
 *  @param  baseAdd                  Memory address of the EDMA instance used.\n
 *
 *  @param  chtype                   (DMA/QDMA) Channel
 *                                    For Example: For DMA it is
 *                                    EDMA3_CHANNEL_TYPE_DMA.\n
 *
 *  @param  chNum                    This is the channel number requested for a
 *                                   particular event.\n
 *
 *  @param  tccNum                   The channel number on which the
 *                                   completion/error interrupt is generated.
 *                                   Not used if user requested for a Link
 *                                   channel.\n
 *
 *  @param  evtQNum                  Event Queue Number to which the channel
 *                                   will be mapped (valid only for the
 *                                   Master Channel (DMA/QDMA) request).\n
 *
 *  @return  TRUE if parameters are valid, else FALSE
 */
uint32_t am335x_edma_request_channel(enum am335x_edma3_controller controller,
		                                 uint32_t chType,
		                                 uint32_t chNum,
		                                 uint32_t tccNum,
		                                 uint32_t evtQNum)
{
	volatile struct am335x_edma_ctrl *edma = edma_ctrl[controller];
    uint32_t retVal = 0;
    if (chNum < SOC_EDMA3_NUM_DMACH)
    {
        /* Enable the DMA channel in the enabled in the shadow region
         * specific register
         */
    	am335x_edma_channel2shadow_enable(controller, chType, chNum);

        am335x_edma_channel2eventQ_map(controller, chType, chNum, evtQNum);
        if (EDMA3_CHANNEL_TYPE_DMA == chType)
        {
            // Interrupt channel nums are < 32
            if (tccNum < SOC_EDMA3_NUM_DMACH)
            {
            // Enable the Event Interrupt
            	am335x_edma_enable_interrupt_event(controller, chNum);
                retVal = 1;
            }
            edma->PaRAM[chNum].opt &= EDMA3CC_OPT_TCC_CLR;
            edma->PaRAM[chNum].opt |= EDMA3CC_OPT_TCC_SET(tccNum);
        }
        else if (EDMA3_CHANNEL_TYPE_QDMA== chType)
        {
            // Interrupt channel nums are < 8
            if (tccNum < SOC_EDMA3_NUM_QDMACH)
            {
                // Enable the Event Interrupt
            	am335x_edma_enable_interrupt_event(controller, chNum);
                retVal = 1;
            }
            edma->PaRAM[chNum].opt &= EDMA3CC_OPT_TCC_CLR;
            edma->PaRAM[chNum].opt |= EDMA3CC_OPT_TCC_SET(tccNum);
        }
    }
    return retVal;
}

/**
 *  @brief    Free the specified channel (DMA/QDMA/Link) and its associated
 *            resources (PaRAM Set, TCC etc) and removes various mappings.
 *
 *  For Link channels, this API only frees the associated PaRAM Set.
 *
 *  For DMA/QDMA channels, it does the following operations:
 *  1) Disable any ongoing transfer on the channel,\n
 *  2) Remove the channel to Event Queue mapping,\n
 *  3) For DMA channels, clear the DCHMAP register, if available\n
 *  4) For QDMA channels, clear the QCHMAP register,\n
 *  5) Frees the DMA/QDMA channel in the end.\n
 *
 *  @param  baseAdd                  Memory address of the EDMA instance used.\n
 *
 *  @param  chtype              (DMA/QDMA) Channel
 *                     For Example: For QDMA it is,
 *                     EDMA3_CHANNEL_TYPE_QDMA.\n
 *
 *  @param  chNum                    This is the channel number requested for a
 *                      particular event.\n
 *
 *  @param  trigMode                 Mode of triggering start of transfer.\n
 *
 *  @param  tccNum                   The channel number on which the
 *                                   completion/error interrupt is generated.
 *                                   Not used if user requested for a Link
 *                                   channel.\n
 *
 *  @param  evtQNum                  Event Queue Number to which the channel
 *                                   will be unmapped (valid only for the
 *                                   Master Channel (DMA/QDMA) request).\n
 *
 *  trigMode can have values:
 *        EDMA3_TRIG_MODE_MANUAL\n
 *        EDMA3_TRIG_MODE_QDMA\n
 *        EDMA3_TRIG_MODE_EVENT
 *
 *  @return  TRUE if parameters are valid else return FALSE
 */
uint32_t am335x_edma_free_channel(enum am335x_edma3_controller controller,
									  uint32_t chType, uint32_t chNum,
									  uint32_t trigMode, uint32_t tccNum)
{
    uint32_t retVal = 0;
    if (chNum < SOC_EDMA3_NUM_DMACH)
    {
    	am335x_edma_disable_transfer(controller, chNum, trigMode);

    	// Disable the DMA channel in the shadow region specific register
    	am335x_edma_channel2shadow_disable(controller, chType, chNum);

        am335x_edma_channel2eventQ_demap(controller, chType, chNum);
        if (EDMA3_CHANNEL_TYPE_DMA == chType)
        {
            // Interrupt channel nums are < 32
            if (tccNum < SOC_EDMA3_NUM_DMACH)
            {
            // Disable the DMA Event Interrupt
            	am335x_edma_disable_interrupt_event(controller, chNum);
                retVal = 1;
            }
        }
        else if (EDMA3_CHANNEL_TYPE_QDMA== chType)
        {
            // Interrupt channel nums are < 8
            if (tccNum < SOC_EDMA3_NUM_QDMACH)
            {
                // Disable the QDMA Event Interrupt
            	am335x_edma_disable_interrupt_event(controller, chNum);
                retVal = 1;
            }
        }
    }
    return retVal;
}



/**
 *  @brief   Disable DMA transfer on the specified channel
 *
 *  There are multiple ways by which an EDMA3 transfer could be triggered.
 *  The triggering mode option allows choosing from the available triggering
 *  modes.
 *
 *  To disable a channel which was previously triggered in manual mode,
 *  this API clears the Secondary Event Register and Event Miss Register,
 *  if set, for the specific DMA channel.
 *
 *  To disable a channel which was previously triggered in QDMA mode, this
 *  API clears the QDMA Event Enable Register, for the specific QDMA channel.
 *
 *  To disable a channel which was previously triggered in event mode, this API
 *  clears the Event Enable Register, Event Register, Secondary Event Register
 *  and Event Miss Register, if set, for the specific DMA channel.
 *
 *
 *  @param  baseAdd         Memory address of the EDMA instance used.\n
 *
 *  @param  chNum           Channel being used to enable transfer.\n
 *
 *  @param  trigMode        Mode of triggering start of transfer (Manual,
 *                          QDMA or Event).\n
 *
 *  trigMode can have values:
 *        EDMA3_TRIG_MODE_MANUAL
 *        EDMA3_TRIG_MODE_QDMA
 *        EDMA3_TRIG_MODE_EVENT
 *
 *  @return  retVal         TRUE or FALSE depending on the param passed.\n
 *
 */
uint32_t am335x_edma_disable_transfer(enum am335x_edma3_controller controller,
		                                  uint32_t chNum,
		                                  uint32_t trigMode)
{
    uint32_t retVal = 0;
    switch (trigMode)
    {
        case EDMA3_TRIG_MODE_MANUAL :
            if (chNum < SOC_EDMA3_NUM_DMACH)
            {
            	am335x_edma_clear_event(controller, chNum);
                retVal = 1;
            }
        break;

        case EDMA3_TRIG_MODE_QDMA :
            if (chNum < SOC_EDMA3_NUM_QDMACH)
            {
            	am335x_edma_disable_qdma_event(controller, chNum);
                retVal = 1;
            }
        break;

        case EDMA3_TRIG_MODE_EVENT :
            if (chNum < SOC_EDMA3_NUM_DMACH)
            {
                //clear SECR & EMCR to clean any previous NULL request
            	am335x_edma3_clear_missed_event(controller, chNum);

                // Set EESR to enable event
            	am335x_edma_disable_dma_event(controller, chNum);
                retVal = 1;
            }
        break;

        default :
            retVal = 0;
        break;
    }
    return retVal;
}


/**
 *  @brief  Clears Event Register and Error Register for a specific
 *          DMA channel and brings back EDMA3 to its initial state.
 *
 *  This API clears the Event register, Event Miss register, Event Enable
 *  register for a specific DMA channel. It also clears the CC Error register.
 *
 *  @param  baseAdd         Memory address of the EDMA instance used.\n
 *
 *  @param  chNum           This is the channel number requested for a
 *                          particular event.\n
 *
 *  @param  evtQNum         Event Queue Number to which the channel
 *                          will be unmapped (valid only for the
 *                          Master Channel (DMA/QDMA) request).\n
 *
 *  @return none.
 */
void am335x_edma_clear_err_bits(enum am335x_edma3_controller controller,
					            uint32_t chNum,
					            uint32_t evtQNum)
{
	volatile struct am335x_edma_ctrl *edma = edma_ctrl[controller];

	if(chNum < SOC_EDMA3_NUM_DMACH)
	{
		 if(chNum < 32)
		 {
			edma->shadow_r[REGION_ID].eecr = (0x01u << chNum);
			// Write to EMCR to clear the corresponding EMR bit
			edma->emcr = (0x01u << chNum);
			// Clears the SER
			edma->shadow_r[REGION_ID].secr = (0x01u << chNum);
		 }
		 else
		 {
			edma->shadow_r[REGION_ID].eecrh = (0x01u << (chNum - 32));
			// Write to EMCR to clear the corresponding EMR bit
			edma->emcrh = (0x01u << (chNum - 32));
			// Clears the SER
			edma->shadow_r[REGION_ID].secrh = (0x01u << (chNum - 32));
		 }

	}

    // Clear the global CC Error Register
    if (0 == evtQNum)
    {
    	edma->ccerrclr &= (EDMA3CC_CCERRCLR_QTHRXCD0 | EDMA3CC_CCERRCLR_TCCERR);
    }
    else if(1 == evtQNum)
    {
    	edma->ccerrclr &= (EDMA3CC_CCERRCLR_QTHRXCD1 | EDMA3CC_CCERRCLR_TCCERR);
    }
    else if(2 == evtQNum)
    {
		edma->ccerrclr &= (EDMA3CC_CCERRCLR_QTHRXCD2 | EDMA3CC_CCERRCLR_TCCERR);
    }
    else if(3 == evtQNum)
    {
    	edma->ccerrclr &= (EDMA3CC_CCERRCLR_QTHRXCD2 | EDMA3CC_CCERRCLR_TCCERR);
    }
}

/**
 *  @brief   This API enables to evaluate the subsequent errors. On writing
 *           to the EEVAL register EDMA3CC error interrupt will be reasserted,
 *           if there are any outstanding error bits set due to subsequent
 *           error conditions.
 *
 *  @param  baseAddr         Memory address of the EDMA instance used.
 *
 *  @return  None
 */
void am335x_edma_eval_error(enum am335x_edma3_controller controller)
{
	volatile struct am335x_edma_ctrl *edma = edma_ctrl[controller];

    edma->eeval = EDMA3CC_EEVAL_EVAL << EDMA3CC_EEVAL_EVAL_SHIFT;
}

/**
 *  @brief   EDMA3 Deinitialization
 *
 *  This function deinitializes the EDMA3 Driver
 *  Clears the error specific registers (EMCR/EMCRh, QEMCR, CCERRCLR) &
 *  deinitialize the Queue Number Registers
 *
 *  @param  baseAdd         Memory address of the EDMA instance used.\n
 *
 *  @param  queNum          Event Queue used
 *
 *  @return  None
 *
 *  \note     The regionId is the shadow region(0 or 1) used and the,
 *            Event Queue used is either (0 or 1). There are only two shadow regions
 *            and only two event Queues
 */
void am335x_edma_deinit (enum am335x_edma3_controller controller)
{
	volatile struct am335x_edma_ctrl *edma = edma_ctrl[controller];

    uint32_t count = 0;

    // Disable the DMA (0 - 62) channels in the DRAE register
    edma->drae0 = EDMA3_CLR_ALL_BITS;
    edma->draeh0 = EDMA3_CLR_ALL_BITS;

    am335x_edma_clear_ccerr(controller, EDMA3CC_CLR_TCCERR);

    // Clear the Event miss Registers
    edma->emcr = EDMA3_SET_ALL_BITS;
    edma->emcrh = EDMA3_SET_ALL_BITS;

    // Clear CCERR register
    edma->ccerrclr = EDMA3_SET_ALL_BITS;

    // Deinitialize the Queue Number Registers
    for (count = 0;count < SOC_EDMA3_NUM_DMACH; count++)
    {
		edma->dmaqnum[count >> 3u] &= EDMA3CC_DMAQNUM_CLR(count);
    }

    for (count = 0;count < SOC_EDMA3_NUM_QDMACH; count++)
    {
        edma->qdmaqnum &= EDMA3CC_QDMAQNUM_CLR(count);
    }
}

/**
 *  @brief   This API can be used to save the register context for EDMA
 *
 *  @param  baseAdd         Memory address of the EDMA instance used.\n
 *
 *  @param  edmaCntxPtr     Pointer to the structure where the context
 *                          needs to be saved.
 *
 *  @return  None
 */
void am335x_edma_save_context(enum am335x_edma3_controller controller, struct edma_context *edmaCntxPtr)
{
	volatile struct am335x_edma_ctrl *edma = edma_ctrl[controller];

    uint32_t i;
    uint32_t maxPar;

    // Get the Channel mapping reg Val

    for(i = 0; i < 64; i++)
    {
         // All events are one to one mapped with the channels
         edmaCntxPtr->dchMap[i] = edma->qchmap[i];
    }

    // Get DMA Queue Number Register Val
    for(i=0; i < 8; i++)
    {
        edmaCntxPtr->dmaQNum[i] = edma->dmaqnum[i];
    }

    // Get the DMA Region Access Enable Register val
    edmaCntxPtr->regAccEnableLow = edma->drae0;
    edmaCntxPtr->regAccEnableHigh = edma->draeh0;

    // Get Event Set Register value
    edmaCntxPtr->eventSetRegLow  = edma->shadow_r[REGION_ID].esr;
    edmaCntxPtr->eventSetRegHigh = edma->shadow_r[REGION_ID].esrh;

    // Get Event Enable Set Register value
    edmaCntxPtr->enableEvtSetRegLow = edma->shadow_r[REGION_ID].eer;
    edmaCntxPtr->enableEvtSetRegHigh = edma->shadow_r[REGION_ID].eerh;

    // Get Interrupt Enable Set Register value
    edmaCntxPtr->intEnableSetRegLow =  edma->shadow_r[REGION_ID].ier;
    edmaCntxPtr->intEnableSetRegHigh =  edma->shadow_r[REGION_ID].ierh;

    maxPar = 256;

    for(i = 0; i < maxPar; i++)
    {
        // Get the  PaRAM  values
    	am335x_edma_get_param(controller, i,
                      (struct edma3cc_param_entry *)(&(edmaCntxPtr->dmaParEntry[i])));
    }
}

/**
 *  @brief   This API can be used to restore the register context for EDMA
 *
 *  @param  baseAdd         Memory address of the EDMA instance used.\n
 *
 *  @param  edmaCntxPtr     Pointer to the structure where the context
 *                          needs to be restored from.
 *
 *  @return  None
 */
void am335x_edma_restore_context(enum am335x_edma3_controller controller, struct edma_context *edmaCntxPtr)
{
	volatile struct am335x_edma_ctrl *edma = edma_ctrl[controller];

    uint32_t i;
    uint32_t maxPar;

    // set the Channel mapping reg Val
    for(i = 0; i < 64; i++)
    {
		// All events are one to one mapped with the channels
		edma->dchmap[i] = edmaCntxPtr->dchMap[i] ;
    }

    // set DMA Queue Number Register Val
    for(i=0; i < 8; i++)
    {
		edma->dmaqnum[i] = edmaCntxPtr->dmaQNum[i];
    }

    // set the DMA Region Access Enable Register val
    edma->drae0 = edmaCntxPtr->regAccEnableLow;
    edma->draeh0 = edmaCntxPtr->regAccEnableHigh;

    // set Event Set Register value
    edma->shadow_r[REGION_ID].esr = edmaCntxPtr->eventSetRegLow;
    edma->shadow_r[REGION_ID].esrh = edmaCntxPtr->eventSetRegHigh;

    // set Event Enable Set Register value
    edma->shadow_r[REGION_ID].eesr = edmaCntxPtr->enableEvtSetRegLow;
    edma->shadow_r[REGION_ID].eesrh = edmaCntxPtr->enableEvtSetRegHigh;

    // set Interrupt Enable Set Register value
    edma->shadow_r[REGION_ID].iesr = edmaCntxPtr->intEnableSetRegLow;
    edma->shadow_r[REGION_ID].iesrh = edmaCntxPtr->intEnableSetRegHigh;

    maxPar = 256;

    for(i = 0; i < maxPar; i++)
    {
        // Get the  PaRAM  values
    	am335x_edma3_set_param(controller, i, (struct edma3cc_param_entry *)(&(edmaCntxPtr->dmaParEntry[i])));
    }
}

/*
 *
 */
void am335x_edma_completion_isr()
{
    volatile uint32_t pendingIrqs;
    volatile bool is_IPR = false;

    uint32_t indexl;
    uint32_t Cnt = 0;

    indexl = 1;

    is_IPR = am335x_edma_get_interrupt_status(AM335X_EDMA3CC);

    if(is_IPR)
    {
        while ((Cnt < EDMA3CC_COMPL_HANDLER_RETRY_COUNT) && (indexl != 0u))
        {
            indexl = 0u;
            pendingIrqs = am335x_edma_get_interrupt_status(AM335X_EDMA3CC);

            while (pendingIrqs)
            {
                if((pendingIrqs & 1u) == 1u)
                {
                    /**
                    * If the user has not given any callback function
                    * while requesting the TCC, its TCC specific bit
                    * in the IPR register will NOT be cleared.
                    */

                    // here write to ICR to clear the corresponding IPR bits
                    am335x_edma_clear_interrupt(AM335X_EDMA3CC, indexl);
                    am335x_edma_callback (indexl, EDMA3_XFER_COMPLETE);
                }
                ++indexl;
                pendingIrqs >>= 1u;
            }
            Cnt++;
        }
    }
}

void am335x_edma_error_isr()
{
    volatile uint32_t pendingIrqs = 0u;
    volatile uint32_t evtqueNum = 0;  // Event Queue Num
    volatile uint32_t Cnt = 0u;
    volatile uint32_t index = 1u;
    volatile bool is_IPRH = false;
    volatile bool is_IPR = false;


    is_IPR  = am335x_edma_get_interrupt_status(AM335X_EDMA3CC);
    is_IPRH = am335_edma_get_interrupt_status_high(AM335X_EDMA3CC);

    if ((is_IPR | is_IPRH ) || (am335x_edma_get_interrupt_status(AM335X_EDMA3CC) != 0)
        					|| (am335x_edma_get_ccerr_status(AM335X_EDMA3CC) != 0))
    {
        /* Loop for EDMA3CC_ERR_HANDLER_RETRY_COUNT number of time,
         * breaks when no pending interrupt is found
         */
        while ((Cnt < EDMA3CC_ERR_HANDLER_RETRY_COUNT) && (index != 0u))
        {
            index = 0u;

            if(is_IPR)
            {
                   pendingIrqs = am335x_edma_get_err_interrupt_status(AM335X_EDMA3CC);
            }
            else
            {
                   pendingIrqs = am335_edma_get_interrupt_status_high(AM335X_EDMA3CC);
            }

            while (pendingIrqs)
            {
                   // Process all the pending interrupts
                   if(1u == (pendingIrqs & 1u))
                   {
                      // Write to EMCR to clear the corresponding EMR bits.

                        //Clear any SER
                        if(is_IPR)
                        {
                             am335x_edma3_clear_missed_event(AM335X_EDMA3CC, index);
                        }
                        else
                        {
                             am335x_edma3_clear_missed_event(AM335X_EDMA3CC, index + 32);
                        }
                   }
                   ++index;
                   pendingIrqs >>= 1u;
            }
            index = 0u;
            pendingIrqs = am335x_edma_get_err_interrupt_status(AM335X_EDMA3CC);
            while (pendingIrqs)
            {
                //Process all the pending interrupts
                if(1u == (pendingIrqs & 1u))
                {
                    // Here write to QEMCR to clear the corresponding QEMR bits
                    //Clear any QSER
                    am335x_edma3_clear_missed_event(AM335X_EDMA3CC, index);
                }
                ++index;
                pendingIrqs >>= 1u;
            }
            index = 0u;


            pendingIrqs = am335x_edma_get_ccerr_status(AM335X_EDMA3CC);
            if (pendingIrqs != 0u)
            {
            // Process all the pending CC error interrupts.
            // Queue threshold error for different event queues.
            for (evtqueNum = 0u; evtqueNum < SOC_EDMA3_NUM_EVQUE; evtqueNum++)
                {
                if((pendingIrqs & (1u << evtqueNum)) != 0u)
                {
                        // Clear the error interrupt.
                        am335x_edma_clear_ccerr(AM335X_EDMA3CC, (1u << evtqueNum));
                    }
                }

            // Transfer completion code error.
            if ((pendingIrqs & (1 << EDMA3CC_CCERR_TCCERR_SHIFT)) != 0u)
            {
                am335x_edma_clear_ccerr(AM335X_EDMA3CC, (0x01u << EDMA3CC_CCERR_TCCERR_SHIFT));
            }
                ++index;
            }
            Cnt++;
        }
    }

}

/********************************* End of file ******************************/
