#pragma once
#ifndef AM335X_MMC_H
#define AM335X_MMC_H
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
 * Abstract: 	AM335x MMC Driver 
 *
 * Purpose:	This module implements basic services to drive the AM335x 
 * 		MMC module.
 *
 * Author: 	Romain Plouzin / Daniel Gachet
 * Date: 	03.08.2016
 */

#include <stdint.h>

/**
 * defines the 3 available MMC controllers of the am335x
 */
enum am335x_mmc_controllers {
	AM335X_MMC0,
	AM335X_MMC1,
};

/*
 * method to initialize MMC peripheral
 *
 *@param ctrl number of the controller to initialize
 *
 */
extern int am335x_mmc_init(enum am335x_mmc_controllers ctrl);


/**
 * method to send initialization commands to MMC/SD card
 *
 *@param ctrl it holds the mmcsd control information
 *
 */
extern int am335x_mmc_init_card(
	enum am335x_mmc_controllers ctrl);


/**
 * method to write data into MMC card
 *
 *@param ctrl it holds the mmcsd control information
 *@param block the block from where start a write command
 *@param nblks the number of block to be written
 *@param ptr a pointer to the data to be written
 * 
 */
extern int am335x_mmc_write(
	enum am335x_mmc_controllers ctrl,
	uint32_t block, 
	uint32_t nblks,
	const void *ptr);


/*
 * Method to read data from MMC/SD card
 */
/**
 * method to read data into MMC card
 *
 *@param ctrl it holds the mmcsd control information
 *@param block the block from where start a read command
 *@param nblks the number of block to be read
 *@param ptr a pointer to the data to be read
 * 
 */
extern int am335x_mmc_read(
	enum am335x_mmc_controllers ctrl,
	uint32_t block,
	uint32_t nblks,
	void *ptr);


extern void am335x_mmc_isr(
	enum am335x_mmc_controllers ctrl);

#endif
