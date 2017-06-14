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
 * Purpose:		This module implement console utilities.
 *
 * Author:		Daniel Gachet (first author)
 * 				Alan Sueur
 * 				Nicolas Fuchs
 *
 * Date: 		27.04.2017
 * Last update:	28.04.2017
 *
 */

#include "console.h"
#include "am335x_console.h"
#include "strings.h"
#include "system.h"

#include <string.h>
#include <stdio.h>
#include "../shell/shell.h"

#define DIR_SEP			'/'
#define SPACE			' '
#define DEL				'\b'
#define CLEAR_SCREEN	'\f'
#define TAB				'\t'
#define RETURN			'\r'
#define NEW_LINE		'\n'
#define C_RETURN		13
#define CTRL_C			3
#define ESCAPE			27
#define BRACKET			91
#define TILDE			126
#define ARROW_UP		65
#define ARROW_DOWN		66
#define ARROW_RIGHT		67
#define ARROW_LEFT		68

#define BUFFER_SIZE		1000
#define CHAR_MEM_SIZE	4

// --- local variables -------------------------------
static int cursor_pos;				// position of cursor in command line
static int line_len;				// len of console line
static char buffer[BUFFER_SIZE];	// buffer for console line
static char mem[CHAR_MEM_SIZE];		// mini buffer for specials keys (ARROW,DEL,etc.)

// --- private functions ------------------------------------
// write buffer content
static void console_write_line()
{
	for(int i=0 ; i<line_len ; i++)
		am335x_console_putc(buffer[i]);
}

// erase line with del character then print prompt
static void console_erase_line()
{
	for(int i=line_len+10 ; i>0 ; i--)
		am335x_console_putc(DEL);

	for(int i=0 ; i<line_len+10 ; i++)
		am335x_console_putc(SPACE);

	for(int i=line_len+20 ; i>0 ; i--)
		am335x_console_putc(DEL);

	shell_print_prompt();
}

// set cursor position on the line command
static void console_set_cursor_pos(int pos)
{
	if(pos<0)pos=0;
	if(pos>line_len)pos=line_len;

	console_erase_line();
	console_write_line();

	for(int i=line_len ; i>pos ; i--)
	{
		am335x_console_putc(ESCAPE);
		am335x_console_putc(BRACKET);
		am335x_console_putc(ARROW_LEFT);
	}

	cursor_pos = pos;
}

// insert c from cursor poos in buffer then rewrite the line
static void console_insert(char c)
{
	for(int i=line_len+1 ; i>cursor_pos ; i--)
		buffer[i] = buffer[i-1];

	buffer[cursor_pos] = c;
	cursor_pos++;
	line_len++;

	console_erase_line();
	console_write_line();
	console_set_cursor_pos(cursor_pos);
}

// add one character at the end of command line
static void console_add(char c)
{
	if(cursor_pos == line_len) cursor_pos++;

	buffer[line_len]=c;
	line_len++;
	am335x_console_putc(c);
}

// return and print prompt
extern void console_new_line()
{
	am335x_console_putc(NEW_LINE);
	shell_print_prompt();
}

// push char c in FIFO mini-buffer
static void console_shift_mem(char c)
{
	for(int i=CHAR_MEM_SIZE-1 ; i>0 ; i--)
		mem[i] = mem[i-1];

	mem[0] = c;
}

// write in command line one cmd
static void console_copy_cmd(char* cmd)
{
	if(!cmd)return;

	console_erase_line();

	memset(&buffer,0,sizeof(char)*BUFFER_SIZE);
	memset(&mem,0,sizeof(char)*CHAR_MEM_SIZE);
	strcpy(buffer,cmd);
	line_len = strlen(cmd);
	cursor_pos = line_len;

	console_write_line();
}

// erase one character like backspace
static void console_back_space()
{
	for(int i=cursor_pos -1 ; i<line_len && i>=0 ; i++)
		buffer[i] = buffer[i+1];

	cursor_pos--;
	line_len--;
	console_erase_line();
	console_write_line();
	console_set_cursor_pos(cursor_pos);
}

// erase one character like del
static void console_del()
{
	for(int i=cursor_pos ; i<line_len ; i++)
		buffer[i] = buffer[i+1];

	line_len--;
	console_erase_line();
	console_write_line();
	console_set_cursor_pos(cursor_pos);
}

// --- public function ------------------------------
extern int console_init()
{
	memset(&buffer,0,sizeof(char)*BUFFER_SIZE);
	memset(&mem,0,sizeof(char)*CHAR_MEM_SIZE);
	line_len = 0;
	cursor_pos = 0;
	console_new_line();
	return 0;
}

// clear the entire screan
extern void console_clear()
{
	am335x_console_putc(CLEAR_SCREEN);
	memset(&buffer,0,sizeof(char)*BUFFER_SIZE);
	memset(&mem,0,sizeof(char)*CHAR_MEM_SIZE);
	line_len = 0;
	cursor_pos = 0;
}

// --- thread to read console input and process commands ---
void console_thread(void* param)
{
	(void)param;

	char c;

	while(1)
	{
		// test if a character is available (ensure thread are not blocked)
		while(am335x_console_tstc())
		{
			c = (char)am335x_console_getc();	// blocking !

			console_shift_mem(c);				// mini buffer for specials KEYS

			if(mem[3]==ESCAPE && mem[2]==BRACKET && mem[1]=='3' && c==TILDE)
			{
				console_del();
			}
			else if(mem[2]==ESCAPE && mem[1]==BRACKET && c==ARROW_UP)
			{
				console_copy_cmd(shell_get_previous_slc());
			}
			else if(mem[2]==ESCAPE && mem[1]==BRACKET && c==ARROW_DOWN)
			{
				console_copy_cmd(shell_get_next_slc());
			}
			else if(mem[2]==ESCAPE && mem[1]==BRACKET && c==ARROW_LEFT)
			{
				console_set_cursor_pos(cursor_pos-1);
			}
			else if(mem[2]==ESCAPE && mem[1]==BRACKET && c==ARROW_RIGHT)
			{
				console_set_cursor_pos(cursor_pos+1);
			}
			else if(mem[2]==ESCAPE && mem[1]=='O' && c=='F')
			{
				console_set_cursor_pos(line_len);
			}
			else if(mem[2]==ESCAPE && mem[1]==BRACKET)
			{
				// Nothing
			}
			else if(mem[1]==ESCAPE)
			{
				// Nothing
			}
			else
			{
				if(c==DEL)
				{
					console_back_space();
				}
				else if(c==CTRL_C)
				{
					console_init();
				}
				else if(c==C_RETURN)
				{
					strings_trim(buffer);
					strings_t line = strings_split(buffer,SPACE);

					am335x_console_putc(NEW_LINE);

					// if success then we meorize the command
					if(shell_execute(line.size, line.strings))
						shell_add_slc(buffer);

					console_init(); // clear buffer

					strings_free_strings_t(&line);
				}
				else if(c!=ESCAPE && c!=BRACKET)
				{
					if(cursor_pos < line_len)
					{
						console_insert(c);
					}
					else
					{
						console_add(c);
					}
				}
			}
		}

		// console is user mode
		system_yield();
	}
}
