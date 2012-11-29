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

include ../main.cfg

TARGET	= libgame.a

CC		= $(TOOLCHAIN)gcc
AS		= $(TOOLCHAIN)as
AR		= $(TOOLCHAIN)ar
CPP		= $(TOOLCHAIN)cpp
RM		= rm -f

START_O	= start.o

CFLAGS	= -O2 -Wall -W -c -I$(NEWLIB)/include -nostdlib -mcpu=arm926ej-s -msoft-float
OBJS	= start.o libgame.o glue.o dmsg.o font6x8.o text.o adbg.o

ASFLAGS	= -mcpu=arm9

ARFLAGS	= cr

all		: $(TARGET)
$(TARGET): $(OBJS)
		$(AR) $(ARFLAGS) $(TARGET) $(OBJS)

%.o		: %.c
		$(CC) $(CFLAGS) $< -o $@
		@mkdir -p .deps/$(dir $*.d)
		@$(CC) -MM -MT $@ $(CFLAGS) $< > .deps/$*.d

%.o		: %.s
		$(CPP) $< $<.tmp
		$(AS) $(ASFLAGS) $<.tmp -o $@
		@mkdir -p .deps/$(dir $*.d)
		@$(CC) -MM -MT $@ $(ASFLAGS) $< > .deps/$*.d
		$(RM) $<.tmp

text.o: hzktable.c
hzktable.c: chinese/BG2UBG.KU chinese/big5.py
	python chinese/big5.py chinese/BG2UBG.KU >hzktable.c

clean	:
		$(RM) $(OBJS)
		$(RM) $(TARGET)
		$(RM) -r .deps
		$(RM) hzktable.c

-include $(OBJS:%.o=.deps/%.d)
