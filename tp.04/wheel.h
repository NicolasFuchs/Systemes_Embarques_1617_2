#pragma once
#ifndef WHEEL_H
#define WHEEL_H
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
 * Project:  HEIA-FR / Embedded Systems 1 Laboratory
 *
 * Abstract: Introduction the C programming language
 *
 * Purpose:  Module for managing the wheel of the HEIA-FR extension card of the
 *           Beaglebone Black board
 *
 * Author: 	Jonathan Rial & Alan Sueur
 * Date: 	November 2016
 */

enum wheel_state {
	WHEEL_NONE, WHEEL_INCR, WHEEL_DECR, WHEEL_RESET
};

void wheel_init();

enum wheel_state wheel_get_state();

#endif
