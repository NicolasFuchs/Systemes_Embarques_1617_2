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
 * Purpose:		This module implement the virtual file system
 *
 * Author:		Daniel Gachet (first author)
 * 				Alan Sueur
 * 				Nicolas Fuchs
 *
 * Date: 		27.04.2017
 * Last update:	28.04.2017
 *
 */

#include "vfs.h"
#include "sd.h"
#include "strings.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#define MAX_FILENAME 	256
#define EOS				'\0'
#define DIR_SEP			'/'
#define CURRENT_DIR		"."
#define PREVIOUS_DIR	".."
#define ROOT_DIR		"/"
#define SDCARD			"sdcard"
#define DEV				"dev"
#define SYS				"sys"
#define PROC			"proc"
#define DIR_SEP_STRING  "/"

// --- forward declaration -----------------------------------------------------
static int vfs_rmdir (const char* path);
static int vfs_open (const char *name, int flags, int m);
static int vfs_read(int fd, char *buff, int len);
static int vfs_write(int fd, char *buff, int len);
static int vfs_lseek(int fd, int offset, int whence);
static int vfs_isatty(int fd);
static int vfs_fstat(int fd, struct stat *st);
static int vfs_close(int fd);
static dir_t* vfs_opendir (const char* name);
static struct dirent* vfs_readdir (dir_t* dirp);
static int vfs_closedir (dir_t* dirp);
static int vfs_stat(const char* name, struct stat *st);
static int vfs_mkdir (const char* path, mode_t mode);

// --- internal structure ------------------------------------------------------
// struct to keep files and directories operations for all vfs nodes
static struct vfs_ops vfs_ops = {
	.fops = {
		.open  = vfs_open,
		.read  = vfs_read,
		.write = vfs_write,
		.lseek = vfs_lseek,
		.isatty= vfs_isatty,
		.fstat = vfs_fstat,
		.close = vfs_close,
	},
	.dops = {
		.opendir = vfs_opendir,
		.readdir = vfs_readdir,
		.closedir = vfs_closedir,
		.stat     = vfs_stat,
		.mkdir	  = vfs_mkdir,
		.rmdir	  = vfs_rmdir,
	},
};

// different type of node (multiple type possible)
enum node_type{	NODE_FILE= (1<<0),
				NODE_DIRECTORY = (1<<1),
				NODE_MOUNTED = (1<<2)};

// the vfs node
typedef struct dnode {
	char name[MAX_FILENAME];		// name of node (not path, only name)
	char vfs_name[MAX_FILENAME];	// name as vfs node (not path, only name)

	enum node_type node_type;

	struct dir_ops* dops;	// functions pointers for directory operations
	struct file_ops* fops;	// functions pointers for files operations

	struct dnode* b_node;	// brother node
	struct dnode* c_node;	// child node
	struct dnode* p_node;	// parent node

	const char* (*get_mounted_name) (struct dnode* node);	// function pointer
}dnode_t;

// function pointer type
typedef const char* (vfs_get_mounted_name_t)(dnode_t* node);

// --- forward declaration --------------------------------------
static const char* vfs_get_mounted_name(dnode_t* node);

// --- internal variables ------------------------------------------------------
// vfs root node
static dnode_t root = {
	.name = ROOT_DIR,
	.vfs_name = ROOT_DIR,
	.node_type = NODE_DIRECTORY,
	.fops = 0,
	.dops = 0,
	.b_node = 0,
	.c_node = 0,
	.p_node = 0,
	.get_mounted_name = vfs_get_mounted_name,
};

static dnode_t* current_node = &root;		// the last node in actual current path
static char current_path[MAX_FILENAME];		// the current path from current_node,
											// null if vfs_node, else path from mounted node

// --- private methods ---------------------------------------------------------
// return name or vfs_name (id node is mounted or not)
// ex: the /sdcard node return "/", not "sdcard"
static const char* vfs_get_mounted_name(dnode_t* node)
{
	if(node->node_type & NODE_MOUNTED)
		return (const char*)node->name;

	return (const char*)node->vfs_name;
}

// return the node of a path (0 if not founded)
// if last_valid = true then return the last valid instead 0
static dnode_t* vfs_get_node(const char* path, bool last_valid)
{
	strings_t p = strings_split(path,DIR_SEP);

	if(p.size==0)
	{
		strings_free_strings_t(&p);
		if(last_valid)
			return current_node;

		return 0;
	}

	dnode_t* candidate = current_node;

	int n=0;
	if(strcmp(p.strings[0], ROOT_DIR) == 0)
	{
		candidate = &root;
		n=1;
	}

	while(n < p.size)
	{
		if(strcmp(p.strings[n],PREVIOUS_DIR)==0)
		{
			if(candidate->p_node)candidate = candidate->p_node;
		}
		else if(strcmp(p.strings[n],CURRENT_DIR)!=0)
		{
			dnode_t* former = candidate;
			dnode_t* new = candidate->c_node;

			while(new)
			{
				if(strcmp(new->vfs_name,p.strings[n]) == 0)
				{
					candidate = new;
					new = 0;
				}
				else
				{
					new = new->b_node;
				}
			}

			if(former == candidate)
			{
				if(last_valid)
				{
					strings_free_strings_t(&p);
					return candidate;
				}

				strings_free_strings_t(&p);
				return 0;
			}
		}

		n++;
	}

	strings_free_strings_t(&p);
	return candidate;
}

// =.=.=.=.==.=.=.=.==.=.=.=.==.=.=.=.==.=.=.=.==.=.=.=.==.=.=.=.==.=.=.=.=



// return the last dir_name or the file_name of a path (return 0 if not found)
static const char* vfs_get_last_part_of(const char* path)
{
	if(!path)return 0;

	int i=strlen(path);

	if(i==0)return 0;
	if(i==1 && path[0]==DIR_SEP)return path;

	i--;
	while(i>=0 && path[i]!=DIR_SEP)i--;

	if(i<0)i=0;
	if(path[i]==DIR_SEP)i++;

	return (path + i);
}

// service function (recursive) for vfs_get_path_prompt
static char* vfs_get_path_prompt_service(dnode_t* node, int nb_dir, int len, int nb_dir_total)
{
	int len_name = strlen(node->vfs_name);
	nb_dir--;
	nb_dir_total++;

	if(nb_dir <= 0 || !node->p_node)
	{
		char* path = calloc(1,sizeof(char)*(len_name+len) + nb_dir_total + 1 );
		strcpy(path,node->vfs_name);
		return path;
	}

	char* path = vfs_get_path_prompt_service(node->p_node, nb_dir, len_name + len, nb_dir_total);
	int start = strlen(path);

	if(start>0 && path[start-1] != DIR_SEP)
	{
		strcpy(path+start,DIR_SEP_STRING);
		start++;
	}

	strcpy(path+start,node->vfs_name);

	return path;
}

// --- file services -----------------------------------------------------------
static int vfs_open (const char *name, int flags, int m)
{
	(void)m; (void)name; (void)flags;

	// TODO : to be implemented
  	return -1;
}
static int vfs_read(int fd, char *buff, int len)
{
	(void)fd; (void)buff; (void)len;

	// TODO : to be implemented
	return -1;
}
static int vfs_write(int fd, char *buff, int len)
{
	(void)fd; (void)buff; (void)len;

	// TODO : to be implemented
	return -1;
}
static int vfs_lseek(int fd, int offset, int whence)
{
	(void)fd; (void)offset; (void)whence;

	// TODO : to be implemented
	return -1;
}
static int vfs_isatty(int fd)
{
	(void)fd;

	// TODO : to be implemented
	return -1;
}
static int vfs_fstat(int fd, struct stat *st)
{
	(void)fd; (void)st;

	// TODO : to be implemented
	return -1;
}
static int vfs_close(int fd)
{
	(void)fd;

	// TODO : to be implemented
	return -1;
}
// --- directory services ------------------------------------------------------
/* Directory object structure (DIR) */

/* Directory object structure (DIR) */

typedef struct {
	dnode_t*	dptr;
	int			entry_pos;
} DIR;

struct directory {
	DIR directory;
	struct dirent entry;
};

static dir_t* vfs_opendir (const char* name)
{
	dnode_t* node = vfs_get_node(name, true);

	if(!(node->node_type & NODE_DIRECTORY))return 0;

	dir_t* dir = calloc (1, sizeof(*dir));

	dir->directory.dptr = node;
	dir->directory.entry_pos = 0;

	return dir;
}

static struct dirent* vfs_readdir (dir_t* dirp)
{
	dnode_t* node = (dnode_t*)(dirp->directory.dptr);
	node = node->c_node;

	int i=0;
	while(i<dirp->directory.entry_pos && node)
	{
		node = node->b_node;
		i++;
	}

	if(!node)return 0;

	dirp->directory.entry_pos++;
	strcpy(dirp->entry.d_name,node->vfs_name);

	if(node->node_type == NODE_FILE)
	{
		dirp->entry.d_size = 0;
		dirp->entry.d_attrib[0] = '-';
		dirp->entry.d_attrib[1] = '-';
		dirp->entry.d_attrib[2] = '-';
		dirp->entry.d_attrib[3] = '-';
		dirp->entry.d_attrib[4] = 'f';
		dirp->entry.d_attrib[5] = '\0';
	}
	else
	{
		dirp->entry.d_size = 0;
		dirp->entry.d_attrib[0] = 'd';
		dirp->entry.d_attrib[1] = '-';
		dirp->entry.d_attrib[2] = '-';
		dirp->entry.d_attrib[3] = '-';
		dirp->entry.d_attrib[4] = '-';
		dirp->entry.d_attrib[5] = '\0';
	}

	struct dirent* res = &(dirp->entry);
	return res;
}

static int vfs_closedir (dir_t* dirp)
{
	free (dirp);

	return 0;
}
static int vfs_stat(const char* name, struct stat *st)
{
	(void)name;(void)st;

	// TODO : to be implemented
	return -1;
}
static int vfs_mkdir (const char* path, mode_t mode)
{
	(void)mode;

	const char* new_dir = vfs_get_last_part_of(path);
	if(!new_dir) return NO_DIR_NAME_VALID;
	if(strcmp(new_dir,ROOT_DIR)==0)return NO_DIR_NAME_VALID;

	dnode_t* node = vfs_get_node(path, true);

	if(!node)return INVALID_PATH;

	dnode_t* new_node = calloc(1,sizeof(*new_node));
	new_node->node_type = NODE_DIRECTORY;
	new_node->p_node = node;
	new_node->dops = node->dops;
	new_node->fops = node->fops;
	strcpy(new_node->name,new_dir);
	strcpy(new_node->vfs_name,new_dir);

	if(node->c_node)
		new_node->b_node = node->c_node;

	node->c_node = new_node;

	return 0;
}
static int vfs_rmdir (const char* path)
{
	(void)path;

	dnode_t* node = vfs_get_node(path, true);
	if(node->c_node) return false;
	dnode_t* p = node->p_node;
	if(!p) return false;
	dnode_t* candidate = p->c_node;
	if(candidate==node)
	{
		p->c_node = candidate->b_node;
		free(candidate);
		return 0;
	}
	dnode_t* b = candidate;
	candidate = candidate->b_node;
	while(candidate)
	{
		if(candidate==node)
		{
			b->b_node = candidate->b_node;
			free(candidate);
			return 0;
		}
		else
		{
			b = candidate;
			candidate = candidate->b_node;
		}
	}
	return -1;
}

// --- public methods ----------------------------------------------------------
int vfs_init()
{
	memset(current_path,0,sizeof(char)*MAX_FILENAME);

	// --------------- create system tree ------------------------
	// root
	root.fops = &(vfs_ops.fops);
	root.dops = &(vfs_ops.dops);

	// /sdcard
	struct sd_ops* sd_ops = sd_init();
	vfs_mount("/sdcard",&(sd_ops->dops),&(sd_ops->fops));

	// /dev
	vfs_mkdir("/dev",0);

	// TODO:
	// /dev/buzzer
	//struct buzzer_ops* buzzer_ops = buzzer_init();
	//vfs_mount("/dev/buzzer",&(buzzer_ops->dops),&(buzzer_ops->fops));

	// TODO:
	// /dev/thermo
	//struct thermo_ops* thermo_ops = thermo_init();
	//vfs_mount("/dev/thermo",&(thermo_ops->dops),&(thermo_ops->fops));

	// /sys  TODO: acces to drivers datas
	vfs_mkdir("/sys",0);

	// /proc TODO: optional: acces to kernel datas
	vfs_mkdir("/proc",0);

	// /usr  TODO: optional: user management (grants, login, pswd) (maybe later :-)  )
	vfs_mkdir("/usr",0);
	vfs_mkdir("/usr/jige",0);
	vfs_mkdir("/usr/karim",0);
	vfs_mkdir("/usr/default",0);

	return 0;
}

// return path for prompt
// nb_dir	int		max number of directories in returned path (from end)
extern char* vfs_get_path_prompt(int nb_dir)
{
	if(current_node->node_type & NODE_MOUNTED)
	{
		char* vfs_part = vfs_get_path_prompt_service(current_node, nb_dir, 0, nb_dir);

		int start = strlen(vfs_part);
		int len = strlen(current_path);

		char* res = calloc(1,sizeof(char)*(start+len+2));

		strcpy(res,vfs_part);

		if(len>0)
		{
			len = strlen(res);
			strcpy(res+len,current_path);
		}

		free(vfs_part);

		return res;
	}
	else
	{
		return vfs_get_path_prompt_service(current_node, nb_dir, 0, nb_dir);
	}
}

extern int vfs_mount(const char* path, struct dir_ops* dops, struct file_ops* fops)
{
	const char* mount_name = vfs_get_last_part_of(path);
	if(strcmp(mount_name,DIR_SEP_STRING)==0)return -1;
	if(strcmp(mount_name,ROOT_DIR)==0)return -1;

	dnode_t* node = vfs_get_node(path, true);

	if(!node)return INVALID_PATH;

	dnode_t* mount = calloc(1,sizeof(dnode_t));

	if(dops)
		mount->node_type = NODE_DIRECTORY + NODE_MOUNTED;
	else
		mount->node_type = NODE_FILE + NODE_MOUNTED;

	mount->p_node = node;
	mount->dops = dops;
	mount->fops = fops;
	strcpy(mount->name,ROOT_DIR);
	strcpy(mount->vfs_name,mount_name);

	if(node->c_node)
		mount->b_node = node->c_node;

	node->c_node = mount;

	return 0;
}

extern void vfs_cd(char* new_path)
{
	dnode_t* candidate = current_node;

	if(candidate->node_type & NODE_MOUNTED)
	{
		if(strcmp(new_path,"..")==0)
		{
			if(strlen(current_path)==0)
			{
				if(current_node->p_node)
					current_node = current_node->p_node;

				return;
			}

			int n = strlen(current_path)-1;
			while(n>=0 && current_path[n]!=DIR_SEP)
			{
				current_path[n--]=EOS;
			}

			current_path[n]=EOS;
			return;
		}
		else
		{
			int len = strlen(current_path);
			strcpy(current_path+len,DIR_SEP_STRING);
			len += strlen(DIR_SEP_STRING);
			strcpy(current_path+len,new_path);
		}
	}
	else
	{
		if(strcmp(new_path,"..")==0)
		{
			if(current_node->p_node)
				current_node = current_node->p_node;

			return;
		}
		else
		{
			candidate = candidate->c_node;

			while(candidate)
			{
				if(strcmp(candidate->vfs_name,new_path)==0)
				{
					current_node = candidate;
					return;
				}
				candidate = candidate->b_node;
			}
		}
	}
}

// find and return files operations and base path
extern struct file_ops* vfs_get_file_ops(const char* name, const char* *base)
{
	*base = name;
	return current_node->fops;
}

// find and return directory operations and base path
extern struct dir_ops* vfs_get_dir_ops(const char* path, const char* *base)
{
	*base = path;
	return current_node->dops;
}

// retur n true if current_node is mounted, else false
extern bool vfs_current_node_is_mounted()
{
	return current_node->node_type & NODE_MOUNTED;
}

// return path part of a mounted node
extern char* vfs_get_current_path_string()
{
	return current_path;
}

