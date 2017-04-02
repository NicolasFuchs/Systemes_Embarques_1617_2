
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
 * Origin:	This driver has been largely inspired from the mikroElektronika
 *		device driver released for the STM32F10VC MCU.
 *
 * Author: 	Daniel Gachet
 * Date: 	11.04.2016
 */

#include <am335x_gpio.h>
#include "am335x_spi.h"

#include "oled.h"

/* R/W pin */
#define OLED_T_PIN	8	
#define OLED_T_GPIO 	AM335X_GPIO0

/* RST pin */
#define OLED_RST_PIN	10
#define OLED_RST_GPIO	AM335X_GPIO0

/* DC pin */
#define OLED_DC_PIN	18
#define OLED_DC_GPIO	AM335X_GPIO1

static void delay_ms(int ms)
{
	for (int i=0; i<ms; i++)
		for (int j=0; j<1000; j++);
}

// --- public methods ----

// init sequence for 96x96 OLED color module
void oled_init()
{
	// init gpio interface
	am335x_gpio_init (AM335X_GPIO0);
	am335x_gpio_init (AM335X_GPIO1);
	am335x_gpio_setup_pin_out (OLED_T_GPIO,   OLED_T_PIN,   false);
	am335x_gpio_setup_pin_out (OLED_RST_GPIO, OLED_RST_PIN, false);
	am335x_gpio_setup_pin_out (OLED_DC_GPIO,  OLED_DC_PIN,  false);

	// init spi interface
	am335x_spi_init (AM335X_SPI1, AM335X_CHAN0, 5000000, 8);

	// oled hard reset
	delay_ms(10);
	am335x_gpio_change_state (OLED_RST_GPIO, OLED_RST_PIN, true);
	delay_ms(10);

	// soft reset
	oled_command (OLED_SOFT_RESET, 0x00);    
  
	// standby ON/OFF (wait for 5ms between on/off (min delay 1ms)
	oled_command (OLED_STANDBY_ON_OFF, 0x01); // on
	delay_ms(5); 
	oled_command (OLED_STANDBY_ON_OFF, 0x00); // off
	delay_ms(5);

	// display turn off
	oled_command (OLED_DISPLAY_ON_OFF, 0x00);

	// Set Oscillator operation (using external resistor and internal OSC)
	oled_command (OLED_ANALOG_CONTROL, 0x00);

	// set frame rate (95Hz)
	oled_command (OLED_OSC_ADJUST,0x03);

	// set active display area of panel
	oled_command (OLED_DISPLAY_X1, 0x00);
	oled_command (OLED_DISPLAY_X2, 0x5F);
	oled_command (OLED_DISPLAY_Y1, 0x00);
	oled_command (OLED_DISPLAY_Y2, 0x5F);

	// Select the RGB data format and set the initial state of RGB 
	// interface port (RGB 8bit interface)
	oled_command (OLED_RGB_IF, 0x00);           

	// set RGB polarity
	oled_command (OLED_RGB_POL, 0x00);

	// set display mode control (SWAP:BGR, Reduce current : Normal, DC[1:0] : Normal)
	oled_command (OLED_DISPLAY_MODE_CONTROL, 0x80);

	// set MCU Interface (MPU External interface mode, 8bits)
	oled_command (OLED_CPU_IF, 0x00);

	// set Memory Read/Write mode
	oled_command (OLED_MEMORY_WRITE_READ, 0x02);

	// set row scan direction (column: 0 --> max, row : 0 --> max)
	oled_command (OLED_ROW_SCAN_DIRECTION, 0x00);

	// Set row scan mode (alternate scan mode)
	oled_command (OLED_ROW_SCAN_MODE, 0x00); 

	// set column current
	oled_command (OLED_COLUMN_CURRENT_R, 0x6E);
	oled_command (OLED_COLUMN_CURRENT_G, 0x4F);
	oled_command (OLED_COLUMN_CURRENT_B, 0x77);

	// set row overlap (band gap only)
	oled_command (OLED_ROW_OVERLAP, 0x00);  

	// set discharge time (normal discharge time)
	oled_command (OLED_DISCHARGE_TIME, 0x01);

	// set peak pulse delay
	oled_command (OLED_PEAK_PULSE_DELAY, 0x00);

	// set peak pulse width
	oled_command (OLED_PEAK_PULSE_WIDTH_R, 0x02);
	oled_command (OLED_PEAK_PULSE_WIDTH_G, 0x02);
	oled_command (OLED_PEAK_PULSE_WIDTH_B, 0x02);

	// set precharge current
	oled_command (OLED_PRECHARGE_CURRENT_R, 0x14);
	oled_command (OLED_PRECHARGE_CURRENT_G, 0x50);
	oled_command (OLED_PRECHARGE_CURRENT_B, 0x19);
	
	// Set row scan on/off (normal row scan)
	oled_command (OLED_ROW_SCAN_ON_OFF, 0x00);

	// set scan off level (VCC_C*0.75)
	oled_command (OLED_SCAN_OFF_LEVEL, 0x04);

	// set memory access point
	oled_command (OLED_DISPLAYSTART_X, 0x00);
	oled_command (OLED_DISPLAYSTART_Y, 0x00);

	// display turn on
	oled_command (OLED_DISPLAY_ON_OFF, 0x01);
}

// send command to OLED display
void oled_command (uint32_t cmd, uint32_t value)
{
	// select command
	am335x_gpio_change_state(OLED_DC_GPIO, OLED_DC_PIN, false);
	am335x_spi_write(AM335X_SPI1, AM335X_CHAN0, &cmd, 1);

	// write command value
	am335x_gpio_change_state(OLED_DC_GPIO, OLED_DC_PIN, true);
	am335x_spi_write(AM335X_SPI1, AM335X_CHAN0, &value, 1);
}

// set memory area(address) to write a display data
void oled_memory_size(uint32_t x1, uint32_t x2, uint32_t y1, uint32_t y2)
{
	oled_command (OLED_MEM_X1, x1);
	oled_command (OLED_MEM_X2, x2);
	oled_command (OLED_MEM_Y1, y1);
	oled_command (OLED_MEM_Y2, y2);

	//set DDRAM access port
	uint32_t data[1] = {0x08};
	am335x_gpio_change_state (OLED_DC_GPIO, OLED_DC_PIN, false);
	am335x_spi_write (AM335X_SPI1, AM335X_CHAN0, data, 1);

	// switch to data mode
	am335x_gpio_change_state (OLED_DC_GPIO, OLED_DC_PIN, true);
}

// send color for 1 pixel
void oled_color(uint32_t color)
{
	uint32_t data[2] = {color>>8, color&0xff};\
	am335x_spi_write(AM335X_SPI1, AM335X_CHAN0, data, 2);
}

