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

#include "kernel.h"
#include "mmu.h"

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))

// list of memory regions in 1MiB sections with populated I/O devices and memories
static const struct region regions[] = {
 	{"sram",      0x40200000, 0x40200000,   1, NORMAL},
	{"l3_ocmc",   0x40300000, 0x40300000,   1, NORMAL},
	{"l3f_cfg",   0x44000000, 0x44000000,   4, STRONG},
	{"l3s_cfg",   0x44800000, 0x44800000,   4, STRONG},
	{"l4_wkup",   0x44c00000, 0x44c00000,   4, STRONG},
	{"mcasp0",    0x46000000, 0x46000000,   4, STRONG},	
	{"mcasp1",    0x46000000, 0x46000000,   4, STRONG},	
	{"usb",       0x47400000, 0x47400000,   1, STRONG},
	{"mmchs2",    0x47800000, 0x47800000,   1, STRONG},
	{"l4_per",    0x48000000, 0x48000000,  16, STRONG},
	{"tpcc",      0x49000000, 0x49000000,   1, STRONG},
	{"tptc0",     0x49800000, 0x49800000,   1, STRONG},
	{"tptc1",     0x49900000, 0x49900000,   1, STRONG},
	{"tptc2",     0x49a00000, 0x49a00000,   1, STRONG},
	{"l4_fast",   0x4a000000, 0x4a000000,  16, STRONG},
	{"debug",     0x4b100000, 0x4b100000,   1, STRONG},
	{"emif0",     0x4c000000, 0x4c000000,  16, STRONG},
	{"gpmc",      0x50000000, 0x50000000,  16, STRONG},
	{"adc_tsc",   0x54c00000, 0x54c00000,   4, STRONG},
	{"sgx530",    0x56000000, 0x56000000,  16, STRONG},
	{"sdram",     0x80000000, 0x80000000, 512, NORMAL},
	{"sdram_dma", 0x80f00000, 0x80f00000,   1, STRONG},
};

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


void thread (void*)
{
	printf ("thread created: name%s, id=%d, param=%p\n", kernel_thread_name(), kernel_thread_id(), param);
	while(1) {
		kernel_thread_yield();
	}
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
	mmu_init();
	mmu_configure_regions (regions, ARRAY_SIZE(regions));
	mmu_enable();

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

