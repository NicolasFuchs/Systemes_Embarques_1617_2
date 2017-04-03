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
 * Abstract: 	newlib system calls
 *
 * Purpose:	This module implements all the required newlib system calls
 *
 * Author: 	Daniel Gachet
 * Date: 	03.04.2017
 */


/* GNU ARM Toolchain & newlib: system calls */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "am335x_clock.h"
#include "am335x_console.h"

#include "vfs.h"


int _read(int file __attribute__((unused)), char *ptr, int len __attribute__((unused))) {
	*ptr = am335x_console_getc();
	return 1;
}

int _write(int file __attribute__((unused)), char *ptr, int len) {
	int todo;
	for (todo = 0; todo < len; todo++) {
		am335x_console_putc(*ptr++);
	}
	return len;
}

int _lseek(int file __attribute__((unused)), int ptr __attribute__((unused)), int dir __attribute__((unused))) {
	return 0;
}

int _isatty(int file __attribute__((unused))) {
	return 1;
}

#include <sys/stat.h>
int _fstat(int file __attribute__((unused)), struct stat *st) {
	st->st_mode = S_IFCHR;
	return 0;
}

int _close(int file __attribute__((unused))) {
	return -1;
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

void _exit(int i __attribute__((unused)))
{
    while(1);
}

void _init()
{
	am335x_clock_enable_l3_l4wkup();
	am335x_console_init();
}

void _fini()
{
}

int _getpid ()
{
	return 1;
}

int _kill (int pid __attribute__((unused)), int sig __attribute__((unused)))
{
	while(1);
}


// --- directory services ------------------------------------------------------

struct s_directory {
	void* dirp;
	struct dir_ops* dops;
};

dir_t* _opendir (const char* name)
{
	const char* fname;
	struct dir_ops* dops = vfs_get_dir_ops(name, &fname);
	if (dops == 0) return 0;

	struct s_directory* dir = calloc(1, sizeof(*dir));
	
	dir->dops = dops;
	dir->dirp = dir->dops->opendir(fname);

	return (dir_t*)dir;
}

struct dirent* _readdir (dir_t* dirp)
{
	struct s_directory* dir = (struct s_directory*)dirp;
	return dir->dops->readdir(dir->dirp);
}

int _closedir (dir_t* dirp)
{
	struct s_directory* dir = (struct s_directory*)dirp;
	dir->dops->closedir(dir->dirp);
	free(dir);
	return 0;
}

int _stat (const char *path, struct stat *st) 
{
	const char* fname;
	char name[strlen(path)+1];
	strcpy(name, path);
	char* d = strrchr(name,'/');
	if (d != 0) *d = 0;

	struct dir_ops* dops = vfs_get_dir_ops(name, &fname);
	if (dops == 0) return 0;

	if (d != 0) *d = '/';
	
	return dops->stat (fname, st);
}


