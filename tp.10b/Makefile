EXEC=app
SRCS=$(wildcard *.c) 
ASRC=$(wildcard *.S)

LIBDIRS=interrupt drivers ff12b

CFLAGS+=-DINTC_EN=1

# Include the standard makefile for the embedded systems 1 & 2 labs 
include $(LMIBASE)/bbb/make/bbb.mk

