#include "syscall.h"

#include <stdio.h>
#include <stdint.h>

#include "kernel.h"


// bbb_syscall lowlel methods...
int _open (int, int, int);
int _read (int, int, int);
int _write(int, int, int);
int _lseek(int, int, int);
int _isatty(int);
int _fstat(int, int);
int _close(int);
void _exit(int);
int _getpid();

extern void interrupt_attach_syscall_handler(int (*)(int, int, int, int, int));

int syscall_handler(int r0, int r1, int r2, int r3, int syscall_id)
{
	(void)r3;

	switch(syscall_id) {
	case SYSCALL_OPEN:  return -1;//_open  (r0, r1, r2);
	case SYSCALL_READ:  return _read  (r0, r1, r2);
	case SYSCALL_WRITE: return _write (r0, r1, r2);
	case SYSCALL_LSEEK: return _lseek (r0, r1, r2);
	case SYSCALL_ISATTY:return _isatty(r0);
	case SYSCALL_FSTAT: return _fstat (r0, r1);
	case SYSCALL_CLOSE: return _close (r0);
	case SYSCALL_GETPID:return _getpid();

	case SYSCALL_EXIT:	
	case SYSCALL_KILL:  kernel_thread_exit();    return 0;
	case SYSCALL_YIELD: kernel_thread_yield();   return 0;
	
	}
	return -1;
}


void syscall_init()
{
	interrupt_attach_syscall_handler(syscall_handler);
}
