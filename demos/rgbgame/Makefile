#
# game demo Makefile
#
#
#

include ../libgame/libgame.cfg

TARGET	= rgbgame

CC		= $(TOOLCHAIN)gcc
AS		= $(TOOLCHAIN)as
LD		= $(TOOLCHAIN)ld
CPP		= $(TOOLCHAIN)cpp
OBJCOPY	= $(TOOLCHAIN)objcopy
MKBING	= ../../tools/bingtools/mkbing
RM		= rm -f

CFLAGS	= -O2 -g -c
ASFLAGS	= -mcpu=arm9

LDSCRIPT= game.ld

OBJS	= start.o game.o

%.o		: %.c
		$(CC) $(CFLAGS) $< -o $@

%.o		: %.s
		$(CPP) $< -o $<-tmp.s
		$(AS) $(ASFLAGS) $<-tmp.s -o $@
		$(RM) $<-tmp.s

all		: $(OBJS)
		$(LD) -T$(LDSCRIPT) $(OBJS) -o $(TARGET).1
		$(OBJCOPY) -S -O binary $(TARGET).1 $(TARGET).2
		$(MKBING) $(TARGET).2 $(TARGET).bin

clean	:
		$(RM) $(OBJS)
		$(RM) $(TARGET).1
		$(RM) $(TARGET).2
		$(RM) $(TARGET).bin

