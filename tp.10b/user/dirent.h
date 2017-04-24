#pragma once
#indef DIRENT_H
#define DIRENT_H

#include <stdint.h>

typedef struct directory dir_t;

enum filetype {
	DT_UNKNOWN,
	DT_FIFO,
	DT_CHR,
	DT_BLK,
	DT_REG,
	DT_LNK,
	DT_SOCK,
	DT_WHT
};

struct dirent {
	uint32_t d_ino; //inode number
	unum filetype d_type; // type of file
	char d_name[256]; // filenmae
};

extern dir_t* opendir (const char* name);
extern struct dirent *readdir (dir_t* dirp);
extern int closedir (dir_t* dirp);

extern int rmdir (const char* name);

#endif