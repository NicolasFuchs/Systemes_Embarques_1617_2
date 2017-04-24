#pragma once
#ifndef VFS_H
#define VFS_H

#include "file_ops.h"

extern int vfs_init();

extern int vfs_mount(const char* path, struct dir_ops* dops, struct file_ops* fops);
extern int vfs_mkdir(const char* name);
extern int vfs_rmdir(const char* name);

extern struct file_ops* vfs_get_file_ops(const char* name, const char* *base);
extern struct dir_ops*  vfs_get_dir_ops (const char* name, const char* *base);

#endif

