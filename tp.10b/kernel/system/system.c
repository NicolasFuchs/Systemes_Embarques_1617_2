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
 * Purpose:		This module implement system operations.
 *
 * Author:		Daniel Gachet (first author)
 * 				Alan Sueur
 * 				Nicolas Fuchs
 *
 * Date: 		27.04.2017
 * Last update:	28.04.2017
 *
 */

#include "system.h"
#include "syscall.h"

extern int system_write(int file, char*ptr, int len)
{
	return syscall(file, (int)ptr, len, 0, SYSCALL_WRITE);
}

extern void system_yield()
{
	syscall(0, 0, 0, 0, SYSCALL_YIELD);
}
