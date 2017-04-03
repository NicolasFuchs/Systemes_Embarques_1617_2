#pragma once
#ifndef OLED_H
#define OLED_H
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
 * Project:	HEIA-FR / Embedded Systems 2 Laboratory
 *
 * Abstract:	OLED-C Click Board (SPEPS114A - LCD)
 *
 * Purpose:	This module provides a basic driver for the OLED-C click board.
 *		This driver is customized to run with the HEIA-FR Beaglebone 
 *		Black in cape 1.
 *
 * Author: 	Daniel Gachet
 * Date: 	11.04.2016
 */

#include <stdint.h>

// OLED commands, for details please read SPES114A datasheet
enum oled_commands {
	OLED_SOFT_RESET=0x01,
	OLED_DISPLAY_ON_OFF=0x02,
	OLED_ANALOG_CONTROL=0x0F,
	OLED_STANDBY_ON_OFF=0x14,
	OLED_OSC_ADJUST=0x1A,
	OLED_ROW_SCAN_DIRECTION=0x09,
	OLED_DISPLAY_X1=0x30,
	OLED_DISPLAY_X2=0x31,
	OLED_DISPLAY_Y1=0x32,
	OLED_DISPLAY_Y2=0x33,
	OLED_DISPLAYSTART_X=0x38,
	OLED_DISPLAYSTART_Y=0x39,
	OLED_CPU_IF=0x0D,
	OLED_MEM_X1=0x34,
	OLED_MEM_X2=0x35,
	OLED_MEM_Y1=0x36,
	OLED_MEM_Y2=0x37,
	OLED_MEMORY_WRITE_READ=0x1D,
	OLED_DDRAM_DATA_ACCESS_PORT=0x08,
	OLED_DISCHARGE_TIME=0x18,
	OLED_PEAK_PULSE_DELAY=0x16,
	OLED_PEAK_PULSE_WIDTH_R=0x3A,
	OLED_PEAK_PULSE_WIDTH_G=0x3B,
	OLED_PEAK_PULSE_WIDTH_B=0x3C,
	OLED_PRECHARGE_CURRENT_R=0x3D,
	OLED_PRECHARGE_CURRENT_G=0x3E,
	OLED_PRECHARGE_CURRENT_B=0x3F,
	OLED_COLUMN_CURRENT_R=0x40,
	OLED_COLUMN_CURRENT_G=0x41,
	OLED_COLUMN_CURRENT_B=0x42,
	OLED_ROW_OVERLAP=0x48,
	OLED_SCAN_OFF_LEVEL=0x49,
	OLED_ROW_SCAN_ON_OFF=0x17,
	OLED_ROW_SCAN_MODE=0x13,
	OLED_SCREEN_SAVER_CONTROL=0xD0,
	OLED_SS_SLEEP_TIMER=0xD1,
	OLED_SCREEN_SAVER_MODE=0xD2,
	OLED_SS_UPDATE_TIMER=0xD3,
	OLED_RGB_IF=0xE0,
	OLED_RGB_POL=0xE1,
	OLED_DISPLAY_MODE_CONTROL=0xE5,
};

/**
 * method to initialize the 96x96 OLED color module
 */
void oled_init();


/**
 * method to send a command to OLED display
 *
 *@param cmd command to send
 *@param value command value
 */
void oled_command (uint32_t cmd, uint32_t value);


/**
 * method to define a memory area(address) to write a display data
 *@param x1 x1 coordinate
 *@param x2 x2 coordinate
 *@param y1 y1 coordinate
 *@param y2 y2 coordinate 
 */
void oled_memory_size(uint32_t x1, uint32_t x2, uint32_t y1, uint32_t y2);


/**
 * method to send color for 1 pixel with previously defined memory area
 *@param color rgb565 pixel color
 */
void oled_color(uint32_t color);

#endif
