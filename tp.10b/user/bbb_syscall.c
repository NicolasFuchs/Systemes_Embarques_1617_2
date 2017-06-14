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
 * Project:	HEIA-FR / Embedded Systems 2 Laboratory
 *
 * Abstract: 	newlib system calls for processes based on HEIA-FR kernel
 *
 * Purpose:	This module implements all the required newlib system calls
 * 		for processes based on HEIA-FR kernel
 *
 * Author: 	Daniel Gachet
 * Date: 	22.05.2017
 */

/* GNU ARM Toolchain & newlib: system calls */
#include <stdlib.h>
#include <stdio.h>

#include "syscall.h"

int _read(int file, char *ptr, int len) {
	return syscall(file, (int)ptr, len, 0, SYSCALL_READ);
}

int _write(int file, char *ptr, int len) {
	return syscall(file, (int)ptr, len, 0, SYSCALL_WRITE);
}

int _lseek(int file, int ptr, int dir) {
	return syscall(file, (int)ptr, dir, 0, SYSCALL_LSEEK);
}

int _isatty(int file) {
	return syscall(file,0 ,0, 0, SYSCALL_ISATTY);
}

#include <sys/stat.h>
int _fstat(int file, struct stat *st) {
	return syscall(file, (int)st, 0, 0, SYSCALL_FSTAT);
}

int _close(int file) {
	return syscall(file, 0, 0, 0, SYSCALL_CLOSE);
}

void* _sbrk(int incr) {
	extern char _end; /* Defined by the linker */
	extern char _stack_bottom;
	static char *heap_end;
	char *prev_heap_end;

	if (heap_end == 0) {
	  heap_end = &_end;
	}
	prev_heap_end = heap_end;
	if (heap_end + incr > &_stack_bottom) {
		return 0;
	} else {
		heap_end += incr;
	}
	return (void*) prev_heap_end;
}

void _exit(int i)
{
	syscall(i,0,0,0, SYSCALL_EXIT);
	while(1);
}

void _init()
{
}

void _fini()
{
}

int _getpid ()
{
	return syscall(0,0,0,0, SYSCALL_GETPID);
}

int _kill (int pid, int sig)
{
	return syscall(pid,sig,0,0, SYSCALL_KILL);
}
