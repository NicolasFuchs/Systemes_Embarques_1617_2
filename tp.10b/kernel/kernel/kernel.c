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
 * Purpose:		Micro kernel service definition
 *
 * Author:		Daniel Gachet (first author)
 * 				Alan Sueur
 * 				Nicolas Fuchs
 *
 * Date: 		27.04.2017
 * Last update:	28.04.2017
 *
 */

#include "kernel.h"
#include "interrupt.h"
#include "intc.h"
#include "exception.h"
#include "mmu.h"
#include "syscall.h"
#include "system.h"
#include "vfs.h"
#include "console.h"
#include "am335x_console.h"
#include "strings.h"

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../shell/shell.h"

#define DIR_SEP			'/'
#define SPACE			' '
#define DEL				'\b'
#define CLEAR_SCREEN	'\f'
#define TAB				'\t'
#define RETURN			'\r'
#define NEW_LINE		'\n'

#define ARRAY_LEN(x) (sizeof(x)/sizeof(x[0]))


// to save current state of thread (registry, ls, cp, cpsr)
struct context{
	uint32_t regs[15];
	uint32_t cpsr;
};

// this function is implemented in ASM, ensure switch between two context (between two threads or two process)
extern void kernel_transfert(const struct region* regions,
			    unsigned number_of_regions,
			    struct context* former,
			    struct context* new);

// to avoid interrupt during sensible work
extern void kernel_interrupt_disable();

// to re-activate interrupt
extern void kernel_interrupt_enable();

// tcb = task control bloc (struct to represent one threadh or one process
struct tcb{
	struct context context;	// all registers datas
	kernel_thread_t start;  // running function

	enum states state;		// TERMINATED, READY, RUNNING, BLOCKED

	bool is_a_process;		// true if it's a process
	uint32_t physical_addr;	// process physical base address
	uint32_t page;			// 1MiB page number

	struct tcb* t_next;		// the next thread in the ring chain (maybe himself)
	struct tcb* t_previous; // the previous thread in the ring chain (maybe himself)

	struct tcb* s_next;		// for semaphore

	uint32_t id;			// thread id (like id process in linux)
	char name[32];			// thread name

	uint32_t stack_size;	// 0x4000 by default (minimum)
	uint32_t stack[];		// not in the sizeof of this struct
};

static int thread_id = 1;   // counter for all thread id

// the idle thread, is the MAIN thread, never TERMINATED
static struct tcb idle = {
		.state = RUNNING,
		.t_next = &idle,
		.t_previous = &idle,
		.id = 0,
		.name = "idle",
		.stack_size = 0,
};

struct args{
	int argc;
	char* argv[127];
	char args[];
};
static bool is_phy_addr_in_used[256];

// current thread using uP
static struct tcb* running_thread = &idle;

// FILO chain to keep threads for clean (TERMINATED thread)
static struct tcb* thread_to_trash = 0;

// function to encapsulate thread working function, just to be sure than kernel_thread_exit() are
// called after the thread function is terminated
static void kernel_startup(void* param, kernel_thread_t thread)
{
	thread(param);			// call thread working function
	kernel_thread_exit();	// this thread is TERMINATED
}

// just for test
void run_thread_1(void* param)
{
	(void)param;
	while(1){
		for(int i=0 ; i<100000 ; i++);

		//system_yield();
		kernel_thread_yield(); // ensure cooperative
	}
}

void kernel_init()
{
	interrupt_init();
	intc_init();
	exception_init();
	mmu_init();
	syscall_init();

	interrupt_enable();

	vfs_init();
	shell_init();
	console_init();

	// create console thread
	kernel_thread_create(console_thread, (void*)0,"console", 0);

	// just for test TODO: remove
	kernel_thread_create(run_thread_1, (void*)42,"thread1", 0);

	// launch all threads and loop
	kernel_launch();
}

// find a thread in ring list
struct tcb* kernel_get_task(char* name)
{
	struct tcb* candidate = running_thread;

	do
	{
		if(strcmp(candidate->name,name) == 0)
			return candidate;

		candidate = candidate->t_next;
	}
	while(candidate != running_thread);

	return 0;
}

// clean all thread in thread_to_trash list
void kernel_process_terminated_threadh()
{
	// clean all TERMINATED threads (in thread_to_trash FILO)
	struct tcb* candidate;
	while((candidate = thread_to_trash))
	{
		thread_to_trash = candidate->t_next;

		if(candidate->is_a_process)
			is_phy_addr_in_used[candidate->page] = false;

		free(candidate);
	}
}

// launch all thread
void kernel_launch()
{
	while(1){
		kernel_thread_yield();					// pass on all thread successively
		kernel_process_terminated_threadh();	// check if threads need to be clean
	}
}

// to maintain a semaphore
struct sema {
	int32_t counter;
	struct tcb* s_next;
	struct sema* chain;
};

// create a semaphore and return id
int kernel_sema_create(int initial_value)
{
	struct sema* sema = calloc(1, sizeof(*sema));
	if(sema == 0) return -1;

	sema->counter = initial_value;
	return (int)sema;
}

// clean a semaphore
int kernel_sema_destroy(int sema_id)
{
	struct sema* sema = (struct sema*)sema_id;
	if(sema->s_next != 0) return -1;

	free(sema);
	return 0;
}

// operation P on semaphore
int kernel_sema_wait(int sema_id)
{
	struct sema* sema = (struct sema*)sema_id;

	// no interrupt before semaphore treatment are done
	kernel_interrupt_disable();

	sema->counter--;

	if(sema->counter < 0) // no P operation pending, the process must wait
	{
		if (sema->s_next == 0) // the semaphore waiting queue is empty
		{
			// the queue for this semaphore
			sema->s_next = running_thread;
		}else // find the last thread in queue and attach this thread to him
		{
			struct tcb* next = sema->s_next;
			while(next->s_next != 0) next = next->s_next;
			next->s_next = running_thread;
		}

		running_thread->s_next = 0;      // this thread is the last in waiting queue, no s_next
		running_thread->state = BLOCKED; // we block this thread (waiting)
		kernel_thread_yield();			 // pass on next thread
	}

	kernel_interrupt_enable();

	return -1;
}

// operation V on semaphore
int kernel_sema_signal(int sema_id)
{
	struct sema* sema = (struct sema*)sema_id;

	// no interrupt before semaphore treatment are done
	kernel_interrupt_disable();

	sema->counter++;

	if(sema->s_next != 0) // one thread at least in waiting queue
	{
		sema->s_next->state = READY; 			// release the first in the list
		sema->s_next = sema->s_next->s_next;	// remove the released thread from the list
	}

	kernel_interrupt_enable();
	return 0;
}

// only for thread, not for process (perhaps later ....)
int kernel_fork(char* task_name, char* new_task_name)
{
	struct tcb* task = kernel_get_task(task_name);
	if(!task)return -1;

	struct tcb* candidate = kernel_get_task(new_task_name);
	if(candidate)return -1;

	return kernel_thread_create(task->start,
								(void*)(task->context.regs[0]),
								new_task_name,
								task->stack_size);
}

// create one thread
int kernel_thread_create(kernel_thread_t routine, void* param, const char* name, size_t stack_sz)
{
	int id = -1;

	// minimal stack size
	stack_sz = (stack_sz < 0x4000) ? 0x4000 : (stack_sz + 8); // 8 to keep enought for alignment
	struct tcb* tcb = malloc(sizeof(struct tcb) + stack_sz);

	if(tcb != 0){
		memset(tcb, 0, sizeof(*tcb));					// set memory to 0 for this struct tcb
		tcb->context.regs[0] = (uint32_t)param;
		tcb->context.regs[1] = (uint32_t)routine;		// function pointer

		tcb->context.regs[13] = (((uint32_t)&tcb->stack[stack_sz/4])/8)*8;  // stack pointer for this thread
		tcb->context.regs[14] = (uint32_t)kernel_startup; 					// encapsulate calling real thread
																			// function (routine -> reg[1])

		tcb->start = routine;

		tcb->context.cpsr = 0x13; // mode user

		tcb->state = READY;
		tcb->stack_size = stack_sz;

		id = thread_id++;
		tcb->id = id;
		strcpy(tcb->name, name); // with memset is ok for \0 (after memset all Bytes from name[] == 0)

		// insert this thread in chain
		tcb->t_next = idle.t_next;
		tcb->t_previous = &idle;
		idle.t_next->t_previous = tcb;
		idle.t_next = tcb;
	}

	return id;
}

void kernel_thread_yield()
{
	struct tcb* former_thread = running_thread;
	struct tcb* new_thread = former_thread->t_next;		// may be himself

	kernel_interrupt_enable();

	if(former_thread->state == RUNNING)
		former_thread->state = READY;

	while(new_thread->state != READY)
	{
		if(new_thread->state == TERMINATED)
		{
			// remove thread from chain only if not the idle
			if(new_thread != &idle)
			{
				former_thread->t_next = new_thread->t_next;
				new_thread->t_next->t_previous = former_thread;

				// add thread to trash queue
				new_thread->t_next = thread_to_trash;
				thread_to_trash = new_thread;

				// affect new_thread
				new_thread = former_thread;
			}
		}

		new_thread = new_thread->t_next;
	}

	running_thread = new_thread;
	running_thread->state = RUNNING;

	static struct region regions[] = {
 		{.region_name="process", .virtual_addr=0x20000000,
		 .physical_addr=0, .sz=1, .cfg=0, },
	};
	if (new_thread->is_a_process) {
		regions[0].physical_addr = new_thread->physical_addr;
		regions[0].cfg = NORMAL;
	} else {
		regions[0].physical_addr = 0;
		regions[0].cfg = 0;
	}

	kernel_transfert(regions,
					 ARRAY_LEN(regions),
					 &former_thread->context,
					 &new_thread->context);
}

void kernel_thread_exit()
{
	running_thread->state = TERMINATED;
	kernel_thread_yield();
}

int kernel_thread_id()
{
	return running_thread->id;
}

int kernel_exec(const char* filename, const char* name, int argc, char* argv[])
{
	int id = -1;

	// search for next free memory page
	unsigned page = 0;
	for(page=0; page<ARRAY_LEN(is_phy_addr_in_used); page++)
	{
		if(!is_phy_addr_in_used[page]) break;
	}

	if(page>ARRAY_LEN(is_phy_addr_in_used)) return -1;
	uint32_t phy_addr = 0x88000000 + 0x100000 * page;

	// load process code into memory
	FILE* file = fopen(filename, "r");
	char* code = (char*)phy_addr;

	if(file != 0)
	{
		while(1)
		{
			size_t sz = fread(code,1,4096,file);
			if(sz<=0) break;
			code += sz;
		}
	}
	else
	{
		printf("process file could not be open ...\n");
		return -1;
	}

	struct args* args = (struct args*)phy_addr;
	args->argc = argc;
	char* arg = &(args->args[0]);
	for(int i=0 ; i<argc ; i++)
	{
		args->argv[i] = 0x20000000 + (arg-phy_addr);
		strcpy(arg,argv[i]);
		arg += strlen(arg) +1;
	}

	// create tcb
	struct tcb* tcb = calloc(1,sizeof(struct tcb));

	if(tcb != 0){
		tcb->context.regs[13] = 0x20100000;
		tcb->context.regs[14] = 0x20001000;
		tcb->context.cpsr = 0x10; // mode user

		tcb->state = READY;
		tcb->stack_size = -1;

		is_phy_addr_in_used[page] = true;
		tcb->is_a_process = true;
		tcb->physical_addr = phy_addr;
		tcb->page = page;

		id = thread_id++;
		tcb->id = id;
		strcpy(tcb->name, name); // with calloc is ok for \0 (after memset all Bytes from name[] == 0)

		// insert this thread in chain
		tcb->t_next = idle.t_next;
		tcb->t_previous = &idle;
		idle.t_next->t_previous = tcb;
		idle.t_next = tcb;
	}

	return id;
}

const char* kernel_thread_name()
{
	return running_thread->name;
}

char* kernel_get_state(enum states s)
{
	switch(s)
	{
	case TERMINATED:
		return "TERMINATED";
	case READY:
		return "READY";
	case RUNNING:
		return "RUNNING";
	case BLOCKED:
		return "BLOCKED";
	}

	return "UNKNOW";
}

void kernel_ps()
{
	struct tcb* th = &idle;

	printf("PID\tNAME\tSTATE\n");
	printf("-----------------------\n");

	do
	{
		printf("%d\t\%s\t\%s\n",(int)(th->id),th->name,kernel_get_state(th->state));
		th = th->t_next;
	}while(th != &idle);

}

int kernel_kill(int pid)
{
	struct tcb* candidate = running_thread;

	do
	{
		if(candidate->id == (uint32_t)pid)
		{
			candidate->state = TERMINATED;
			kernel_thread_yield();
			return -1;
		}
		candidate = candidate->t_next;
	}
	while(candidate != running_thread);

	return 0;
}
