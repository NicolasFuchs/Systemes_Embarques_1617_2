/**
 * Copyright 2017 University of Applied Sciences Western Switzerland / Fribourg
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
 * Project:		HEIA-FR / Embedded Systems 1 Laboratory
 * 						Personal Operating System
 *
 * Abstract:	Implementation of a basic cooperative Operating System
 *
 * Purpose:		This module implement buzzer utilities.
 *
 * Author:		Daniel Gachet (first author)
 * 				Alan Sueur
 * 				Nicolas Fuchs
 *
 * Date: 		27.04.2017
 * Last update:	28.04.2017
 *
 */

#include "buzzer.h"
#include "am335x_console.h"
#include <string.h>
#include <stdlib.h>
#include "epwm1.h"
#include "vfs.h"

static int buzzer_open (const char *name, int flags, int mode) {
	(void)name; (void)flags; (void)mode;
  	return 0;
}

static int buzzer_read(int file, char *ptr, int len) {
	(void)file; (void)ptr; (void)len;
	return 0;
}

static int buzzer_write(int file, char *ptr, int len) {
	(void)file;
	char buff[20];
	memset(buff, 0, sizeof(char)*20);
	for(int i = 0; i < len; i++) buff[i] = ptr[i];
	int freq = atoi(buff);
	epwm1_init();
	if(freq)
	{
		epwm1_set_frequency(freq);
		epwm1_set_duty(50);
	}
	else
	{
		epwm1_set_duty(0);
	}
	return len;
}

static int buzzer_lseek(int file, int ptr, int dir) {
	(void)file; (void)ptr; (void)dir;
	return 0;
}

static int buzzer_isatty(int file) {
	(void)file;
	return 0;
}

static int buzzer_fstat(int file, struct stat *st) {
	(void)file; (void)st;
	return 0;
}

static int buzzer_close(int file) {
	(void)file;
	return 0;
}

static struct file_ops f_ops = {
	.open  = buzzer_open,
	.read  = buzzer_read,
	.write = buzzer_write,
	.lseek = buzzer_lseek,
	.isatty= buzzer_isatty,
	.fstat = buzzer_fstat,
	.close = buzzer_close,

};

struct file_ops* buzzer_init()
{
	return &f_ops;
}
