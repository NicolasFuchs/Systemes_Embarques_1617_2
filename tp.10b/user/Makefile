EXEC=app
SRCS=$(wildcard *.c) 
ASRC=$(wildcard *.S)

# Include the standard makefile for the embedded systems 1 & 2 labs 
PROCESS=true
include $(LMIBASE)/bbb/make/bbb.mk

EXTRAS=a.bin

bin: all
	arm-none-eabi-objcopy -O binary $(EXE) a.bin

