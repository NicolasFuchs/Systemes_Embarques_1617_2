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
 * Date: 	22.05.2017
 */

/* GNU ARM Toolchain & newlib: system calls */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "am335x_clock.h"
#include "am335x_console.h"
#include "kernel.h"
#include "vfs.h"
#include "cons.h"

#define ARRAY_SIZE(x) ((int)(sizeof(x)/sizeof(x[0])))

static struct file {
	struct file_ops* op;
	int id;
} f[1024];

int _open (const char *name, int flags, int mode)
{
	int i=0;
	while ((f[i].op!=0) && (i<ARRAY_SIZE(f))) i++;
	if (i >= ARRAY_SIZE(f)) return -1;

	const char* fname;
	struct file_ops* fops = vfs_get_file_ops(name, &fname);
	if (fops == 0) return -1;

	int id = fops->open (fname, flags, mode);
	if (id == -1) return -1;

	f[i].id = id;
	f[i].op = fops;

  	return i;
}

int _read(int fd, char *ptr, int len) {
	return f[fd].op->read (f[fd].id, ptr, len);
}

int _write(int fd, char *ptr, int len) {
	return f[fd].op->write (f[fd].id, ptr, len);
}

int _lseek(int fd, int offset, int whence) {
	return f[fd].op->lseek(f[fd].id, offset, whence);
}

int _isatty(int fd) {
	return f[fd].op->isatty(f[fd].id);
}

int _close(int fd) {
	int res = f[fd].op->close(f[fd].id);
	f[fd].op = 0;
	return res;
}

int _fstat(int fd, struct stat *st) {
	return f[fd].op->fstat(f[fd].id, st);
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
    kernel_thread_exit();
    while(1);
}

void _init()
{
	am335x_clock_enable_l3_l4wkup();
	am335x_console_init();
	struct file_ops* fops = cons_init();
	f[0].op=fops; f[0].id=0;
	f[1].op=fops; f[1].id=1;
	f[2].op=fops; f[2].id=2;
}

void _fini()
{
}

int _getpid ()
{
	// return the running thread id
	return kernel_thread_id();
}

int _kill (int pid __attribute__((unused)), int sig __attribute__((unused))) 
{
	kernel_thread_exit();
	return 0;
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


