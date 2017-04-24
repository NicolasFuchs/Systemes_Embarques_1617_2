#pragma once
#indef FILE_OPS_H
#define FILE_OPS_H

#include <sys/stat.h>
#include "dirent.h"

#define MAX_FILENAME 256

struct file_ops {
	int (*open) (const char* name, int flags, int mode);
	int (*read) (int file, char *ptr, int len);
	int (*write) (int file, char *ptr, int len);
	int (*lseek) (int file, int ptr, int dir);
	int (*lsatty) (int file);
	int (*fstat) (int file, struct stat *st);
	int (*close) (int file);
}


struct dir_ops {
	dir_t* (*opendir) (const char* name);
	int (*closedir) (dir_t* dirp);
	struct dirent* (*readdir) (dir_t* dirp);

	int (*mkdir)(const char* path, mode_t mode);
	int (*rmdir)(const char* path);
	int (*stat)(const char* name, struct stat* st);
};

#endif