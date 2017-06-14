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
 * Purpose:		Micro kernel service definition
 *
 * Author:		Daniel Gachet (first author)
 * 				Alan Sueur
 * 				Nicolas Fuchs
 *
 * Date: 		27.04.2017
 * Last update:	28.04.2017
 *
 */

#pragma once
#ifndef KERNEL_H_
#define KERNEL_H_

#include <stdlib.h>

// the four different state for one thread
enum states {TERMINATED,READY,RUNNING,BLOCKED};

typedef void (*kernel_thread_t)(void* param);

extern void kernel_init();
extern void kernel_launch();

extern int kernel_exec(const char* filename, const char* name, int argc, char* argv[]);

extern int kernel_fork(char* task_name, char* new_task_name);
extern int kernel_thread_create(kernel_thread_t routine, void* param, const char* name, size_t stack_sz);
extern void kernel_thread_yield();
extern void kernel_thread_exit();
extern int kernel_kill(int pid);

extern int kernel_thread_id();
extern const char* kernel_thread_name();

extern int kernel_sema_create(int initial_value);
extern int kernel_sema_destroy(int sema_id);
extern int kernel_sema_wait(int sema_id);
extern int kernel_sema_signal(int sema_id);

extern void kernel_ps();
#endif /* KERNEL_H_ */
