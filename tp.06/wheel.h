
#pragma once
#ifndef WHEEL_H_
#define WHEEL_H_

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
 * Abstract:	Introduction the C programming language
 *
 * Purpose:	Simple C program implementing basic access to the GPIO modules
 *		of the AM3358 microprocessor to control the wheel and the
 * 		7-segment display of the HEIA-FR extension card of the
 *		Beaglebone Black board
 *
 * Author: 	Charlotte Junod and Nicolas Fuchs
 * Date: 	25.11.2016
 */

/*
 * wheel_states:
 *
 */
enum wheel_states {NONE,INCR,DECR,RESET};

/*
 * wheel_init:
 * init the wheel
 */
void wheel_init();

/*
 * wheel_get_state:
 * return the current state of the wheel
 */
enum wheel_states wheel_get_state();

#endif /* WHEEL_H_ */
