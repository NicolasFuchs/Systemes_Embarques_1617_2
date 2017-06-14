#pragma once
#ifndef VFS_H
#define VFS_H

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
 * Purpose:		This module implement console utilities.
 *
 * Author:		Daniel Gachet (first author)
 * 				Alan Sueur
 * 				Nicolas Fuchs
 *
 * Date: 		27.04.2017
 * Last update:	28.04.2017
 *
 */

#include <stdbool.h>

#include "file_ops.h"

#define NO_DIR_NAME_VALID	1;
#define INVALID_PATH		2;

// struct to return pointers on files operations (fops) and directories operations (dops)
struct vfs_ops {
	struct file_ops fops;
	struct dir_ops  dops;
};

extern int vfs_init();

extern bool vfs_current_node_is_mounted();
extern char* vfs_get_current_path_string();

// return path for prompt
// nb_dir	int		number of directories name include in prompt (~ to replace dir not showed)
extern char* vfs_get_path_prompt(int nb_dir);

extern int vfs_mount(const char* path, struct dir_ops* dops, struct file_ops* fops);
extern void vfs_cd(char* new_path);

extern struct file_ops* vfs_get_file_ops(const char* name, const char* *base);
extern struct dir_ops*  vfs_get_dir_ops (const char* name, const char* *base);

#endif

