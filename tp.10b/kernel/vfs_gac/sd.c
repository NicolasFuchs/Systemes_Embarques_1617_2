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

//#define printd(args...) printf (args)
#define printd(args...) 

// --- file services -----------------------------------------------------------

struct file {
	FIL fil;
	char name[];
};

static int s_open (const char *name, int flags, int m) 
{
	(void)m; 
	(void)name; (void)flags;
  	return -1;  
} 

static int s_read(int fd, char *buff, int len) 
{
	(void)fd; (void)buff; (void)len;
	return -1;
}

static int s_write(int fd, char *buff, int len) 
{
	(void)fd; (void)buff; (void)len;
	return -1;
}

static int s_lseek(int fd, int offset, int whence) 
{
	(void)fd; (void)offset; (void)whence;
	return -1;
}

static int s_isatty(int fd) {
	(void)fd;
	return -1;
}

static int s_fstat(int fd, struct stat *st) 
{
	(void)fd; (void)st;
	return -1;
}

static int s_close(int fd) 
{
	(void)fd;
	return -1;
}


// --- directory services ------------------------------------------------------

struct directory {
	DIR directory;
	struct dirent entry;
};

static dir_t* s_opendir (const char* name) 
{
	dir_t* dir = calloc (1, sizeof(*dir));

	FRESULT res=f_opendir(&(dir->directory), name);

	if (res != FR_OK) {
		free(dir);
		return 0;
	}

	return dir;
}

static struct dirent *s_readdir (dir_t* dirp)
{
	FILINFO fno;
	FRESULT res=f_readdir(&(dirp->directory), &fno);
	if ((res != FR_OK) || (fno.fname[0] == 0)) return 0;

	struct dirent* entry = &(dirp->entry);
	memset(entry, 0 , sizeof(*entry));
	entry->d_ino = 0;
	entry->d_type = (fno.fattrib & AM_DIR) != 0 ? DT_DIR : DT_REG;
	strncpy(entry->d_name, fno.fname, sizeof(entry->d_name)-1);

	return entry;
}

static int s_closedir (dir_t* dirp)
{
	free (dirp);
	return 0;
}


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
}


static int s_mkdir (const char* path, mode_t mode)
{
	(void)mode;
	FRESULT res=f_mkdir(path);
	return (res!=0) ? -1 : 0;
}

static int s_rmdir (const char* path)
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
		.open  = s_open,
		.read  = s_read,
		.write = s_write,
		.lseek = s_lseek,
		.isatty= s_isatty,
		.fstat = s_fstat,
		.close = s_close,
	},
	.dops = {
		.opendir = s_opendir,
		.readdir = s_readdir,
		.closedir = s_closedir,
		.stat     = s_stat,
		.mkdir	  = s_mkdir,
		.rmdir	  = s_rmdir,
	},
};

static FATFS fs;

// --- public methods ----------------------------------------------------------

struct sd_ops* sd_init()
{
	static bool is_initialized = false;
	if (!is_initialized) {
		FRESULT r = f_mount (&fs, "/", 1);
		printd ("sd_init: r=%d\n", r);
		if (r!=0) return 0;

		is_initialized = true;
	}

	return &sd_ops;
}
