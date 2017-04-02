#pragma once
#include "diskio.h"		
DSTATUS MMC_disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
);

DSTATUS MMC_disk_initialize (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
);
DRESULT MMC_disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,		/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
);

DRESULT MMC_disk_write (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Start sector in LBA */
	UINT count		/* Number of sectors to write */
);

DRESULT MMC_disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
);

