#pragma once
#ifndef MMU_H
#define MMU_H
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
 * Date: 	07.05.2016
 */

#include <stdint.h>

// 1st level translation table: device types
//   nG     = non-Global
//   S      = Shareable
//   APX-AP = Access Permissions
//   TEX    = Type EXtension
//   NX     = Never Execute
//   C      = Cacheable
//   B      = Bufferable
//                  0        nG         S        APX       TEX       AP        
//                  P        NX         C        B
#define STRONG	((0<<18) | (0<<17) | (0<<16) | (0<<15) | (0<<12) | (3<<10) | \
                 (0<< 9) | (1<< 4) | (0<< 3) | (0<< 2) | (2))
#define NORMAL	((0<<18) | (0<<17) | (0<<16) | (0<<15) | (5<<12) | (3<<10) | \
                 (0<< 9) | (0<< 4) | (1<< 3) | (1<< 2) | (2))

// memory region configuration
struct region {
	char* region_name;        // name of the region in 1MiB section
	uint32_t virtual_addr;    // virtual address
	uint32_t physical_addr;   // physical adress
	uint32_t sz;              // size of the region in MiB
	uint32_t cfg;             // configuration flags of the section building the region
};

extern void mmu_init();
extern void mmu_configure_regions (const struct region* regions, int count);
extern void mmu_enable();
extern void mmu_disable();

#endif
