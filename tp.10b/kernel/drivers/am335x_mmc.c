/*
 * MMC_driver.c
 *
 *  Created on: Jun 2, 2016
 *      Author: Romain Plouzin
 *
 *      abstract : driver for MMC peripheral
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "am335x_mmc.h"
#include "am335x_clock.h"
#include "am335x_mux.h"
#include "am335x_edma3.h"

/*
 * this structure describes the organization of MMC registers in memory
 */
struct mmc_ctrl {
	uint8_t  res0[0x110];
	uint32_t sysconfig;	//110
	uint32_t sysstatus;	//114
	uint32_t res1[3];	//118-120
	uint32_t csre;		//124
	uint32_t systest;	//128
	uint32_t con;		//12c
	uint32_t pwcnt;		//130
	uint32_t res2[51];	//134-1fC
	uint32_t sdmasa;	//200
	uint32_t blk;		//204
	uint32_t arg;		//208
	uint32_t cmd;		//20c
	uint32_t rsp[4];	//210-21c
	uint32_t data;		//220
	uint32_t pstate;	//224
	uint32_t hctl;		//228
	uint32_t sysctl;	//22c
	uint32_t stat;		//230
	uint32_t ie;		//234
	uint32_t ise;		//238
	uint32_t ac12;		//23c
	uint32_t capa;		//240
	uint32_t res3[1];	//244
	uint32_t cur_capa;	//248
	uint32_t res4[1];	//24c
	uint32_t fe;		//250
	uint32_t admaes;	//254
	uint32_t admasal;	//258
	uint32_t admasah;	//25c
	uint32_t res5[39];	//260-2f8
	uint32_t rev;		//2fc
};
// pointers for MMC register's base addresses
static struct mmc_ctrl* mmc_ctrl[] = {
	(struct mmc_ctrl*)(0x48060000),	// MMC0
	(struct mmc_ctrl*)(0x481d8000),	// MMC1
//	(struct mmc_ctrl*)(0x47810000),	// MMC2
};

// ----------------------------------------------------------------------------
// constants for MMC/SD registers
// ----------------------------------------------------------------------------
// MMC_SYSCONFIG register
#define SYCONFIG_STANDBYMODE_MASK 	(0x3<<12)

#define SYSCONFIG_CLKACTIVITY_NONE	(0x00<<8)
#define SYSCONFIG_CLKACTIVITY_INTER	(0x01<<8)
#define SYSCONFIG_CLKACTIVITY_FUNC	(0x02<<8)
#define SYSCONFIG_CLKACTIVITY_BOTH	(0x03<<8)


#define SYSCONFIG_SIDLEMODE_FORCE	(0x00<<3)
#define SYSCONFIG_SIDLEMODE_NONE	(0x01<<3)
#define SYSCONFIG_SIDLEMODE_SMART	(0x02<<3)
#define SYSCONFIG_SIDLEMODE_MASK	(0x03<<3)

#define SYSCONFIG_ENAWAKEUP		(0x1<<2)

#define SYSCONFIG_SOFTRESET		(0x1<<1)

#define SYSCONFIG_AUTOIDLE		(0x1<<0)

// MMC_SYSSTATUS registers
#define SYSSTATUS_RESETDONE		(0x1<<0)

// MMC_CON register
#define CON_INIT			(0x1<<1)

#define CON_MODE_SYSTEST		(0x1<<4)

#define CON_DW_1			(0x0<<5)
#define CON_DW_4			(0x0<<5)
#define CON_DW_8			(0x1<<5)

#define CON_CDP_ACTIVELOW		(0x1<<7)

#define CON_CTPL_DIS_ALL		(0x0<<11)
#define CON_CTPL_DIS			(0x1<<11)

#define CON_BOOT_ACK			(0x1<<17)
#define CON_BOOT_CF0			(0x1<<18)

// MMC SD_BLK register
#define SD_BLK_NBLK_1			(0x0001<<16)
#define SD_BLK_NBLK_2			(0x0002<<16)
#define SD_BLK_ALL			(0xffff<<16)

#define SD_BLK_BLEN_NONE		(0x000<<0)
#define SD_BLK_BLEN_1			(0x001<<0)
#define SD_BLK_BLEN_2			(0x002<<0)
#define SD_BLK_BLEN_3			(0x003<<0)
#define SD_BLK_BLEN_511			(0x1ff<<0)
#define SD_BLK_BLEN_512			(0x200<<0)
#define SD_BLK_BLEN_2047		(0x7ff<<0)
#define SD_BLK_BLEN_2048		(0x800<<0)

// MMC SD_CMD register
#define SD_CMD_TYPE_OTHER		(0x00<<22)
#define SD_CMD_TYPE_SUSPEND		(0x01<<22)
#define SD_CMD_TYPE_FUNCSEL		(0x02<<22)
#define SD_CMD_TYPE_ABORT		(0x03<<22)

#define SD_CMD_DP			(0x1<<21)

#define SD_CMD_CCCE			(0x1<<19)

#define SD_CMD_RSP_TYPE_NONE		(0x00<<16)
#define SD_CMD_RSP_TYPE_136B		(0x01<<16)
#define SD_CMD_RSP_TYPE_48B		(0x02<<16)
#define SD_CMD_RSP_TYPE_48B_BUSY	(0x03<<16)

#define SD_CMD_MSBS_SINGLE		(0x0<<5)
#define SD_CMD_MSBS_MULTIPLE		(0x1<<5)

#define SD_CMD_DDIR_WR			(0x0<<4)
#define SD_CMD_DDIR_RD			(0x1<<4)

#define SD_CMD_BCE 			(0x1<<1)

#define SD_CMD_DE 			(0x1<<0)

// MMC SD_HCTL register
#define SD_HCTL_VOLTAGE18		(0x5<<9)
#define SD_HCTL_VOLTAGE3		(0x6<<9)
#define SD_HCTL_VOLTAGE33		(0x7<<9)

#define SD_HCTL_SDBP			(0x1<<8)
#define SD_HCTL_CDSS 			(0x1<<7)
#define SD_HCTL_CDTL			(0x1<<6)
#define SD_HCTL_HSPE			(0x1<<2)
#define SD_HCTL_DTW			(0x1<<1)

// MMC SD_SYSCTL register
#define SD_SYSCTL_SRD			(0x1<<26)
#define SD_SYSCTL_SRC			(0x1<<25)
#define SD_SYSCTL_SRA			(0x1<<24)

#define SD_SYSCTL_DTO			(0x1 << 16)

#define SD_SYSCTL_CLKD2			(0x002<<6)
#define SD_SYSCTL_CLKD3			(0x003<<6)
#define SD_SYSCTL_CLKD1023		(0x3ff<<6)
#define SD_SYSCTL_CLKD_SHIFT		(6)

#define SD_SYSCTL_CEN 			(0x1<<2)
#define SD_SYSCTL_ICS			(0x1<<1)
#define SD_SYSCTL_ICE			(0x1<<0)

// MMC SD_STAT register
#define SD_STAT_BADA			(0x1<<29)
#define SD_STAT_CERR			(0x1<<28)
#define SD_STAT_ADMAE			(0x1<<25)
#define SD_STAT_DEB			(0x1<<22)
#define SD_STAT_DCRC			(0x1<<21)
#define SD_STAT_DTO			(0x1<<20)
#define SD_STAT_CEB			(0x1<<18)
#define SD_STAT_CCRC			(0x1<<17)
#define SD_STAT_CTO			(0x1<<16)
#define SD_STAT_ERRI			(0x1<<15)
#define SD_STAT_CREM			(0x1<<7)
#define SD_STAT_CINS			(0x1<<6)
#define SD_STAT_BRR			(0x1<<5)
#define SD_STAT_BWR			(0x1<<4)
#define SD_STAT_TC 			(0x1<<1)
#define SD_STAT_CC			(0x1<<0)

// MMC SD_PSTATE register
#define SD_PSTATE_CINS			(0x1<<16)
#define SD_PSTATE_RDENABLE		(0x1<<11)
#define SD_PSTATE_WRENABLE		(0x1<<10)

 // MMC CAPA register
 #define SD_CAPA_BIT64			(0x1<<28)
 #define SD_CAPA_VS18			(0x1<<26)
 #define SD_CAPA_VS30 			(0x1<<25)
 #define SD_CAPA_VS33			(0x1<<24)
 #define SD_CAPA_SRS			(0x1<<23)
 #define SD_CAPA_DS			(0x1<<22)
 #define SD_CAPA_HSS			(0x1<<21)
 #define SD_CAPA_AD2S			(0x1<<19)

#define SD_CAPA_MBL_512			(0x0<<16)
#define SD_CAPA_MBL_1024		(0x1<<16)
#define SD_CAPA_MBL_2048		(0x2<<16)
#define SD_CAPA_MBL_MASK		(0x3<<16)

#define SD_CAPA_BCF(x)			((x)<<8)
#define SD_CAPA_BCF_MASK		(0x3f<<8)

#define SD_CAPA_TCU			(0x1<<7)
#define SD_CAPA_TCF(x)			((x)<<0)
#define SD_CAPA_TCF_MASK		(0x3f<<0)

// MMC IE register
#define SD_IE_CC_ENABLE			(0x1<<0)
#define SD_IE_TC_ENABLE			(0x1<<1)
#define SD_IE_BRR_ENABLE		(0x1<<5)
#define SD_IE_DTO_ENABLE		(0x1<<16)
#define SD_IE_CTO_ENABLE		(0x1<<20)

// MMC ISE register
#define SD_ISE_CC_SIGEN			(0x1<<0)
#define SD_ISE_TC_SIGEN			(0x1<<1)
#define SD_ISE_DTO_SIGEN		(0x1<<20)
#define SD_ISE_CTO_SIGEN		(0x1<<16)

// ----------------------------------------------------------------------------
// Constants for MMC/SD driver App level
// ----------------------------------------------------------------------------
#define DATA_RESPONSE_WIDTH		(512)

/* Command/Response flags for notifying some information to controller */
#define SD_CMDRSP_NONE			(0x1<<0)
#define SD_CMDRSP_STOP			(0x1<<1)
#define SD_CMDRSP_FS			(0x1<<2)
#define SD_CMDRSP_ABORT			(0x1<<3)
#define SD_CMDRSP_BUSY			(0x1<<4)
#define SD_CMDRSP_136BITS		(0x1<<5)
#define SD_CMDRSP_DATA			(0x1<<6)
#define SD_CMDRSP_READ			(0x1<<7)
#define SD_CMDRSP_WRITE			(0x1<<8)

/* Check pattern that can be used for card response validation */
#define SD_CHECK_PATTERN   		0xaa

/* SD voltage enumeration as per VHS field of the interface command */
#define SD_VOLT_2P7_3P6			(0x000100u)

/* SD OCR register definitions */
/* High capacity */
#define SD_OCR_HIGH_CAPACITY    	(0x1<<30)
/* Voltage */
#define SD_OCR_VDD_2P7_2P8		(0x1<<15)
#define SD_OCR_VDD_2P8_2P9		(0x1<<16)
#define SD_OCR_VDD_2P9_3P0		(0x1<<17)
#define SD_OCR_VDD_3P0_3P1		(0x1<<18)
#define SD_OCR_VDD_3P1_3P2		(0x1<<19)
#define SD_OCR_VDD_3P2_3P3		(0x1<<20)
#define SD_OCR_VDD_3P3_3P4		(0x1<<21)
#define SD_OCR_VDD_3P4_3P5		(0x1<<22)
#define SD_OCR_VDD_3P5_3P6		(0x1<<23)
/* This is for convenience only. Sets all the VDD fields */
#define SD_OCR_VDD_WILDCARD		(0x1ff << 15)


// ----------------------------------------------------------------------------
// Structure for card commands 
// ----------------------------------------------------------------------------
typedef struct _mmc_cmd {
	uint32_t flags;
	uint32_t indx;
	uint32_t arg;
	uint32_t nblks;
	uint32_t rsp[4];
	uint8_t *data;
} mmc_cmd_t;

// -- macros ------------------------------------------------------------------

#define CMD(x)				(x<<24)	// to fit INDX field of CMD register
#define DATA_TIMEOUT(x)			((((x) - 13) & 0xf) << 16)
#define CMD_SET(cmd, type, restype, rw)	(cmd | type | restype | rw )

/* Check RCA/status */
#define SD_RCA_ADDR(rca)		((rca & 0xffff0000) >> 16)
#define SD_RCA_STAT(rca)		(rca & 0xffff)

#define SD_CARD_VERSION(sdcard)		((sdcard)->raw_scr[0] & 0xf)
#define SD_CARD_BUSWIDTH(sdcard)	(((sdcard)->raw_scr[0] & 0xf00) >> 8)
#define SD_CARD_CSD_VERSION(crd) 	(((crd)->raw_csd[3] & 0xc0000000) >> 30)

#define SD_CSD0_DEV_SIZE(csd3, csd2, csd1, csd0) 	(((csd2 & 0x000003ff) << 2) | ((csd1 & 0xc0000000) >> 30))
#define SD_CSD0_MULT(csd3, csd2, csd1, csd0) 		((csd1 & 0x00038000) >> 15)
#define SD_CSD0_RDBLKLEN(csd3, csd2, csd1, csd0) 	((csd2 & 0x000f0000) >> 16)
#define SD_CSD0_TRANSPEED(csd3, csd2, csd1, csd0) 	((csd3 & 0x000000ff) >> 0)

#define SD_CARD0_DEV_SIZE(crd) 		SD_CSD0_DEV_SIZE((crd)->raw_csd[3], (crd)->raw_csd[2], (crd)->raw_csd[1], (crd)->raw_csd[0])
#define SD_CARD0_MULT(crd) 		SD_CSD0_MULT((crd)->raw_csd[3], (crd)->raw_csd[2], (crd)->raw_csd[1], (crd)->raw_csd[0])
#define SD_CARD0_RDBLKLEN(crd) 		SD_CSD0_RDBLKLEN((crd)->raw_csd[3], (crd)->raw_csd[2], (crd)->raw_csd[1], (crd)->raw_csd[0])
#define SD_CARD0_TRANSPEED(crd) 	SD_CSD0_TRANSPEED((crd)->raw_csd[3], (crd)->raw_csd[2], (crd)->raw_csd[1], (crd)->raw_csd[0])
#define SD_CARD0_NUMBLK(crd) 		((SD_CARD0_DEV_SIZE((crd)) + 1) * (1 << (SD_CARD0_MULT((crd)) + 2)))
#define SD_CARD0_SIZE(crd) 		((SD_CARD0_NUMBLK((crd))) * (1 << (SD_CARD0_RDBLKLEN(crd))))

#define SD_CSD1_DEV_SIZE(csd3, csd2, csd1, csd0) 	(((csd2 & 0x0000003f) << 16) | ((csd1 & 0xffff0000) >> 16))
#define SD_CSD1_RDBLKLEN(csd3, csd2, csd1, csd0) 	((csd2 & 0x000f0000) >> 16)
#define SD_CSD1_TRANSPEED(csd3, csd2, csd1, csd0) 	((csd3 & 0x000000ff) >> 0)

#define SD_CARD1_DEV_SIZE(crd)		SD_CSD1_DEV_SIZE((crd)->raw_csd[3], (crd)->raw_csd[2], (crd)->raw_csd[1], (crd)->raw_csd[0])
#define SD_CARD1_RDBLKLEN(crd)		SD_CSD1_RDBLKLEN((crd)->raw_csd[3], (crd)->raw_csd[2], (crd)->raw_csd[1], (crd)->raw_csd[0])
#define SD_CARD1_TRANSPEED(crd)		SD_CSD1_TRANSPEED((crd)->raw_csd[3], (crd)->raw_csd[2], (crd)->raw_csd[1], (crd)->raw_csd[0])
#define SD_CARD1_SIZE(crd) 		((SD_CARD1_DEV_SIZE((crd)) + 1) * (512 * 1024))


/* =================================================================
 *				    global variables definitions
 * =================================================================
 */

enum card_opers {WRITE_OPER, READ_OPER};
enum card_types {CARD_SD, CARD_EMMC, CARD_SDIO};


#define SOC_CACHELINE_SIZE	512
static uint32_t data_buffer[3][DATA_RESPONSE_WIDTH] __attribute__((aligned(SOC_CACHELINE_SIZE))) __attribute__((section(".dmasafe")));

/* Structure for SD Card information */
struct mmc_card_info {
	enum am335x_mmc_controllers controller;
	enum card_types card_type;
	uint32_t rca;
	uint32_t raw_scr[2];
	uint32_t raw_csd[4];
	uint32_t raw_cid[4];
	uint32_t blk_len;
	uint32_t n_blks;
	uint64_t size;
	uint8_t  sd_ver;
	uint8_t  card_bus_width;
	uint8_t  tran_speed;
	bool     high_capacity;

	volatile uint32_t error_flag;
	volatile bool flag_transfer_completed;
	volatile bool flag_command_completed;
	volatile bool data_timeout;
	volatile bool cmd_timeout;
	volatile bool callback_occured;

	volatile bool dma_enabled;
	uint32_t dma_tx_channel;	
	uint32_t dma_rx_channel;	

	uint32_t* data_buffer;
};

static struct mmc_card_info cards[] = {
	[0] = {	.controller = AM335X_MMC0, 
		.dma_tx_channel = 24,	
		.dma_rx_channel = 25,	
		.data_buffer = data_buffer[0],
	      },
	[1] = {.controller = AM335X_MMC1, 
		.dma_tx_channel = 2,	
		.dma_rx_channel = 3,	
		.data_buffer = data_buffer[1],
	      },
/*	[2] = {.controller = AM335X_MMC2, 
		.dma_tx_channel = 1,	
		.dma_rx_channel = 2,	
		.data_buffer = data_buffer[2],
	      },
*/
};

// -----------------------------------------------------------------
//             end of global variables definitions
// -----------------------------------------------------------------

// macro to print error string if x is false
#define pr_err(x,y...) do { if (x==0) printf (y); } while(0)

// ----------------------------------------------------------------------------
// private methods
// ----------------------------------------------------------------------------

int mmc_reset (enum am335x_mmc_controllers ctrl)
{
	volatile struct mmc_ctrl *mmc = mmc_ctrl[ctrl];

	// reset MMC controller
	mmc->sysconfig |= SYSCONFIG_SOFTRESET;	
		
	// wait until soft reset insn't done
	uint32_t retry = 0xffff;
	while(((--retry) != 0) && ((mmc->sysstatus & SYSSTATUS_RESETDONE) == 0)); 

	pr_err(retry,"Unable to perform a software reset of the MMC controller\n");
	return (retry != 0) ? 0 : -1;
}

/* -------------------------------------------------------------------------- */

int mmc_reset_line (enum am335x_mmc_controllers ctrl, uint32_t lines)
{
	volatile struct mmc_ctrl *mmc = mmc_ctrl[ctrl];

	// reset the requested lines
	mmc->sysctl |= lines;

	// wait until reset is performed
	uint32_t retry = 0xffff;
	while (((--retry) != 0) && ((mmc->sysctl & lines) == lines));

	pr_err(retry,"Unable to perform a software reset of the requested lines of the MMC controller\n");
	return (retry != 0) ? 0 : -1;
}

/* -------------------------------------------------------------------------- */

int mmc_set_bus_freq (enum am335x_mmc_controllers ctrl, uint32_t clk_in, uint32_t clk_out)
{
	volatile struct mmc_ctrl *mmc = mmc_ctrl[ctrl];

	// enable internal clock
	mmc->sysctl |= SD_SYSCTL_ICE; 
	// make sure that internal clock is stable
	uint32_t retry = 0xffff;
	while (((--retry) != 0) && ((mmc->sysctl & SD_SYSCTL_ICS) != SD_SYSCTL_ICS)); 

	// reset the register
	mmc->sysctl &= ~SD_SYSCTL_CLKD1023;

	// set the divider
	uint32_t divider = clk_in / clk_out;

	// program the clock divider
	mmc->sysctl |= divider << SD_SYSCTL_CLKD_SHIFT;

	// enable clock to the card
	mmc->sysctl |= SD_SYSCTL_CEN;

	pr_err(retry,"Unable to set the bus frequency of the MMC controller\n");
	return (retry != 0) ? 0 : -1;
}

/* -------------------------------------------------------------------------- */

static int mmc_set_bus_width (enum am335x_mmc_controllers ctrl, uint32_t bus_width)
{
	volatile struct mmc_ctrl *mmc = mmc_ctrl[ctrl];

	switch (bus_width) {
	case 1:
		mmc->con  &= ~CON_DW_8; // clear the byte for SD cards
		mmc->hctl &= ~SD_HCTL_DTW;
		break;
	
	case 4:
		mmc->con  &= ~CON_DW_8; // clear the byte for SD cards
		mmc->hctl &= ~SD_HCTL_DTW;
		break;

	case 8:
		mmc->con  |= CON_DW_8;	// for MMC only
		mmc->hctl &= ~SD_HCTL_DTW;
		break;

	default: 
		pr_err(0,"invalid bus width of the MMC controller\n");
		return -1;
	}

	return 0;
}

/* -------------------------------------------------------------------------- */

static int mmc_set_bus_voltage (enum am335x_mmc_controllers ctrl, uint32_t supported_voltage, uint32_t voltage)
{
	volatile struct mmc_ctrl *mmc = mmc_ctrl[ctrl];

	// init register
	mmc->capa &= ~(SD_CAPA_VS18 | SD_CAPA_VS30 | SD_CAPA_VS33);
	// set the supported voltage (3V & 1.8V)
	mmc->capa |= supported_voltage;
	// reset the register
	mmc->hctl &= ~SD_HCTL_VOLTAGE33;
	// set the value
	mmc->hctl |= voltage;

	return 0;
}

/* -------------------------------------------------------------------------- */

static int mmc_set_module_idle_mode(enum am335x_mmc_controllers ctrl)
{
	volatile struct mmc_ctrl *mmc = mmc_ctrl[ctrl];

    	mmc->sysconfig &= ~(SYCONFIG_STANDBYMODE_MASK | SYSCONFIG_CLKACTIVITY_BOTH
                        | SYSCONFIG_SIDLEMODE_MASK | SYSCONFIG_ENAWAKEUP
                        | SYSCONFIG_AUTOIDLE);
    	mmc->sysconfig |= SYSCONFIG_AUTOIDLE; // auto idle on

	return 0;
}

/* -------------------------------------------------------------------------- */

static int mmc_set_bus_power(enum am335x_mmc_controllers ctrl, bool on)
{
	volatile struct mmc_ctrl *mmc = mmc_ctrl[ctrl];

	if (on)
		mmc->hctl |= SD_HCTL_SDBP;	// enable bus power
	else
		mmc->hctl &= ~SD_HCTL_SDBP;

	return 0;
}

/* -------------------------------------------------------------------------- */

static int mmc_send_init_frame (enum am335x_mmc_controllers ctrl)
{
	volatile struct mmc_ctrl *mmc = mmc_ctrl[ctrl];
	volatile struct mmc_card_info* card = &cards[ctrl];

	// Enable interrupt flag bit for CC
	mmc->ie  |= SD_IE_CC_ENABLE;	
	mmc->ise |= SD_ISE_CC_SIGEN;

	// request an init stream
	mmc->con |= CON_INIT;
	mmc->cmd  = CMD(0);

	// Make sure the command is completed
	uint32_t retry  = 0xffff;
	while (((--retry) != 0) && ((mmc->stat & SD_STAT_CC) != SD_STAT_CC) && !card->flag_command_completed); 

	// reset the flags
	mmc->con  &= ~CON_INIT;
	mmc->stat |= SD_STAT_CC; 
	card->flag_command_completed = false;

	pr_err(retry, "The initializing frame was not sent properly\n");
	return (retry != 0) ? 0 : -1;
}

/* -------------------------------------------------------------------------- */

// mandatory w/ or without interrupt mechanism
static int mmc_interrupt_enable (enum am335x_mmc_controllers ctrl)
{
	volatile struct mmc_ctrl *mmc = mmc_ctrl[ctrl];

	mmc->ie |= (SD_IE_TC_ENABLE | SD_IE_DTO_ENABLE | SD_IE_CTO_ENABLE | SD_IE_BRR_ENABLE);

#ifdef INTC_EN
	mmc->ise |= (SD_ISE_TC_SIGEN | SD_ISE_DTO_SIGEN | SD_ISE_CTO_SIGEN);
#endif
	return 0;
}

/* -------------------------------------------------------------------------- */

static void mmc_set_data_timeout (enum am335x_mmc_controllers ctrl, uint32_t timeout)
{
	volatile struct mmc_ctrl *mmc = mmc_ctrl[ctrl];

 	mmc->sysctl &= ~SD_SYSCTL_DTO;
	mmc->sysctl |= timeout;
}

/* -------------------------------------------------------------------------- */

static void mmc_command_send(
	enum am335x_mmc_controllers ctrl,
	uint32_t cmd, 
	uint32_t cmdarg, 
	bool has_data,
	uint32_t nblks)
{
	volatile struct mmc_ctrl *mmc = mmc_ctrl[ctrl];
	volatile struct mmc_card_info* card = &cards[ctrl];

	if (has_data) {
		cmd |= (SD_CMD_DP | SD_CMD_MSBS_MULTIPLE | SD_CMD_BCE);
	}

	if (card->dma_enabled) {
		cmd |= SD_CMD_DE;
	}

	// Set the block information; block length is specified separately
	mmc->blk &= ~SD_BLK_ALL;
	mmc->blk |= nblks << 16;

	// Set the command/command argument
	mmc->arg = cmdarg;
	mmc->cmd = cmd;
}

/* -------------------------------------------------------------------------- */

static int mmc_transfer_get_status (enum am335x_mmc_controllers ctrl)
{
	volatile struct mmc_ctrl *mmc = mmc_ctrl[ctrl];
	struct mmc_card_info* card = &cards[ctrl];

#ifndef INTC_EN
	// try to get the TC flag
	uint32_t retry = 0xffff;
	while (((--retry) != 0) && ((mmc->stat & SD_STAT_TC) != SD_STAT_TC)); 
	// if transfer is completed, get data
	if (retry!=0) {
		mmc->stat |= SD_STAT_TC;	// reset the flag
	}

	return (retry != 0) ? 0 : -1;
#else
	int status = 0;
	while (!card->flag_transfer_completed && !card->data_timeout);
	
	card->flag_transfer_completed = false;

	if (card->data_timeout) {
		status  = -1;
		card->data_timeout = false;
	}

	if ((mmc->cmd & SD_CMD_DP) != 0) {
		uint32_t retry = 0xffff;
		while ((card->callback_occured == 0) && ((retry--) != 0));
		card->callback_occured = 0;

		if(retry == 0) {
			status = -1;
		}
	}

	card->dma_enabled = false;
	return status;
#endif
}

/* -------------------------------------------------------------------------- */

static int mmc_command_get_status (enum am335x_mmc_controllers ctrl)
{
	struct mmc_card_info* card = &cards[ctrl];
	int status = 0;

#ifdef INTC_EN
	while(!card->flag_command_completed && !card->cmd_timeout) {	
		// sometimes, a TC flag may come before CC flag.
		// this is to prevent infinite loop
		if (card->flag_transfer_completed) 
			mmc_transfer_get_status(ctrl);
	}

	card->flag_command_completed = false;
	status = card->cmd_timeout ? -1 : 0;
	card->cmd_timeout = false;

	return status;
#else
	volatile struct mmc_ctrl *mmc = mmc_ctrl[ctrl];

	uint32_t retry = 0xffff;
	while (((--retry) != 0) && ((mmc->stat & SD_STAT_CC) != SD_STAT_CC)); 

	return (retry != 0) ? 0 : -1;

#endif
}

/* -------------------------------------------------------------------------- */

static void mmc_get_response (enum am335x_mmc_controllers ctrl, uint32_t *rsp)
{
	volatile struct mmc_ctrl *mmc = mmc_ctrl[ctrl];

	for (int i = 0; i <= 3; i++) {
		rsp[i] = mmc->rsp[i];
	}
}

/* -------------------------------------------------------------------------- */

static int mmc_hs_cmd_send (enum am335x_mmc_controllers ctrl, mmc_cmd_t *cmd)
{
	volatile struct mmc_ctrl *mmc = mmc_ctrl[ctrl];

	uint32_t cmdType = SD_CMD_TYPE_OTHER;
	if (cmd->flags & SD_CMDRSP_STOP) {
		cmdType = SD_CMD_TYPE_SUSPEND;	
	} else if (cmd->flags & SD_CMDRSP_FS) {
		cmdType = SD_CMD_TYPE_FUNCSEL;
	} else if (cmd->flags & SD_CMDRSP_ABORT) {
		cmdType = SD_CMD_TYPE_ABORT;
	}

	uint32_t cmdDir = (cmd->flags & SD_CMDRSP_READ) ? SD_CMD_DDIR_RD : SD_CMD_DDIR_WR;
	bool has_data = (cmd->flags & SD_CMDRSP_DATA) != 0;
	uint32_t nblks = (has_data) ? cmd->nblks : 0;	// get nblks

	uint32_t rspType = SD_CMD_RSP_TYPE_48B;
	if (cmd->flags & SD_CMDRSP_NONE) {
		rspType = SD_CMD_RSP_TYPE_NONE;
	} else if (cmd->flags & SD_CMDRSP_136BITS) {
		rspType = SD_CMD_RSP_TYPE_136B;
	} else if (cmd->flags & SD_CMDRSP_BUSY) {
		rspType = SD_CMD_RSP_TYPE_48B_BUSY;
	}

	uint32_t command = CMD_SET(cmd->indx, cmdType, rspType, cmdDir);

	if (has_data) {
		mmc->stat |= SD_STAT_TC;
		mmc_set_data_timeout (ctrl, DATA_TIMEOUT(27));
	}

	// transmit command to the card
	mmc_command_send (ctrl, command, cmd->arg, has_data, nblks);

	// get command status
	int status = mmc_command_get_status(ctrl);

	if (status == 0) {
		mmc->stat |= SD_STAT_CC; // clear the flag
		mmc_get_response(ctrl, cmd->rsp);
	}

	return status;
}

/* -------------------------------------------------------------------------- */

static int mmc_card_reset (enum am335x_mmc_controllers ctrl)
{
	// CMD0 is the reset command
	mmc_cmd_t cmd = {.indx=CMD(0), .flags=SD_CMDRSP_NONE, .arg=0, };
	int err = mmc_hs_cmd_send(ctrl, &cmd);

	return err;
}


/* -------------------------------------------------------------------------- */

static int mmc_app_cmd_send (enum am335x_mmc_controllers ctrl, mmc_cmd_t *cmd)
{
	// application cmd must be preceeded by a CMD55
	mmc_cmd_t capp = { .indx=CMD(55), .flags=0, .arg=cards[ctrl].rca<<16, };
	int err = mmc_hs_cmd_send(ctrl, &capp);

	if (err==0) err = mmc_hs_cmd_send(ctrl, cmd);

	return err;
}

/* -------------------------------------------------------------------------- */

static int mmc_type_card_check (enum am335x_mmc_controllers ctrl)
{
    /*
     * Card type can be found by sending CMD55. If the card responds,
     * it is a SD card. Else, we assume it is a MMC Card
     */
    mmc_cmd_t cmd = { .indx=CMD(55), .flags=0, .arg=0, };
    int status = mmc_app_cmd_send(ctrl, &cmd);

    return status;
}

/* -------------------------------------------------------------------------- */

static void mmc_set_block_length(enum am335x_mmc_controllers ctrl, uint32_t blk_len)
{
	volatile struct mmc_ctrl *mmc = mmc_ctrl[ctrl];

	mmc->blk |= SD_BLK_BLEN_NONE;
	mmc->blk |= blk_len;
}

/* -------------------------------------------------------------------------- */

static int mmc_send_stop (enum am335x_mmc_controllers ctrl)
{
	mmc_cmd_t cmd = { .indx=CMD(12), .flags=SD_CMDRSP_BUSY, .arg=0, };
	int err = mmc_app_cmd_send(ctrl, &cmd);

	if (err==0) err = mmc_transfer_get_status(ctrl);

	return err;
}

/* -------------------------------------------------------------------------- */

static int mmc_send_status(enum am335x_mmc_controllers ctrl, unsigned int *status)
{
	struct mmc_card_info *card = &cards[ctrl];
	mmc_cmd_t cmd = { .indx=CMD(13), .flags=0, .arg= card->rca << 16, };

	int err = mmc_hs_cmd_send(ctrl, &cmd);

	*status = cmd.rsp[0];

	return err;
}

/* -------------------------------------------------------------------------- */

static int mmc_switch(enum am335x_mmc_controllers ctrl, char set, char index, char value)
{
	#define MMC_SWITCH_MODE_WRITE_BYTE 3
	mmc_cmd_t cmd = { 
		.indx=CMD(6), 
		.flags=SD_CMDRSP_BUSY, 
		.arg= (MMC_SWITCH_MODE_WRITE_BYTE << 24) | (index << 16) | (value << 8) | set, 
	};

	int err = mmc_hs_cmd_send(ctrl, &cmd);

	if (err != 0) return err;

    /* Must check status to be sure of no errors */
	unsigned int status = 0;
    do
    {
       err = mmc_send_status(ctrl, &status);

        if(status & ((1<<7)))
        {
        	// switch error
        	//UARTPuts("switch error\n", -1);
        	return -1;
        }

        if(status & ((1<<22)))
	   {
        	// Illegal command
        	//UARTPuts("Illegal command\n", -1);
        	return -1;
	   }

    } while (((status & (1<<8))==0) && (err!= 0)); // run while the card is not ready


    return 0;
}


/* -------------------------------------------------------------------------- */

static void mmc_rx_edma_setup(
	enum am335x_mmc_controllers ctrl,
	void *ptr,
	unsigned int blkSize,
	unsigned int nblks)
{
	volatile struct mmc_ctrl *mmc = mmc_ctrl[ctrl];
	struct mmc_card_info *card = &cards[ctrl];

	struct edma3cc_param_entry param = {
		.srcAddr    = (unsigned int)&mmc->data,
		.destAddr   = (unsigned int)ptr,
		.srcBIdx    = 0,
		.srcCIdx    = 0,
		.destBIdx   = 4,
		.destCIdx   = (unsigned short)blkSize,
		.aCnt       = 0x4,
		.bCnt       = (unsigned short)blkSize/4,
		.cCnt       = (unsigned short)nblks,
		.bCntReload = 0x0,
		.linkAddr   = 0xffff,
		.opt        = 0,
	};

	// Set OPT 
	param.opt |= ((card->dma_rx_channel << EDMA3CC_OPT_TCC_SHIFT) & EDMA3CC_OPT_TCC);

	// Transmission complition interrupt enable 
	param.opt |= (1 << EDMA3CC_OPT_TCINTEN_SHIFT);

	// Read FIFO : SRC Constant addr mode 
	param.opt |= (1 << 0);

	// SRC FIFO width is 32 bit 
	param.opt |= (2 << 8);

	// AB-Sync mode 
	param.opt |= (1 << 2);

	// configure PaRAM Set 
	am335x_edma3_set_param (AM335X_EDMA3CC, card->dma_rx_channel, &param);

	// Enable the transfer 
	am335x_edma3_enable_transfer(AM335X_EDMA3CC, card->dma_rx_channel, AM335X_EDMA3_TRIG_MODE_EVENT);
}

/* -------------------------------------------------------------------------- */

static void mmc_tx_edma_setup(
	enum am335x_mmc_controllers ctrl,
	void *ptr,
	unsigned int blkSize,
	unsigned int blks)
{
	volatile struct mmc_ctrl *mmc = mmc_ctrl[ctrl];
	struct mmc_card_info *card = &cards[ctrl];

	struct edma3cc_param_entry param = {
		.srcAddr    = (unsigned int)ptr,
		.destAddr   = (unsigned int)&mmc->data,
		.srcBIdx    = 4,
		.srcCIdx    = blkSize,
		.destBIdx   = 0,
		.destCIdx   = 0,
		.aCnt       = 0x4,
		.bCnt       = (unsigned short)blkSize/4,
		.cCnt       = (unsigned short)blks,
		.bCntReload = 0x0,
		.linkAddr   = 0xffff,
		.opt        = 0,
	};

	// Set OPT 
	param.opt |= ((card->dma_tx_channel << EDMA3CC_OPT_TCC_SHIFT) & EDMA3CC_OPT_TCC);

	// Transmission completion interrupt enable 
	param.opt |= (1 << EDMA3CC_OPT_TCINTEN_SHIFT);

	// Read FIFO : DST Constant addr mode 
	param.opt |= (1 << 1);

	// DST FIFO width is 32 bit 
	param.opt |= (2 << 8);

	// AB-Sync mode 
	param.opt |= (1 << 2);

	// configure PaRAM Set 
	am335x_edma3_set_param(AM335X_EDMA3CC, card->dma_tx_channel, &param);

	// Enable the transfer 
	am335x_edma3_enable_transfer(AM335X_EDMA3CC, card->dma_tx_channel, AM335X_EDMA3_TRIG_MODE_EVENT);
}

/* -------------------------------------------------------------------------- */

static int mmc_transfer_setup (
	enum am335x_mmc_controllers ctrl, 
	enum card_opers oper, 
	void *ptr, 
	uint16_t nblks)
{
	struct mmc_card_info *card = &cards[ctrl];

	if (oper == READ_OPER) {	
		// configure edma to receive data
		mmc_rx_edma_setup (ctrl, ptr, card->blk_len, nblks);
	} else {	
		// configure edma to send data
		mmc_tx_edma_setup (ctrl, ptr, card->blk_len, nblks);
	}

	// dma is enabled
	card->dma_enabled = true;

	// set card's block length
	mmc_set_block_length(ctrl, card->blk_len);

	return 0;
}

// --------------------------------------------------------------------------

static void mmc_edma3_callback (void* param)
{
	enum am335x_mmc_controllers ctrl = (enum am335x_mmc_controllers)param;
	struct mmc_card_info *card = &cards[ctrl];
	card->callback_occured = true;
}

/* -------------------------------------------------------------------------- */

static int mmc_init_sdcard (enum am335x_mmc_controllers ctrl)
{
	struct mmc_card_info *card = &cards[ctrl];
	mmc_cmd_t cmd = { .flags = 0, };

	int err = 0;
	card->card_type = CARD_SD;

	// CMD0 - reset card 
	err = mmc_card_reset(ctrl);
	if (err != 0) return err;

	// CMD8 - send oper voltage 
	cmd.indx = CMD(8);
	cmd.flags = 0;
	cmd.arg = (SD_CHECK_PATTERN | SD_VOLT_2P7_3P6);
	if (err==0) err = mmc_hs_cmd_send(ctrl, &cmd);
	if (err != 0) {
		/* If the cmd fails, it can be due to version < 2.0, since
		 * we are currently supporting high voltage cards only
		 */
		err = 0;
	}

	// Go ahead and send ACMD41, with host capabilities 
	cmd.indx = CMD(41);
	cmd.flags = 0;
	cmd.arg = SD_OCR_HIGH_CAPACITY | SD_OCR_VDD_WILDCARD;
	if(err==0) err = mmc_app_cmd_send(ctrl, &cmd);

	// Poll until we get the card status (BIT31 of OCR) is powered up 
	uint32_t retry = 0xffff;
	while (err==0) {
		mmc_cmd_t cmd = {
			.indx  = CMD(41), 
			.flags = 0,
			.arg   = SD_OCR_HIGH_CAPACITY | SD_OCR_VDD_WILDCARD, 
		};
		err = mmc_app_cmd_send(ctrl, &cmd);

		if ((cmd.rsp[0] & (0x1<<31)) != 0) {
			card->high_capacity = (cmd.rsp[0] & SD_OCR_HIGH_CAPACITY) != 0;
			break;
		}

		retry--;
		if (retry == 0) err = -1;
	}

	// Send CMD2, to get the card identification register 
	cmd.indx = CMD(2);
	cmd.flags = SD_CMDRSP_136BITS;
	cmd.arg = 0;
	if (err==0) err = mmc_hs_cmd_send(ctrl, &cmd);
	memcpy(card->raw_cid, cmd.rsp, sizeof(card->raw_cid));

	// Send CMD3, to get the card relative address 
	cmd.indx = CMD(3);
	cmd.flags = 0;
	cmd.arg = 0;
	if (err==0) err = mmc_hs_cmd_send(ctrl, &cmd);
	card->rca = SD_RCA_ADDR(cmd.rsp[0]);

	// Send CMD9, to get the card specific data 
	cmd.indx = CMD(9);
	cmd.flags = SD_CMDRSP_136BITS;
	cmd.arg = card->rca << 16;
	if (err==0) err = mmc_hs_cmd_send(ctrl,&cmd);
	memcpy(card->raw_csd, cmd.rsp, sizeof(card->raw_csd));

	if (SD_CARD_CSD_VERSION(card) != 0) {
		card->tran_speed = SD_CARD1_TRANSPEED(card);
		card->blk_len    = 1 << (SD_CARD1_RDBLKLEN(card));
		card->size       = SD_CARD1_SIZE(card);
		card->n_blks      = card->size / card->blk_len;
	} else {
		card->tran_speed = SD_CARD0_TRANSPEED(card);
		card->blk_len    = 1 << (SD_CARD0_RDBLKLEN(card));
		card->size       = SD_CARD0_SIZE(card);
		card->n_blks      = SD_CARD0_NUMBLK(card);
	}

	// Set data block length to 512 (for byte addressing cards) 
	if (!(card->high_capacity)) {
		cmd.indx = CMD(16);
		cmd.flags = SD_CMDRSP_NONE;
		cmd.arg = 512;
		if (err==0) err = mmc_hs_cmd_send(ctrl, &cmd);
		card->blk_len = 512;
	}

	// Select the card 
	cmd.indx = CMD(7);
	cmd.flags = SD_CMDRSP_BUSY;
	cmd.arg = card->rca << 16;
	if (err==0) err = mmc_hs_cmd_send(ctrl, &cmd);

	/*
	 * Send ACMD51, to get the SD Configuration register details.
	 * Note, this needs data transfer (on data lines).
	 */
	cmd.indx = CMD(55);
	cmd.flags = 0;
	cmd.arg = card->rca << 16;
	if (err==0) err = mmc_hs_cmd_send(ctrl, &cmd);
	if (err==0) err = mmc_transfer_setup (ctrl, READ_OPER, card->data_buffer, 1);

	cmd.indx = CMD(51);
	cmd.flags = SD_CMDRSP_READ | SD_CMDRSP_DATA;
	cmd.arg = card->rca << 16;
	cmd.nblks = 1;
	cmd.data = (uint8_t*)card->data_buffer;
        if (err==0) err = mmc_hs_cmd_send(ctrl, &cmd);
	if (err==0) err = mmc_transfer_get_status(ctrl);
	// fill raw_scr's card tab
	card->raw_scr[0] = card->data_buffer[0];
	card->raw_scr[1] = card->data_buffer[1];

/*Gac*/	// reinitialize data buffer.
/*Gac*/	memset(card->data_buffer, '\0', DATA_RESPONSE_WIDTH);

	// get SD card version & bus width
	card->sd_ver = SD_CARD_VERSION(card);
	card->card_bus_width = SD_CARD_BUSWIDTH(card);

	return err;
}

/* -------------------------------------------------------------------------- */

static int mmc_init_emmc (enum am335x_mmc_controllers ctrl)
{
	struct mmc_card_info *card = &cards[ctrl];
	mmc_cmd_t cmd = { .flags = 0, };

	int err = 0;
	uint32_t retry = 0xffff;
	card->card_type = CARD_EMMC;

	if (err==0) err = mmc_set_bus_width (ctrl, 1);

	// CMD0 - reset card 
	err = mmc_card_reset(ctrl);
	if (err != 0) return err;

	/*
	 * The SEND_OP_COND (CMD1) command is designed to provide MultiMediaCard 
	 * hosts with a mechanism to identify and reject cards which do not match 
	 * the VDD range desired by the host. This is accomplished by the host 
	 * sending the required VDD voltage window as the operand of this command
	 *
	 * For eMMC devices, the voltage range in CMD1 is no longer valid. 
	 * Regardless of the voltage range indicated by the host, the eMMC devices 
	 * shall respond with a fixed pattern of either 0x00FF 8080 (capacity less 
	 * than or equal to 2GB) or 0x40FF 8080 (capacity greater than 2GB) if 
	 * device is busy, and they shall not move into Inactive state.
	 *
	 * For eMMC devices, the host shall still send the correct Access mode in 
	 * CMD1 argument.
	 */
	// loop until the card is not busy
	retry = 0xffff;
	err = 0;
	while (err==0) {
		mmc_cmd_t cmd = { .indx = CMD(1), .flags = 0, .arg = 0x40FF8000, };
		err = mmc_hs_cmd_send(ctrl, &cmd);
		if (err != 0) break;

		int delay = 0xFF;
		while(delay--); // wait for it to response

		if ((cmd.rsp[0] & (0x1<<31)) != 0) {
			// The extra bit indicates if we support high capacity
			card->high_capacity = (cmd.rsp[0] & SD_OCR_HIGH_CAPACITY) != 0;
			break;
		}

		retry--;
		if (retry == 0) err = -1;
	} 

	// Send CMD2, to get the card identification register 
	cmd.indx = CMD(2);
	cmd.flags = SD_CMDRSP_136BITS;
	cmd.arg = 0;
	if (err==0) err = mmc_hs_cmd_send(ctrl, &cmd);
	memcpy(card->raw_cid, cmd.rsp, sizeof(card->raw_cid));

	// Send CMD3, to get the card relative address 
	card->rca = 1;
	cmd.indx = CMD(3);
	cmd.flags = 0;
	cmd.arg = card->rca << 16;
	if (err==0) err = mmc_hs_cmd_send(ctrl, &cmd);

	// Send CMD9, to get the card specific data 
	cmd.indx = CMD(9);
	cmd.flags = SD_CMDRSP_136BITS;
	cmd.arg = card->rca << 16;
	if (err==0) err = mmc_hs_cmd_send(ctrl,&cmd);
	memcpy(card->raw_csd, cmd.rsp, sizeof(card->raw_csd));

	if (SD_CARD_CSD_VERSION(card) != 0) {
		card->tran_speed = SD_CARD1_TRANSPEED(card);
		card->blk_len    = 1 << (SD_CARD1_RDBLKLEN(card));
		card->size       = SD_CARD1_SIZE(card);
		card->n_blks      = card->size / card->blk_len;
	} else {
		card->tran_speed = SD_CARD0_TRANSPEED(card);
		card->blk_len    = 1 << (SD_CARD0_RDBLKLEN(card));
		card->size       = SD_CARD0_SIZE(card);
		card->n_blks      = SD_CARD0_NUMBLK(card);
	}

	// Select the card 
	cmd.indx = CMD(7);
	cmd.flags = SD_CMDRSP_BUSY;
	cmd.arg = card->rca << 16;
	if (err==0) err = mmc_hs_cmd_send(ctrl, &cmd);

	// CMD8 - now that the card is selected we can read the extended CSD.
	if (err==0) err = mmc_transfer_setup (ctrl, READ_OPER, card->data_buffer, 1);
	cmd.indx = CMD(8);
	cmd.flags = SD_CMDRSP_READ | SD_CMDRSP_DATA;
	cmd.arg = 0;
	cmd.nblks = 1;
	cmd.data = (uint8_t*)card->data_buffer;
	if (err==0) err = mmc_hs_cmd_send(ctrl, &cmd);
	if (err==0) err = mmc_transfer_get_status(ctrl);

/*Gac*/	// reinitialize data buffer.
/*Gac*/	memset(card->data_buffer, '\0', DATA_RESPONSE_WIDTH);

	#define MMC_SWITCH_MODE_WRITE_BYTE 3
	#define EXT_CSD_HS_TIMING 185
	#define EXT_CSD_BUS_WIDTH 183
	#define EXT_CSD_BUS_WIDTH_8 2
	if (err==0) err = mmc_switch(ctrl, MMC_SWITCH_MODE_WRITE_BYTE, EXT_CSD_HS_TIMING, 0x1);
	if (err != 0) printf ("error switch timing\n");
	int delay = 0xFFF;
	while(delay--); // wait for it to response

	if (err==0) err = mmc_switch(ctrl, MMC_SWITCH_MODE_WRITE_BYTE, EXT_CSD_BUS_WIDTH, EXT_CSD_BUS_WIDTH_8);
	if (err != 0) printf ("error switch bus width\n");
	if (err==0) err = mmc_set_bus_width (ctrl, 8);
	if (err != 0) printf ("error setting bus width\n");

	return err;
}
/* --------------------------------------------------------------------------
 * implementation of the public methods
 * -------------------------------------------------------------------------- */

int am335x_mmc_init(enum am335x_mmc_controllers ctrl)
{
	int err = 0;

	am335x_clock_enable_mmc_module(ctrl);
	am335x_mux_setup_mmc_pins(ctrl);

	if (err==0) err = mmc_reset (ctrl);
	if (err==0) err = mmc_reset_line (ctrl, SD_SYSCTL_SRA);

	if (ctrl == AM335X_MMC0) {
		if (err==0) err = mmc_set_bus_voltage (ctrl, SD_CAPA_VS18 | SD_CAPA_VS30, SD_HCTL_VOLTAGE3);
		if (err==0) err = mmc_set_module_idle_mode (ctrl);
		if (err==0) err = mmc_set_bus_width (ctrl, 4);
		if (err==0) err = mmc_set_bus_freq (ctrl, 96000000, 8000000);
	} else if (ctrl == AM335X_MMC1) {
		if (err==0) err = mmc_set_bus_voltage (ctrl, SD_CAPA_VS18 | SD_CAPA_VS33, SD_HCTL_VOLTAGE33);
		if (err==0) err = mmc_set_module_idle_mode (ctrl);
		if (err==0) err = mmc_set_bus_width (ctrl, 8);
		if (err==0) err = mmc_set_bus_freq (ctrl, 48000000, 1000000);
//		#warning "AM335X_MMC1 initialization..."
	}

	if (err==0) err = mmc_set_bus_power (ctrl, true);
	if (err==0) err = mmc_send_init_frame (ctrl);
	if (err==0) err = mmc_interrupt_enable (ctrl);

	if (err==0) am335x_edma3_init(AM335X_EDMA3CC, 0);
	
	return err;
}

/* -------------------------------------------------------------------------- */

int am335x_mmc_init_card (enum am335x_mmc_controllers ctrl)
{
	struct mmc_card_info *card = &cards[ctrl];

	int err = mmc_card_reset(ctrl);
	if (err!=0) return err; 

	am335x_edma3_channel_init (AM335X_EDMA3CC, 
	  AM335X_EDMA3_CHANNEL_TYPE_DMA, card->dma_rx_channel, 0, 
	  mmc_edma3_callback, (void*)ctrl);

	am335x_edma3_channel_init (AM335X_EDMA3CC, 
	  AM335X_EDMA3_CHANNEL_TYPE_DMA, card->dma_tx_channel, 0, 
	  mmc_edma3_callback, (void*)ctrl);

	// Returns 0 for a SD card, 1 for a non-SD card 
	int status = mmc_type_card_check(ctrl);

	if (status == 0) { 
		// it's an SD Card 
		err = mmc_init_sdcard(ctrl);
	} else {
		// it's an eMMC
		err = mmc_init_emmc(ctrl);
	}

	return err;
}


/* -------------------------------------------------------------------------- */

int am335x_mmc_write(
	enum am335x_mmc_controllers ctrl,
	uint32_t block,
	uint32_t nblks,
	const void *ptr)
{
	struct mmc_card_info *card = &cards[ctrl];
	volatile struct mmc_ctrl *mmc = mmc_ctrl[ctrl];
	int err = 0;

	// test if card is inserted
	if ((mmc->pstate & SD_PSTATE_CINS) == 0) {
		printf("write : no card detected..\n");
		return -1;	// no SD card in the slot
	}

	/*
	 * address is in blks for high cap cards and in actual bytes
	 * for standard capacity cards
	 */
	mmc_cmd_t cmd = { 
		.indx = CMD(24),
		.flags = SD_CMDRSP_WRITE | SD_CMDRSP_DATA,
		.arg = card->high_capacity ? block : block * card->blk_len,
		.nblks = nblks,
	};
	if (nblks > 1) {
		cmd.indx = CMD(25);
		cmd.flags |= SD_CMDRSP_ABORT;
	}

	memcpy(card->data_buffer, ptr, DATA_RESPONSE_WIDTH);

	if (err==0) err = mmc_transfer_setup(ctrl, WRITE_OPER, card->data_buffer, 1);
	int delay = 0xFFF;
	while(delay--); // wait for it to response

	if (err==0) err = /*mmc_app_cmd_send*/mmc_hs_cmd_send(ctrl, &cmd);
	if (err==0) err = mmc_transfer_get_status(ctrl);

/*Gac*/	memset(card->data_buffer, '\0', DATA_RESPONSE_WIDTH);

	if (cmd.nblks > 1) {
		if (err==0) err = mmc_send_stop(ctrl);
	}

	return err;
}

/* -------------------------------------------------------------------------- */

int am335x_mmc_read(
	enum am335x_mmc_controllers ctrl,
	uint32_t block,
	uint32_t nblks,
	void *ptr)
{
	struct mmc_card_info *card = &cards[ctrl];
	volatile struct mmc_ctrl *mmc = mmc_ctrl[ctrl];
	int err = 0;

	// test if card is inserted
	if (((mmc->pstate & SD_PSTATE_CINS) == 0) && (ctrl == AM335X_MMC0)) {
		printf("read : no card detected..\n");
		return -1;	// no SD card in the slot
	}

	/*
	 * Address is in blks for high cap cards and in actual bytes
	 * for standard capacity cards
	 */
	mmc_cmd_t cmd = { 
		.indx = CMD(17),
		.flags = SD_CMDRSP_READ | SD_CMDRSP_DATA,
		.arg = card->high_capacity ? block : block * card->blk_len,
		.nblks = nblks,
	};

/*Gac*/	memset(card->data_buffer, '\xab', DATA_RESPONSE_WIDTH);

	if (err==0) err = mmc_transfer_setup(ctrl, READ_OPER, card->data_buffer, 1);
	int delay = 0xFFF;
	while(delay--); // wait for it to response

	if (err==0) err = /*mmc_app_cmd_send*/mmc_hs_cmd_send(ctrl, &cmd);
	if (err==0) err = mmc_transfer_get_status(ctrl);

	memcpy(ptr, card->data_buffer, DATA_RESPONSE_WIDTH);
/*Gac*/	memset(card->data_buffer, '\0', DATA_RESPONSE_WIDTH);

	return err;
}



#ifdef INTC_EN
/**
 * MMC interrupt handler
 */
void am335x_mmc_isr(enum am335x_mmc_controllers ctrl)
{
	volatile struct mmc_card_info* card = &cards[ctrl];
	volatile struct mmc_ctrl *mmc = mmc_ctrl[ctrl];

	uint32_t status = mmc->stat;
	mmc->stat = status;

	if ((status & SD_STAT_CC) != 0) {
		card->flag_command_completed = true;
	}

	if ((status & SD_STAT_ERRI) != 0) {
		card->error_flag = status & 0xffff0000;

		if ((status & SD_STAT_CTO) != 0) {
		    card->cmd_timeout = true;
		}

		if ((status & SD_STAT_DTO) != 0) {
		    card->data_timeout = true;
		}
	}

	if ((status & SD_STAT_TC) != 0) {
		card->flag_transfer_completed = true;
	}
}
#endif

