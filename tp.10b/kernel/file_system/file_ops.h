#pragma once
#ifndef FILE_OPS_H
#define FILE_OPS_H

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
 * Purpose:		This module implement file/directory operations.
 *
 * Author:		Daniel Gachet (first author)
 * 				Alan Sueur
 * 				Nicolas Fuchs
 *
 * Date: 		27.04.2017
 * Last update:	28.04.2017
 *
 */

#include <sys/stat.h>
#include "dirent.h"

struct file_ops {
	int (*open)  (const char* name, int flags, int mode);
	int (*read)  (int fd, char *ptr, int len);	
	int (*write) (int fd, char *ptr, int len);
	int (*lseek) (int fd, int offset, int whence);
	int (*isatty)(int fd);
	int (*fstat) (int fd, struct stat *st);
	int (*close) (int fd);
};

struct dir_ops {
	dir_t* (*opendir) (const char* name);
	int (*closedir) (dir_t* dirp);
	struct dirent* (*readdir) (dir_t* dirp);

	int (*stat) (const char* name, struct stat* st);
	int (*mkdir)(const char* path, mode_t mode);
	int (*rmdir)(const char* path);
};

#endif

