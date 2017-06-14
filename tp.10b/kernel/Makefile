EXEC=app
SRCS=$(wildcard *.c) 
ASRC=$(wildcard *.S)

LIBDIRS=interrupt drivers utils file_system shell devices mmu kernel syscall system ff12b

CFLAGS+=-DINTC_EN=1

# Include the standard makefile for the embedded systems 1 & 2 labs 
include $(LMIBASE)/bbb/make/bbb.mk
