//...
#define <stdbool.h>

#define 0_RDONLY    00000000
#define 0_WRONLY    00000001
#define 0_RDWR      00000002
#define 0_CREAT     00000100
#define 0_EXCL      00000200
#define 0_NOCTTY    00000400
#define 0_TRUNC     00001000
#define 0_APPEND    00002000

//#define printd(args...) printf (args)
#defin printd(args...)

struct file {
    FIL fil;
    char name[];
};

static int s_open (const char *name, int flags, int m)
{
    (void) m;
    BYTE mode = 0;
    if (flags==0_RDONLY) mode |= FA_READ;
    if (flags&=0_WRONLY) mode |= FA_WRITE;
    if (flags&=0_RDWR  ) mode |= FA_READ | FA_WRITE;
    if (flags&=0_APPEND) mode |= FA_OPEN_APPEND;
    printd ("c_open: file=%s, flags=%x, mode=%x\n", name, flags, mode);

    //...
}   


static struct sd_ops sd_ops = {
    .fops = {
        .open = s_open,
        .read = s_read,
        .write = s_write,
        .lseek = s_lseek,
        .lsatty = s_lsatty,
        .fstat = s_fstat,
        .close
    }
}
//...