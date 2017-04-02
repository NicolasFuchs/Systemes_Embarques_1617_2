#include <stdio.h>
#include "oled.h"

#include <am335x_gpio.h>


static void delay_ms(int ms)
{
	for (int i=0; i<ms; i++)
		for (int j=0; j<1000; j++);
}


static void oled_background()
{
	oled_memory_size (0,95,0,95);
	for(int j=0; j<(96*96); j++) {
		oled_color (0xffff);
	}

	oled_memory_size(5,90,5,90);
	for(int j=0; j<(86*86); j++){
		oled_color (0x0cc0);
	}

	oled_memory_size (10,85,10,85);
	for(int j=0; j<(76*76); j++){
		oled_color (0xff00);
	}

	oled_memory_size (15,80,15,80);
	for(int j=0; j<(66*66); j++){
		oled_color (0x8000);
	}

	oled_memory_size (20,75,20,75);
	for(int j=0; j<(56*56); j++){
		oled_color (0xf800);
	}

	oled_memory_size (25,70,25,70);
	for(int j=0; j<(46*46); j++){
		oled_color (0x00ff);
	}

	oled_memory_size (30,65,30,65);
	for(int j=0; j<(36*36); j++){
		oled_color (0x80ff);
	}
}

static void oled()
{
	oled_background();

	delay_ms(500);

	for(int i=0;i<8;i++) {
		oled_command (OLED_SCREEN_SAVER_MODE, i);
		oled_command (OLED_SCREEN_SAVER_CONTROL, 0x88);
		delay_ms (500);
		oled_command (OLED_SCREEN_SAVER_CONTROL, 0x00);
	}
}

int main()
{
	oled_init();
	oled();
	while(1);

	return 0;
}
