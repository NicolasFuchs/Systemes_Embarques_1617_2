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

#include <am335x_gpio.h>
#include <am335x_clock.h>

#include "dmtimer1.h"

/**
 * method to enable the clock instance for a specific timer module.
extern void am335x_clock_enable_timer_module (enum am335x_clock_timer_modules module);
*/

/**
 * am335x gpio clock modules
enum am335x_clock_gpio_modules {
    AM335X_CLOCK_GPIO0,
    AM335X_CLOCK_GPIO1,
    AM335X_CLOCK_GPIO2,
    AM335X_CLOCK_GPIO3,
};
 */
#include <stdbool.h>
#include <am335x_gpio.h>
#include "dmtimer1.h"
#include <am335x_clock.h>

#define TISTAT_RESETDONE (1 << 0)
#define TCLR_AR (1 << 1)
#define TCLR_ST (1 << 0)
#define TIOCP_CFG_SOFTRESET (1 << 1)

struct dmtimer1_regs{
    uint32_t res1[3];
    uint32_t tidr;
    uint32_t tiocp_cfg;
    uint32_t tistat;
    uint32_t tisr;
    uint32_t tier;
    uint32_t twer;
    uint32_t tclr; // Auto-reload on
    uint32_t tcrr; // Counter register
    uint32_t tldr; // Load register
    uint32_t ttgr; // Trig register
    uint32_t twps;
    uint32_t tmar;
    uint32_t tcar1;
    uint32_t tsicr;
    uint32_t tcar2;
    uint32_t tpir;
    uint32_t tnir;
    uint32_t tcvr;
    uint32_t tocr;
    uint32_t towr;
};

static volatile struct dmtimer1_regs* timer1 = (struct dmtimer1_regs*)0x44e31000;


uint32_t dmtimer1_get_counter(){
    uint32_t tccr = timer1->tcrr;
    return tccr;
}

void dmtimer1_init(){

    static bool is_initialized = false;
    if(is_initialized) return;

    am335x_clock_enable_timer_module(AM335X_CLOCK_TIMER1);
    timer1->tiocp_cfg = TIOCP_CFG_SOFTRESET;
    while((timer1->tistat & TISTAT_RESETDONE)==0);
    timer1->ttgr = 0;
    timer1->tldr = 0;
    timer1->tclr = TCLR_AR | TCLR_ST;
    timer1->tcrr = 0;

    is_initialized = true;
}

uint32_t dmtimer1_get_frequency(){
    return 24000000;
}