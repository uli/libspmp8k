#
# libgame Makefile
#
#   libgame incorporates the SPMP8k game api as well as some glue
#	for newlib usage and a growing set of tiny goodies :-)
#
#	in order to use newlib configure it (newlib) with:
#		configure --disable-newlib-supplied-syscalls --target=arm-elf
#	build it and use it in conjunction with this libgame.
#

include libgame.cfg

TARGET	= libgame.a

CC		= $(TOOLCHAIN)gcc
AS		= $(TOOLCHAIN)as
AR		= $(TOOLCHAIN)ar
CPP		= $(TOOLCHAIN)cpp
RM		= rm -f

START_O	= start.o

ifdef NEWLIB
CFLAGS	= -O2 -Wall -W -c -I$(NEWLIB)/include -nostdlib -mcpu=arm926ej-s -msoft-float
OBJS	= start.o libgame.o glue.o dmsg.o font6x8.o
#OBJS	= libgame.o glue.o dmsg.o font6x8.o
else
CFLAGS	= -O2 -c -nostdlib -march=armv5 -msoft-float
#OBJS	= start.o libgame.o
OBJS	= libgame.o
endif
ASFLAGS	= -mcpu=arm9
ARFLAGS	= cr

#$(START_O)

all		: $(TARGET)
$(TARGET): $(OBJS)
		$(AR) $(ARFLAGS) $(TARGET) $(OBJS)

%.o		: %.c
		$(CC) $(CFLAGS) $< -o $@

%.o		: %.s
		$(CPP) $< $<.tmp
		$(AS) $(ASFLAGS) $<.tmp -o $@
		$(RM) $<.tmp

clean	:
		$(RM) $(OBJS)
		$(RM) $(TARGET)

