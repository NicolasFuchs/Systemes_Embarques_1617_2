#pragma once
#ifndef BUZZER_H
#define BUZZER_H
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
 * Project:	HEIA-FR / Embedded Systems 1 Laboratory
 *
 * Abstract:	Click Board Buzzer
 *
 * Purpose:	This module implements basic services to drive the click
 *		board BUZZ.
 *
 * Author: 	Daniel Gachet
 * Date: 	18.12.2016
 */

/**
 * method to initialize the buzzer resources.
 * should be called prior any other methods.
 */
extern void buzzer_init();

/** 
 * method to swith on in the buzzer
 */
extern void buzzer_switch_on();

/** 
 * method to swith off in the buzzer
 */
extern void buzzer_switch_off();

#endif
