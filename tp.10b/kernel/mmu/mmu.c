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
 * This module is based on the software library developped by Texas Instruments
 * Incorporated - http://www.ti.com/ for its AM335x starter kit.
 *
 * Project:	HEIA-FR / Embedded Systems 1+2 Laboratory
 *
 * Abstract: 	AM335x - Cortex-A8 - Memory Management Unit
 *
 * Purpose:	This module implements basic services to drive the MMU
 *
 * Author: 	Daniel Gachet
 * Date: 	07.05.2017
 */

#include <stdio.h>
#include <stdint.h>
#include "mmu.h"

//#define pr_debug(x...) printf(x)
#define pr_debug(x...) 

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
	{"sdram",     0x80000000, 0x80000000,  15, NORMAL},
	{"sdram_dma", 0x80f00000, 0x80f00000,   1, STRONG},
	{"processes", 0x88000000, 0x88000000, 128, NORMAL},
};


/* -- Low level MMU service ------------------------------------------------- */
/* Ref: 03_cortex_a_programmer_guide.pdf, chapter 9
 * Ref: 02_cortex_a8_technical_reference_manual.pdf, chapter 3 & 6
 *   3.2.25 c1,  Control Register
 *   3.2.31 c2,  Translation Table Base Register 0
 *   3.2.33 c2,  Translation Table Base Control Register
 *   3.2.34 c3,  Domain Access Control Register
 *   3.2.41 c8,  TLB operations
 *   3.2.58 c10, Memory Region Remap Registers
 */

extern void  mmu_ll_set_ttrb0(volatile void* addr);
extern void  mmu_ll_set_dacr(uint32_t val);
extern void  mmu_ll_tlb_invalidate();

/* -- Internal types and constant definition -------------------------------- */

// SRAM :     o = 0x402F0400,  l = 0x0000FC00  /* 64KB internal SRAM */
#define TT_L1_BASE 0x402f4000
#define TT_L2_BASE 0x402f0400

#define DACR_CLIENT 0x55555555 /* grant full access to all */

#define TT_SZ	4096

static volatile uint32_t *ttl1 = (volatile uint32_t*) TT_L1_BASE;

/* -- Public methods definition --------------------------------------------- */

void mmu_init()
{
	mmu_ll_set_dacr(DACR_CLIENT);

	// mark all memory area as faulty
	for (int i=0; i<TT_SZ; i++) {
		ttl1[i] = 0;
	}

	mmu_ll_set_ttrb0(ttl1);

	mmu_configure_regions (regions, ARRAY_SIZE(regions));
	mmu_enable();
}

/* -------------------------------------------------------------------------- */

extern void mmu_configure_regions (const struct region* regions, int count)
{
	// configure memory regions as specified
	for (int i=0; i<count; i++)
	{
		uint32_t cfg = regions[i].cfg;
		uint32_t phy_addr = regions[i].physical_addr & 0xfff00000;
		uint32_t vir_addr = regions[i].virtual_addr >> 20;
		volatile uint32_t* tt  = &ttl1[vir_addr];

		for (int j=regions[i].sz; j>0; j--) {
			*tt++ = phy_addr | cfg;
			phy_addr += 0x100000;
		}
	}
	mmu_ll_tlb_invalidate();
}
