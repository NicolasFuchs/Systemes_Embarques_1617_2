EXEC=app
SRCS=$(wildcard *.c) 
ASRC=$(wildcard *.S)

LIBDIRS=interrupt drivers ff12b vfs_gac kernel

CFLAGS+=-DINTC_EN=1

# Include the standard makefile for the embedded systems 1 & 2 labs 
#PROCESS=true
include $(LMIBASE)/bbb/make/bbb.mk

EXTRAS=a.bin

bin: all
	arm-none-eabi-objcopy -O binary $(EXE) a.bin

