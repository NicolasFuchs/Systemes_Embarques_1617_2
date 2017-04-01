#pragma once
#ifndef THERMO_H
#define THERMO_H

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
 * Project:	HEIA-FR / Embedded Systems 1 Laboratory
 *
 * Abstract:	Click Board Thermo3
 *
 * Purpose:	This module implements basic services to drive the click
 *		board Thermo3.
 *
 * Author: 	Daniel Gachet
 * Date: 	19.03.2017
 */

typedef void(*thermo_alarm_ih_t)(bool on);

/**
 * high and low temperatures used for alarm generation
 */
enum thermo_limits {THERMO_T_HIGH, THERMO_T_LOW};

/**
 * method to initialize the thermo resources.
 * should be called prior any other methods.
 */
extern void thermo_init(thermo_alarm_ih_t routine);

/** 
 * method to get the current temperature
 * @return current temperature en degree celsius
 */
extern int thermo_get_temp();

/**
 * method to get alert flag, read from alert pin
 * @return true if alert pin is on, false otherwise
 */
extern bool thermo_is_alert_on();

/** 
 * method to set the Thigh or Tlow limit 
 * @param limit limit to set
 * @param temp temperature limit in degree celsius
 * @return status : 0 --> success, -1 --> error
 */
extern int thermo_set_limit(enum thermo_limits limit, int temp);

/** 
 * method to get the Thigh or Tlow limit 
 * @return temperature limit in degree celsius
 */
extern int thermo_get_limit(enum thermo_limits limit);

#endif
