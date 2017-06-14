#pragma once
#ifndef FILE_SYSTEM_DIRENT_H
#define FILE_SYSTEM_DIRENT_H

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
 * Purpose:		This module implement directory utilities.
 *
 * Author:		Daniel Gachet (first author)
 * 				Alan Sueur
 * 				Nicolas Fuchs
 *
 * Date: 		27.04.2017
 * Last update:	28.04.2017
 *
 */

#include <stdint.h>


typedef struct directory dir_t;

enum filetype {
    DT_UNKNOWN,
    DT_FIFO,
    DT_CHR,
    DT_DIR,
    DT_BLK,
    DT_REG,
    DT_LNK,
    DT_SOCK,
    DT_WHT
  };

struct dirent {
    uint32_t       d_ino;       /* inode number */
    enum filetype  d_type;      /* type of file; not supported
                                   by all file system types */
    char           d_name[256]; /* filename */
    char		   d_attrib[6]; /* atributes */
    long		   d_size;		/* size (octets) */
};

extern dir_t* opendir (const char* name);
extern struct dirent *readdir (dir_t* dirp);
extern int closedir (dir_t* dirp);

extern int rmdir(const char* name);

#endif //FILE_SYSTEM_DIRENT_H
