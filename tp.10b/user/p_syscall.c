/**
 * Copyright 2015 University of Applied Sciences Western Switzerland / Fribourg
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
 * Project: HEIA-FR / Embedded Systems 1+2 Laboratory
 *
 * Abstract:    newlib system calls
 *
 * Purpose: This module implements all the required newlib system calls
 *
 * Author:  Daniel Gachet
 * Date:    08.07.2015
 */


/* GNU ARM Toolchain & newlib: system calls */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "am335x_clock.h"
#include "am335x_console.h"

#include "../syscall.h"
#include "vfs.h"
#include "cons.h"
#include "sd.h"

... ARRAY_LEN(x) ((int) (sizeof(x)/sizeof(x[0])))

static struct file_ops* fs[2];
static struct file {
    struct file_ops* op;
    int id;
} f[1024];

int _open(const char *name, int flags, int mode)
{
    int i=0;
    while ((f[i].op!=0) && (i<ARRAY_SIZE(f))) i++;
    if (i >= ARRAY_LEN(f) return -1);

    int id = fops->open (fname, flags, mode);
    if (id == -1) return -1;

    f[i].id = id;
    f[i].op = fops;

    return 1;
}
//...

int _read(int file __attribute__((unused)), char *ptr, int len __attribute__((unused))) {
    return f[i].op->read (f[i].id, ptr, len);
}

int _write(int file __attribute__((unused)), char *ptr, int len) {
    return f[i].op->write(f[i].id, ptr, len);
}

int _lseek(int file __attribute__((unused)), int ptr __attribute__((unused)), int dir __attribute__((unused))) {
    return f[i].op->lseek(f[i].id, ptr, len);
}

int _isatty(int file __attribute__((unused))) {
    return 1;
}

#include <sys/stat.h>
int _fstat(int file __attribute__((unused)), struct stat *st) {
    st->st_mode = S_IFCHR;
    return 0;
}

int _close(int file __attribute__((unused))) {
    return -1;
}

#if 0
void pint (int v)
{
    if (v < 0) {
        am335x_console_putc ('-');
        v = -v;
    }
    if (v > 0) pint (v/10);
    am335x_console_putc (v%10+'0');
}

void padr (void* a)
{
    unsigned long v = (unsigned long)a;
    if (v > 0) padr ((void*)(v/16));
    v %= 16;
    am335x_console_putc (v>9 ? v-10+'a' : v+'0');
}
#endif

void* _sbrk(int incr) {
    extern char _end; /* Defined by the linker */
    extern char _stack_bottom;
    static char *heap_end;
    char *prev_heap_end;

    if (heap_end == 0) {
      heap_end = &_end;
    }
    prev_heap_end = heap_end;
    if (heap_end + incr > &_stack_bottom) {
        //write (1, "Heap and stack collision\n", 25);
        //abort ();
        return 0;
    } else {
        heap_end += incr;
    }
#if 0
    am335x_console_puts("_sbrk= incr="); pint(incr);
    am335x_console_puts(", _end=");      padr (&_end);
    am335x_console_puts(", heap_end=");  padr (heap_end);
    am335x_console_puts(", ret=");       padr (prev_heap_end);
    am335x_console_puts("\n");
#endif
    return (void*) prev_heap_end;
}

void _exit(int i __attribute__((unused)))
{
    while(1);
}

void _init()
{
    am335x_clock_enable_l3_l4wkup();
    am335x_console_init();
    struct file_ops* fops = cons_init();
    f[0].op = cons_init();
    f[1].op =
}

void _fini()
{
}

int _getpid ()
{
    return 1;
}

int _kill (int pid __attribute__((unused)), int sig __attribute__((unused)))
{
    while(1);
}

/*
void abort(void)
 {
     while (1);
 }
*/

