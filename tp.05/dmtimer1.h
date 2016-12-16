#pragma once
#ifndef DMTIMER1_H
#define DMTIMER1_H
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
 * Project:	HEIA-FR / Embedded Systems 1 Laboratory
 *
 * Abstract:	Introduction to device driver development in C
 *
 * Purpose:	 Implementation of a basic timer based on the AM335x DMTimer1 timer.
 *
 * Author: 	Jonathan Rial, Alan Sueur
 * Date: 	december 2016
 */

void dmtimer1_init();

void dmtimer1_refresh_time();

void dmtimer1_start();

void dmtimer1_stop();

void dmtimer1_reset();

uint32_t dmtimer1_get_hours();

uint32_t dmtimer1_get_minutes();

uint32_t dmtimer1_get_seconds();

uint32_t dmtimer1_get_milliseconds();

#endif
