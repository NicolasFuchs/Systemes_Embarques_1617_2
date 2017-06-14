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
 * Purpose:		This module implement shell utilities.
 *
 * Author:		Daniel Gachet (first author)
 * 				Alan Sueur
 * 				Nicolas Fuchs
 *
 * Date: 		27.04.2017
 * Last update:	28.04.2017
 *
 */

#include "../shell/shell.h"

#include "strings.h"
#include "vfs.h"
#include "am335x_console.h"
#include "kernel.h"
#include "console.h"
#include "ff.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define ROOT				"/"
#define CURRENT_DIR			"."
#define EOS					'\0'
#define BRACKET_OPEN		91
#define BRACKET_CLOSE		93
#define SPACE				' '
#define PROMPT				'$'
#define COMMAND_NOT_FOUND	"Command not found!\n"
#define NB_DIR_PROMPT		2
#define MAX_FILE_MAME_SIZE	256
#define MAX_FILE_SIZE		4096

#define ARRAY_LEN(x) (sizeof(x)/sizeof(x[0]))

// --- local struct ----------------------------------
typedef struct _shell_list_cmd{
	char* cmd_text;
	struct _shell_list_cmd*	slc_next;
	struct _shell_list_cmd* slc_previous;
}shell_list_cmd;

typedef struct _shell_cmd {
	char*  		name;
	char*  		help;
	char*		descr;
	bool (*fnct) (struct _shell_cmd*, int argc, char** args);
}shell_cmd;

// --- local functions declaration ------------------
shell_cmd*  shell_get_cmd(char* cmd_name);
bool 		shell_help(shell_cmd* me, int argc, char** args);
bool 		shell_ls(shell_cmd* me, int argc, char** args);
bool 		shell_ps(shell_cmd* me, int argc, char** args);
bool 		shell_fork(shell_cmd* me, int argc, char** args);
bool 		shell_clear(shell_cmd* me, int argc, char** args);
bool 		shell_mkdir(shell_cmd* me, int argc, char** args);
bool 		shell_rm(shell_cmd* me, int argc, char** args);
bool 		shell_kill(shell_cmd* me, int argc, char** args);
bool 		shell_cd(shell_cmd* me, int argc, char** args);
bool 		shell_cat(shell_cmd* me, int argc, char** args);
bool		shell_echo(shell_cmd* me, int argc, char** args);

// --- local datas ----------------------------------
static shell_cmd cmd_list[]={
		{"help","help\t[cmd name]\n\ncmd name\toptional\tcommand name wanted\n","Help for one command or list of all commands.",shell_help},
		{"ls","ls\t[directory name]\n\ndirectory name\toptional\tif empty,current directory\n\n The ls command list all files and\n directories for one directory\n","Show the contents of a structure.",shell_ls},
		{"ps","ps\tno argument\n\n Show all process and threads\n","Shor list of process/thread.",shell_ps},
		{"fork","fork\ttask_name\tnew_task_name\n\ntask_name\t\ttask name to copying\nnew_task_name\t\tnew task name\n","Duplicate a process/thread.",shell_fork},
		{"clear","clear\tno argument\n\nClear console screen\n","Empty the console screen.",shell_clear},
		{"mkdir","mkdir\tname\n\nCreate directory in specified name.","Create a directory.",shell_mkdir},
		{"rm","rmdir\tname\n\nDelete empty directory or file.","Delete empty directory or file.",shell_rm},
		{"kill","kill\tpid\n\nKill a specidief thread","Stop and delete a process/thread.",shell_kill},
		{"cd","cd\tpath\n\npath\t\tthe path to the new directory","Change current directory to new one.",shell_cd},
		{"cat","cat\tfilename\n\nfilename\t\tthe filename we want to display","Dispay file content.",shell_cat},
		{"echo","echo\ttext\necho\tfilename\t<\ttext\n","Write in file.",shell_echo},

};

static shell_list_cmd* last_slc=0; //slc = shell list command
static char buffer_shell[MAX_FILE_MAME_SIZE];

//--- local functions implementation ----------------

bool shell_args_contains(int argc, char** args, char* value)
{
	for(int i = 0; i < argc; i++)
	{
		if(strcmp(args[i], value)==0) return true;
	}
	return false;
}

// find and return a shell_cmd struct
shell_cmd* shell_get_cmd(char* cmd_name)
{
	int n=ARRAY_LEN(cmd_list)-1;
	while(n>=0)
	{
		if(strcmp(cmd_list[n].name,cmd_name) == 0)
		{
			return &(cmd_list[n]);
		}
		n--;
	}
	return 0;
}

// print on console help for commands
bool shell_help(shell_cmd* me, int argc, char** args)
{
	(void)me;
	printf("\n");
	if(argc==1)
	{
		for(uint32_t i=0 ; i<ARRAY_LEN(cmd_list) ; i++)
			printf("%s\t\t%s\n",cmd_list[i].name, cmd_list[i].descr);
	}
	else if(argc==2)
	{
		printf("Help for cmd %s\n\n",args[1]);
		printf(shell_get_cmd(args[1])->help);
	}
	else
	{
		return false;
	}

	return true;
}

// show content of a directory
bool shell_ls(shell_cmd* me, int argc, char** args)
{
	// WARNING: args[0] is command name (ls)
	(void)me;
	(void)args;

	if(argc>1)
		return false;  // for the moment only ls, maybe later ls [directoryname]  :-)

	memset(buffer_shell,0,sizeof(char)*MAX_FILE_MAME_SIZE);

	char* base=0;
	struct dir_ops*  dir_ops;

	dir_ops = vfs_get_dir_ops(buffer_shell, (const char**)(&base));

	if(vfs_current_node_is_mounted())
	{
		strcpy(buffer_shell,vfs_get_current_path_string());
	}

	dir_t* dir = dir_ops->opendir(buffer_shell);

	if (dir)
	{
		printf("\n");

		while(1)
		{
			struct dirent* entry = dir_ops->readdir(dir);
			if(!entry)break;

			printf("%s %8lu %s\n", entry->d_attrib, entry->d_size, entry->d_name);
		}
		dir_ops->closedir(dir);

		return true;
	}

//	if(base)free(base);
	return false;
}

bool shell_cd(shell_cmd* me, int argc, char** args)
{
	// WARNING: args[0] is command name (cd)
	(void)me;

	if(argc==2 && strlen(args[1])>0)
	{
		vfs_cd(args[1]);
		return true;
	}

	return false;
}

bool shell_cat(shell_cmd* me, int argc, char** args)
{
	(void)me;
	(void)argc;
	(void)args;
	if (argc < 2) return false;

	char* base=0;
	struct file_ops* file_ops = vfs_get_file_ops(args[0],(const char**)(&base));

	char* path = vfs_get_current_path_string();
	memset(buffer_shell, 0, sizeof(char)*MAX_FILE_MAME_SIZE);

	strcpy(buffer_shell, path);
	strcpy(buffer_shell+strlen(buffer_shell), "/");
	strcpy(buffer_shell+strlen(buffer_shell), args[1]);

	struct file {
		FIL fil;
		char name[];
	};

	struct file* f = (struct file*)file_ops->open(buffer_shell, FA_READ | FA_WRITE, 0);

	struct stat st;
	file_ops->fstat((int)f, &st);

	char buff[MAX_FILE_SIZE];
	memset(buff, 0, sizeof(char)*MAX_FILE_SIZE);
	file_ops->read((int)f, buff, st.st_size);

	printf("\n%s", buff);
	//printf("size : %d\n", (int)st.st_size);
	//printf("name : %s\n", f->name);

	file_ops->close((int)f);
	return true;
}


bool shell_ps(shell_cmd* me, int argc, char** args)
{
	(void)argc;
	(void)args;
	(void)me;
	kernel_ps();

	return true;
}
bool shell_fork(shell_cmd* me, int argc, char** args)
{
	(void)me;
	if(argc != 3)
	{
		return false;
	}
	if(!args[0] | !args[1] | !args[2])
	{
		return false;
	}

	return (bool)kernel_fork(args[1],args[2]);
}
bool shell_kill(shell_cmd* me, int argc, char** args)
{
	(void)me;
	if(argc != 2)
	{
		return false;
	}

	int pid = atoi(args[1]);

	return (bool)kernel_kill(pid);
}
bool shell_clear(shell_cmd* me, int argc, char** args)
{
	(void)argc;
	(void)args;
	(void)me;
	console_clear();

	return true;
}

// create a directory
bool shell_mkdir(shell_cmd* me, int argc, char** args)
{
	// WARNING: args[0] is command name (mkdir)
	(void)me;

	char* base=0;
	struct dir_ops* dir_ops;

	if(argc<2) return false;

	dir_ops = vfs_get_dir_ops(args[1], (const char**)(&base));

	if(!dir_ops)return false;

	char buff[MAX_FILE_MAME_SIZE];
	memset(buff, 0, sizeof(char)*MAX_FILE_MAME_SIZE);
	if(vfs_current_node_is_mounted()) {
		strcpy(buff, vfs_get_current_path_string());
		strcpy(buff+strlen(buff), "/");
		strcpy(buff+strlen(buff), base);
	}
	else
	{
		strcpy(buff, base);
	}

	int res = -1;

	if(base)
	{
		res = dir_ops->mkdir(buff,0);
	}

	return !(bool)res;
}

// delete a directory
bool shell_rm(shell_cmd* me, int argc, char** args)
{
	// WARNING: args[0] is command name (rm)
	(void)me;

		char* base=0;
		struct dir_ops*  dir_ops;

		if(argc<2)
			return false;

		dir_ops = vfs_get_dir_ops(args[1], (const char**)(&base));

		if(!dir_ops)return false;

		char buff[MAX_FILE_MAME_SIZE];
		memset(buff, 0, sizeof(char)*MAX_FILE_MAME_SIZE);
		if(vfs_current_node_is_mounted()) {
			strcpy(buff, vfs_get_current_path_string());
			strcpy(buff+strlen(buff), "/");
			strcpy(buff+strlen(buff), base);
		}
		else
		{
			strcpy(buff, base);
		}

		int res = -1;

		if(base)
		{
			res = dir_ops->rmdir(buff);
		}

		return !(bool)res;
}

bool shell_echo(shell_cmd* me, int argc, char** args)
{
	(void)me;
	if(!shell_args_contains(argc, args, "<"))
	{
		for(int i = 1; i < argc; i++)
		{
			printf("%s ", args[i]);
		}
		printf("\n");
		return true;
	}

	char buff[MAX_FILE_MAME_SIZE];
	memset(buff,0,sizeof(char)*MAX_FILE_MAME_SIZE);

	for(int i = 3; i < argc; i++) {
		strcpy(buff+strlen(buff),args[i]);
		if(i < argc-1)
		{
			strcpy(buff+strlen(buff)," ");
		}
	}
	strcpy(buff+strlen(buff),"\n");

	char* path = vfs_get_current_path_string();
	memset(buffer_shell, 0, sizeof(char)*MAX_FILE_MAME_SIZE);

	if(path && strlen(path)>0) strcpy(buffer_shell, path);
	strcpy(buffer_shell+strlen(buffer_shell), "/");
	strcpy(buffer_shell+strlen(buffer_shell), args[1]);

	FILE* f = fopen(buffer_shell, "wa");
	uint32_t written = fwrite((const char*)buff, sizeof(char), strlen(buff), f);
	fclose(f);

	if(written != strlen(buff)) return false;

	return true;
}

bool shell_slc_exist(char* cmd)
{
	if(!last_slc) return false;

	shell_list_cmd* candidate = last_slc;

	do
	{
		if(strcmp(candidate->cmd_text,cmd)==0)
			return true;

		candidate = candidate->slc_next;
	}
	while(candidate != last_slc);

	return false;
}

void shell_slc_pos(char* cmd)
{
	shell_list_cmd* candidate = last_slc;

	do
	{
		if(strcmp(candidate->cmd_text,cmd)==0)
		{
			last_slc = candidate;
			return;
		}

		candidate = candidate->slc_next;
	}
	while(candidate != last_slc);

}
// --- public functions ----------------------------

extern void shell_init()
{

}

extern char* shell_get_previous_slc()
{
	if(!last_slc)return 0;

	char* res = last_slc->cmd_text;
	last_slc = last_slc->slc_previous;
	return res;
}

extern char* shell_get_next_slc()
{
	if(!last_slc)return 0;

	last_slc = last_slc->slc_next;
	return last_slc->cmd_text;
}

extern void shell_add_slc(char* cmd)
{
	if(shell_slc_exist(cmd))
	{
		shell_slc_pos(cmd);
		return;
	}

	shell_list_cmd* new_slc = calloc(1,sizeof(shell_list_cmd));
	new_slc->cmd_text = calloc(1,sizeof(char) * strlen(cmd)+1);
	strcpy(new_slc->cmd_text,cmd);

	if(last_slc)
	{
		new_slc->slc_next = last_slc->slc_next;
		new_slc->slc_previous = last_slc;
		last_slc->slc_next = new_slc;
		new_slc->slc_next->slc_previous = new_slc;
	}
	else
	{
		new_slc->slc_next = new_slc;
		new_slc->slc_previous = new_slc;
	}

	last_slc = new_slc;

	return;
}

extern int shel_get_prompt_len()
{
	char* prompt = vfs_get_path_prompt(NB_DIR_PROMPT);
	int res = strlen(prompt);
	free(prompt);

	return res+4;
}

extern void shell_print_prompt()
{
	int i=0;
	am335x_console_putc(BRACKET_OPEN);

	char* prompt = vfs_get_path_prompt(NB_DIR_PROMPT);
	while(prompt[i])
		am335x_console_putc(prompt[i++]);

	free(prompt);

	am335x_console_putc(BRACKET_CLOSE);
	am335x_console_putc(PROMPT);
	am335x_console_putc(SPACE);
}

extern bool shell_execute(int argc, char** args)
{
	if(!argc)
	{
		return false;
	}

	int n=ARRAY_LEN(cmd_list)-1;
	while(n>=0)
	{
		if(strcmp(cmd_list[n].name,args[0]) == 0)
		{
			return cmd_list[n].fnct(&(cmd_list[n]),argc,args);
		}
		n--;
	}
	printf(COMMAND_NOT_FOUND);
	return false;
}

