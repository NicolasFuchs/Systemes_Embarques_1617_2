#pragma once
#ifndef dirent_h
#define dirent_h

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
};

extern dir_t* opendir (const char* name);
extern struct dirent *readdir (dir_t* dirp);
extern int closedir (dir_t* dirp);

extern int rmdir(const char* name);

#endif
