#include "cons.h"
#include "am335x_console.h"
#include <string.h>
#include "vfs.h"

static int c_open (const char *name, int flags, int mode) {
	(void)name; (void)flags; (void)mode;
	
  	return 1;  
} 

static int c_read(int file, char *ptr, int len) {
	(void)file; (void)len;
	*ptr = am335x_console_getc();
	return 1;
}

static int c_write(int file, char *ptr, int len) {
	(void)file; 
	for (int todo = 0; todo < len; todo++) {
		am335x_console_putc(*ptr++);
	}
	return len;
}

static int c_lseek(int file, int ptr, int dir) {
	(void)file; (void)ptr; (void)dir; 
	return 0;
}

static int c_isatty(int file) {
	(void)file; 
	return 1;
}

static int c_fstat(int file, struct stat *st) {
	(void)file;
	memset(st, 0, sizeof(*st));
	st->st_mode = S_IFCHR;
	return 0;
}

static int c_close(int file) {
	(void)file; 
	return 0;
}

static struct file_ops f_ops = {
	.open  = c_open,
	.read  = c_read,
	.write = c_write,
	.lseek = c_lseek,
	.isatty= c_isatty,
	.fstat = c_fstat,
	.close = c_close,

};

struct file_ops* cons_init()
{
	return &f_ops;
}
