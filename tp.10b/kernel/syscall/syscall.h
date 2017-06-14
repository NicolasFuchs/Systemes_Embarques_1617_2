#pragma once
#ifndef SYSCALL_H
#define SYSCALL_H

#define SYSCALL_ID	44

#define SYSCALL_OPEN	0
#define SYSCALL_READ	1
#define SYSCALL_WRITE	2
#define SYSCALL_LSEEK	3
#define SYSCALL_ISATTY	4
#define SYSCALL_FSTAT	5
#define SYSCALL_CLOSE	6
#define SYSCALL_GETPID	7

#define SYSCALL_EXIT	100
#define SYSCALL_KILL	101
#define SYSCALL_YIELD	102



#ifndef __ASSEMBLER__

extern void syscall_init();
extern int syscall(int r0, int r1, int r2, int r3, int oper);

#endif
#endif
