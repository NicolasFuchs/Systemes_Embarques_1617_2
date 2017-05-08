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
 * Project:	HEIA-FR / Embedded Systems 2 
 *
 * Abstract: 	Introduction to Operating System
 *
 * Purpose:	Micro kernel service definition
 *
 * Autĥor:	Daniel Gachet / HEIA-FR
 * Date:	31.03.2017
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "kernel.h"

#ifdef DEBUG
#define pr_debug(x...) printf(x)
#else
#define pr_debug(x...)
#endif

// definition of the possible states of a thread
enum states {TERMINATED, RUNNING, READY, BLOCKED};

// definition of the thread control block (tcb)
struct tcb {
	struct {
		uint32_t regs[15];
		uint32_t psr;
	} context;
	enum states state;

	struct tcb* t_next;	// used to chain all tcb
	struct tcb* s_next;	// used to chain blocked threads within a sema

	int32_t id;		// unique thread identification
	char name[32];		// thread name

	uint32_t stack_size;	// thread stack size in bytes
	uint32_t stack[];	// stack
};

// kernel idle thread
static struct tcb idle_thread = {
	.state = RUNNING,
	.t_next = &idle_thread,
	.id = 0,
	.name = "idle",
	.stack_size = 0,
};

static int32_t thread_id = 1;
static struct tcb* running_thread  = &idle_thread;
//static struct tcb* list_of_threads = &idle_thread;


/* forward declaration of low level transfer routines...
 * those methods have be implemented in assembler...
 */
extern void kernel_transfer(struct tcb* former, struct tcb* new);
extern void kernel_interrupt_enable();
extern void kernel_interrupt_disable();


// ----------------------------------------------------------------------------
// --- task service implementation --------------------------------------------
// ----------------------------------------------------------------------------

// --- local services ---

// this method shall be called only from the idle thread
static void kernel_process_terminated_threads(struct tcb* idle)
{
	struct tcb* prev = idle;
	while(1) {
		struct tcb* actual = prev->t_next;
		if (actual == idle) break;

		if (actual->state == TERMINATED) {
			prev->t_next = actual->t_next;
			pr_debug("kernel --> free: %ld - %s\n", 
				running_thread->id, running_thread->name);
			free(actual);
		} else {
			prev = prev->t_next;
		}
	}
}

static void kernel_startup (void* param, kernel_thread_t thread)
{
	thread (param);
	kernel_thread_exit();
}
 
// --- public services ---

void kernel_thread_exit()
{
	running_thread->state = TERMINATED;
	pr_debug("kernel --> exit: %ld - %s\n", 
		running_thread->id, running_thread->name);
	kernel_thread_yield();
}

void kernel_thread_yield()
{
	struct tcb* former_thread = running_thread;
	struct tcb* new_thread = former_thread->t_next;

	kernel_interrupt_enable();
	if (former_thread->state == RUNNING) former_thread->state = READY;
	while (new_thread->state != READY) new_thread = new_thread->t_next;

	running_thread = new_thread;
	new_thread->state = RUNNING;
	kernel_transfer(former_thread, new_thread);
}

void kernel_launch()
{
	while(1) {
		kernel_thread_yield();
		kernel_process_terminated_threads(running_thread);
	}
}

int kernel_thread_create(
	kernel_thread_t thread, 
	void* param, 
	const char* name, 
	uint32_t stack_size)
{
	int id = -1;

	stack_size = (stack_size < 0x4000) ? 0x4000 : (stack_size + 8);
	struct tcb* tcb = malloc (sizeof(struct tcb) + stack_size);

	if (tcb != 0) {
		memset (tcb, 0, sizeof(struct tcb));
		tcb->context.regs[0] = (uint32_t)param;
		tcb->context.regs[1] = (uint32_t)thread;
		tcb->context.regs[13] = (((uint32_t)&tcb->stack[stack_size/4])/8)*8;
		tcb->context.regs[14] = (uint32_t)kernel_startup;
		tcb->context.psr = 0x13; // mode: supervior; IRQ & FIQ enabled 
		tcb->state = READY;

		tcb->stack_size = stack_size;

		id = thread_id++;
		tcb->id = id;
		strncpy (tcb->name, name, sizeof(tcb->name)-1);

		tcb->t_next = idle_thread.t_next;
		idle_thread.t_next = tcb;
	}

	return id;
}

const char* kernel_thread_name()
{
	return running_thread->name;
}

int kernel_thread_id()
{
	return running_thread->id;
}


// ----------------------------------------------------------------------------
// --- semaphore service implementation ---------------------------------------
// ----------------------------------------------------------------------------


struct sema {
	int32_t count;
	struct tcb* s_next;
	struct sema* chain;
};

struct sema* sema_list = 0;

// --- public services ---

int kernel_sema_create (int initial_value)
{
	struct sema * sema = calloc (1, sizeof(*sema));

	if (sema == 0) return -1;

	sema->count = initial_value;
	sema->s_next = 0;

	// add sema to list
	sema->chain = sema_list;
	sema_list = sema;

	return (int)sema;
}

int kernel_sema_destroy (int sema_id)
{
	struct sema* sema = (struct sema*)sema_id;

	if (sema->s_next != 0) return -1;

	// remove sema from list
	struct sema** indirect = &sema_list;
	while ((*indirect) != sema)
		indirect = &(*indirect)->chain; 
	*indirect = sema->chain;

	free (sema);

	return 0;
}


int kernel_sema_wait (int sema_id)
{
	struct sema* sema = (struct sema*)sema_id;

	kernel_interrupt_disable();
	sema->count--;
	if (sema->count < 0) {
		if (sema->s_next == 0) {
			sema->s_next = running_thread;
		} else {
			struct tcb* next = sema->s_next;
			while (next->s_next != 0) next = next->s_next;
			next->s_next = running_thread;
		}
		running_thread->s_next = 0;
		running_thread->state = BLOCKED;
		kernel_thread_yield();
	}
	kernel_interrupt_enable();
	return 0;
}

int kernel_sema_signal (int sema_id)
{
	struct sema* sema = (struct sema*)sema_id;
	
	kernel_interrupt_disable();
	sema->count++;
	if (sema->s_next != 0) {
		sema->s_next->state = READY;
		sema->s_next = sema->s_next->s_next;
	}
	kernel_interrupt_enable();
	return 0;
}



// ----------------------------------------------------------------------------
// --- micro kernel initialization --------------------------------------------
// ----------------------------------------------------------------------------

void kernel_init() { }
