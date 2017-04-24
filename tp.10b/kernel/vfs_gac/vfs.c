#include "vfs.h"
#include <string.h>
#include <stdlib.h>


#define MAX_FILENAME 256


// --- forward declarations ----------------------------------------------------


// --- internal structure ------------------------------------------------------

struct dnode {
	char name[MAX_FILENAME];

	struct dir_ops* dops;
	struct file_ops* fops;

	struct dnode* b_node;
	struct dnode* c_node;
};

// --- internal variables ------------------------------------------------------

static struct dnode root = {
	.name = "/",
	.fops = 0,
	.dops = 0,
};

// --- public methods ----------------------------------------------------------


int vfs_init()
{
	return 0;
}

int vfs_mount(const char* path, struct dir_ops* dops, struct file_ops* fops)
{
	(void)path;
	root.dops = dops;
	root.fops = fops;
	return 0;
}

int vfs_mkdir(const char* name)
{
	(void)name;
	return -1;
}

int vfs_rmdir(const char* name)
{
	(void)name;
	return -1;
}

struct file_ops* vfs_get_file_ops(const char* name, const char* *base)
{
	*base = name;
	return root.fops;
}

struct dir_ops* vfs_get_dir_ops(const char* name, const char* *base)
{
	*base = name;
	return root.dops;
}

