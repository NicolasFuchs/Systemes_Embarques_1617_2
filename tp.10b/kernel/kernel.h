#pragma once
#ifndef KERNEL_H
#define KERNEL_H
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
 * Project:	HEIA-FR / Embedded Systems 2 
 *
 * Abstract: 	Introduction to Operating System
 *
 * Purpose:	Micro kernel service definition
 *
 * Autĥor:	Daniel Gachet / HEIA-FR
 * Date:	27.03.2017
 */

typedef void (*kernel_thread_t) (void* param);

extern void kernel_init();
extern void kernel_launch();

extern int  kernel_thread_create (kernel_thread_t thread, void* param, const char* name, uint32_t stack_size);
extern void kernel_thread_exit();
extern void kernel_thread_yield();
extern const char* kernel_thread_name();
extern int kernel_thread_id();

extern int  kernel_sema_create (int initial_value);
extern int  kernel_sema_destroy (int sema_id);
extern int  kernel_sema_wait (int sema_id);
extern int  kernel_sema_signal (int sema_id);

#endif
