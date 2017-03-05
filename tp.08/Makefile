EXEC=app
SRCS=$(wildcard *.c) 
ASRC=$(wildcard *.S)

ifneq ($(DEBUG),)
CFLAGS+=-DDEBUG
endif
ifneq ($(FAST_HANDLER),)
CFLAGS+=-DFAST_HANDLER
endif
# Include the standard makefile for the embedded systems 1 & 2 labs 
include $(LMIBASE)/bbb/make/bbb.mk

