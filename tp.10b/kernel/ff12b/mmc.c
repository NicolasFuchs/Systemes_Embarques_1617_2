/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "mmc.h"

#include "am335x_mmc.h"
#include "am335x_edma3.h"
#include "am335x_gpio.h"
#include "interrupt.h"
#include "intc.h"

#define SDCARD
#ifdef SDCARD
#define MMC_CARD		AM335X_MMC0
#define MMC_CARD_INT	 	INTC_MMC0
#else
#define MMC_CARD		AM335X_MMC1
#define MMC_CARD_INT	 	INTC_MMC1
#endif

//#define printd(args...) printf(args)
#define printd(args...)
#define unused(x) (void)x

//------------------------------------------------------------------------

struct partition {
	uint8_t  state;
	uint8_t  bhead;
	uint16_t bsector;
	uint8_t  type;
	uint8_t  ehead;
	uint16_t esector;
	uint16_t fsector[2];
	uint16_t nsectors[2];
};

struct mbr {
	uint16_t xcode[223];
	struct partition p[4];
	uint8_t sign[2];
};

//------------------------------------------------------------------------

static int is_initialized=0;

//------------------------------------------------------------------------

static void mmc_isr(enum intc_vectors vector, void* param)
{
	(void)vector;
	am335x_mmc_isr((enum am335x_mmc_controllers)param);
}
static void edma_completion_isr(enum intc_vectors vector, void* param)
{
	(void)vector; (void)param;
	am335x_edma_completion_isr();
}
static void edma_error_isr(enum intc_vectors vector, void* param)
{
	(void)vector; (void)param;
	am335x_edma_error_isr();
}

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS MMC_disk_status (
	BYTE pdrv
)
{
	(void)pdrv;
	printd ("MMC_disk_status\n");
	DSTATUS stat = is_initialized ? 0 : STA_NOINIT;
	return stat;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS MMC_disk_initialize (
	BYTE pdrv
)
{
	(void)pdrv;
	printd ("MMC_disk_initialize\n");
	int status=0;

	if (is_initialized) return 0;

	// Registering HSMMC Interrupt handler
	intc_attach (MMC_CARD_INT, mmc_isr, (void*)MMC_CARD);
	
	// Registering EDMA3 Channel Controller transfer completion interrupt.
	intc_attach (INTC_EDMACOMPINT, edma_completion_isr, 0);

	// Registering EDMA3 Channel Controller Error Interrupt.
	intc_attach (INTC_EDMAERRINT, edma_error_isr, 0);

	am335x_gpio_init (AM335X_GPIO2);
	am335x_gpio_setup_pin_out (AM335X_GPIO2, 0, true);

	// init MMC/SD driver
	status = am335x_mmc_init(MMC_CARD);//, HSMMCSD_IN_FREQ, HSMMCSD_INIT_FREQ);
	printd("\nMMC driver initialization : %s\n", (status == 0) ? "OK" : "failed");

	// init MMC/SD card
	status  = am335x_mmc_init_card(MMC_CARD);
	printd("Card initialization : %s\n\n", (status == 0) ? "OK" : "failed");

	if (status == 0) {
		struct mbr mbr;
		status = am335x_mmc_read(MMC_CARD, 0, 1, &mbr);
		if (status == 0) {
			long sector_ofs = (uint32_t)mbr.p[0].fsector[0] + ((uint32_t)mbr.p[0].fsector[1]<<16);
			unused(sector_ofs);
			printd ("mbr.p[0] --> ofs, %d\n", (char*)&mbr.p[0]-(char*)&mbr);
			printd ("sign=%x,%x\n", mbr.sign[0], mbr.sign[1]);
			printd ("state  =%x\n", mbr.p[0].state);
			printd ("bhead  =%x\n", mbr.p[0].bhead);
			printd ("bsector=%x\n", mbr.p[0].bsector);
			printd ("type   =%x\n", mbr.p[0].type);
			printd ("ehead  =%x\n", mbr.p[0].ehead);
			printd ("esector=%x\n", mbr.p[0].esector);
			printd ("fsector=%x,%x (%d)\n", mbr.p[0].fsector[0], mbr.p[0].fsector[1], sector_ofs);
			printd ("nsector=%x,%x\n", mbr.p[0].nsectors[0], mbr.p[0].nsectors[1]);

			int valid_partition = 0;
			switch(mbr.p[0].type) {
			case 0x0B:
			case 0x06:
			case 0x0C:
			case 0x0E:
			case 0x0F:
			case 0x05:
			    valid_partition = 1;
			break;
			case 0x00:
			    valid_partition = 0;
			    break;
			default:
			    if (mbr.p[0].type <= 0x06)
				valid_partition = 1;
			break;
			}
			if (!(valid_partition && (mbr.sign[0] == 0x55) && (mbr.sign[1] == 0xaa))) status = -1;
			is_initialized = (status == 0);
			printd ("status=%d\n", status);
		}
	}

	return (status == 0) ? 0 : STA_NODISK;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT MMC_disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,		/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	(void)pdrv;
	int status = 0;
	printd ("MMC read: sector=%d, count=%d", sector, count);

	for (unsigned long i=0;i<count;i++) {
		status = am335x_mmc_read(MMC_CARD, sector, 1, buff);
		sector++;
		buff += 512;
	}
	printd (", status=%d\n", status);

	return (status == 0) ? RES_OK : RES_ERROR;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT MMC_disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	(void)pdrv;
	int status = 0;
	printd ("MMC read: sector=%d, count=%d", sector, count);

	for (unsigned long i=0;i<count;i++) {
		status = am335x_mmc_write(MMC_CARD, sector, 1, buff);
		sector++;
		buff += 512;
	}
	printd (", status=%d\n", status);

	return (status == 0) ? RES_OK : RES_ERROR;
}



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT MMC_disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res = RES_PARERR;
	printd ("MMC_disk_ioctl: cmd=%d\n",cmd);
	(void)pdrv;
	(void)buff;
	if (cmd == CTRL_SYNC) res=RES_OK;

	return res;
}

