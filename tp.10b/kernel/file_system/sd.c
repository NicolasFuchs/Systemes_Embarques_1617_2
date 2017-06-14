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
 * Purpose:		This module implement SD card utilities.
 *
 * Author:		Daniel Gachet (first author)
 * 				Alan Sueur
 * 				Nicolas Fuchs
 *
 * Date: 		27.04.2017
 * Last update:	28.04.2017
 *
 */

#include "sd.h"
#include "ff.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define O_RDONLY        00000000
#define O_WRONLY        00000001
#define O_RDWR          00000002
#define O_CREAT         00000100 
#define O_EXCL          00000200 
#define O_NOCTTY        00000400 
#define O_TRUNC         00001000 
#define O_APPEND        00002000

static FATFS fs;

static char* fa(BYTE attr) {
	static char a[6];
	a[0] = (attr & AM_DIR) ? 'd' : '-';
	a[1] = (attr & AM_RDO) ? 'r' : '-';
	a[2] = (attr & AM_HID) ? 'h' : '-';
	a[3] = (attr & AM_SYS) ? 's' : '-';
	a[4] = (attr & AM_ARC) ? 'a' : '-';
	a[5] = 0;
	return a;
}

//#define printd(args...) printf (args)
#define printd(args...) 

// --- file services -----------------------------------------------------------

struct file {
	FIL fil;
	char name[];
};

static int sd_open (const char *name, int flags, int m)
{
	(void)m;
	BYTE mode = 0;
	if (flags==O_RDONLY) mode |= FA_READ;
	if (flags&O_WRONLY ) mode |= FA_WRITE;
	if (flags&O_RDWR   ) mode |= FA_READ | FA_WRITE;
	if (flags&O_APPEND ) mode |= FA_OPEN_APPEND;
	printd ("c_open: file=%s, flags=%x, mode=%x\n", name, flags, mode);

	struct file* f = calloc(1, sizeof(*f)+strlen(name)+1);
	if (f != 0) {
		strcpy (f->name, name);
		FRESULT r = f_open (&(f->fil), name, mode);
		if (r != FR_OK) {
			printd("error opening file\n");
			free(f);
			f = 0;
		}
	}

  	return (f == 0) ? -1 : (int)f;
}

static int sd_stat(const char* name, struct stat *st)
{
	memset (st, 0, sizeof(*st));
	if (strlen (name) == 0) {
		st->st_mode = S_IFDIR;
		st->st_mode |= S_IRWXU | S_IRWXG | S_IRWXO;
		return 0;
	}
	FILINFO fno;
	FRESULT r=f_stat(name, &fno);
	if (r == 0) {
		if (fno.fattrib & AM_DIR) st->st_mode = S_IFDIR;
				     else st->st_mode = S_IFREG;
		st->st_mode |= S_IRWXU | S_IRWXG | S_IRWXO;
		st->st_size = fno.fsize;
	}
	return (r==0) ? 0 : -1;
}

static int sd_fstat(int f, struct stat *st)
{
	return sd_stat((*(struct file*)f).name, st);
}

static int sd_read(int f, char *buff, int len)
{
	UINT lr=0;
	FRESULT r=f_read (&(*(struct file*)f).fil, buff, len, &lr);
	printd ("c_read: %d/%d\n", r, lr);
	return (r != 0) ? -1 : (int)lr;
}

static int sd_write(int f, char *buff, int len)
{
	UINT lw=0;
	FRESULT r=f_write(&(*(struct file*)f).fil, buff, len, &lw);
	return (r != 0) ? -1 : (int)lw;
}

static int sd_lseek(int f, int ptr, int dir)
{
	FSIZE_t ofs = ptr;

	switch (dir) {
	case 0 /*SEEK_SET*/:
		break;

	case 1 /*SEEK_CUR*/:
		ofs = f_tell(&(*(struct file*)f).fil) + ptr;
		break;

	case 2 /*SEEK_END*/:
		ofs = f_size(&(*(struct file*)f).fil) + ptr;
		break;
	default:
		return -1;
	}

	FRESULT r=f_lseek(&(*(struct file*)f).fil, ofs);
	if (r != 0) ofs = -1;

	return ofs;
}

static int sd_isatty(int file)
{
	(void)file;
	return -1;
}

/*
static int s_fstat(int fd, struct stat *st) 
{
	(void)fd; (void)st;
	return -1;
}
*/

static int sd_close(int f)
{
	FRESULT r=f_close(&(*(struct file*)f).fil);
	free ((void*)f);
	return (r==0) ? 0 : -1;
}


// --- directory services ------------------------------------------------------

struct directory {
	DIR directory;
	struct dirent entry;
};

static dir_t* sd_opendir (const char* name)
{
	dir_t* dir = calloc (1, sizeof(*dir));

	FRESULT res=f_opendir(&(dir->directory), name);

	if (res != FR_OK) {
		free(dir);
		return 0;
	}

	return dir;
}

static struct dirent* sd_readdir (dir_t* dirp)
{
	FILINFO fno;
	FRESULT res=f_readdir(&(dirp->directory), &fno);
	if ((res != FR_OK) || (fno.fname[0] == 0)) return 0;

	struct dirent* entry = &(dirp->entry);
	memset(entry, 0 , sizeof(*entry));
	entry->d_ino = 0;
	entry->d_type = (fno.fattrib & AM_DIR) != 0 ? DT_DIR : DT_REG;
	strncpy(entry->d_name, fno.fname, sizeof(entry->d_name)-1);
	strcpy(entry->d_attrib,fa(fno.fattrib));
	entry->d_size = fno.fsize;

	return entry;
}

static int sd_closedir (dir_t* dirp)
{
	free (dirp);
	return 0;
}

/*
static int s_stat(const char* name, struct stat *st) 
{
	memset (st, 0, sizeof(*st));
	FILINFO fno;
	FRESULT r=f_stat(name, &fno);
	if (r == 0) {
		if (fno.fattrib & AM_DIR) st->st_mode = S_IFDIR;
				     else st->st_mode = S_IFREG;
		st->st_mode |= S_IRWXU | S_IRWXG | S_IRWXO;
		st->st_size = fno.fsize;
	}
	return (r==0) ? 0 : -1;
}*/

static int sd_mkdir (const char* path, mode_t mode)
{
	(void)mode;
	FRESULT res=f_mkdir(path);
	return (res!=0) ? -1 : 0;
}

static int sd_rmdir (const char* path)
{
	int status = 0;
	if (f_unlink(path) != 0) {
		status = -1;
	}

	return status;
}

// --- internal structure ------------------------------------------------------

static struct sd_ops sd_ops = {
	.fops = {
		.open  = sd_open,
		.read  = sd_read,
		.write = sd_write,
		.lseek = sd_lseek,
		.isatty= sd_isatty,
		.fstat = sd_fstat,
		.close = sd_close,
	},
	.dops = {
		.opendir = sd_opendir,
		.readdir = sd_readdir,
		.closedir = sd_closedir,
		.stat     = sd_stat,
		.mkdir	  = sd_mkdir,
		.rmdir	  = sd_rmdir,
	},
};

static FATFS fs;

// --- public methods ----------------------------------------------------------

struct sd_ops* sd_init()
{
	static bool is_initialized = false;

	if (!is_initialized)
	{
		FRESULT r = f_mount (&fs, "/", 1);
		//printd ("sd_init: r=%d\n", r);
		if (r!=0) return 0;

		is_initialized = true;
	}

	return &sd_ops;
}
