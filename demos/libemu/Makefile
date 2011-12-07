#
# libemu Makefile
#

include ../libgame/libgame.cfg

TARGET	= libemu.a

CC	= $(TOOLCHAIN)gcc
AS	= $(TOOLCHAIN)as
AR	= $(TOOLCHAIN)ar
CPP	= $(TOOLCHAIN)cpp
RM	= rm -f

START_O	= start.o

CFLAGS	= -O2 -c -I$(NEWLIB)/include -nostdlib -march=armv5 -msoft-float
OBJS	= libemu.o
ASFLAGS	= -mcpu=arm9
ARFLAGS	= cr

all: $(OBJS)
	$(AR) $(ARFLAGS) $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

%.o: %.s
	$(CPP) $< $<.tmp
	$(AS) $(ASFLAGS) $<.tmp -o $@
	$(RM) $<.tmp

clean:
	$(RM) $(OBJS)
	$(RM) $(TARGET)
