#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "interrupt.h"
#include "intc.h"
#include "exception.h"
#include "ff.h"

static FATFS fs;

static char* fa(BYTE attr) {
	static char a[6];
	a[0] = (attr & AM_DIR) ? 'd' : '-';
	a[1] = (attr & AM_RDO) ? 'r' : '-';
	a[2] = (attr & AM_HID) ? 'h' : '-';
	a[3] = (attr & AM_SYS) ? 's' : '-';
	a[4] = (attr & AM_ARC) ? 'a' : '-';
	a[5] = 0;
	return a;
}

int main()
{
	printf("\n\n");
	printf("HEIA-FR : File System Test Program - "__DATE__" - " __TIME__ "\n");
	printf("-----------------------------------------------------------\n");

	interrupt_init();
	intc_init();
	exception_init();
	interrupt_enable();

	FRESULT r = f_mount (&fs, "/", 1);
	if (r != 0) {
		printf ("error while mounting sc_card\n");
		while(1);
	}

	// read root directory
	DIR dir;
	r=f_opendir(&dir, "/");
	if (r==0) {
		while(1) {
			FILINFO fno;
			FRESULT res=f_readdir(&dir, &fno);
			if ((res != FR_OK )|| (fno.fname[0] == 0)) break;
			printf ("%s %8lu %s\n", fa(fno.fattrib), fno.fsize, fno.fname);

		}
		f_closedir(&dir);
	} else {
		printf ("error while opening root directory\n");
	} 

	while(1);

	return 0;
}

