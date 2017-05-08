#pragma once
#ifndef FILE_OPS_H
#define FILE_OPS_H

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

