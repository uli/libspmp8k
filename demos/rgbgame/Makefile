#
# game demo Makefile
#
#
#

TARGET	= rgbgame
TOOLCHAIN=arm-elf-

CC		= $(TOOLCHAIN)gcc
AS		= $(TOOLCHAIN)as
LD		= $(TOOLCHAIN)ld
CPP		= $(TOOLCHAIN)cpp
OBJCOPY	= $(TOOLCHAIN)objcopy
MKBING	= ../../tools/bingtools/mkbing
RM		= rm -f

CFLAGS	= -O2 -c
ASFLAGS	= -mcpu=arm926ejs

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
		$(MKBING) $(TARGET).2 $(TARGET).BIN

clean	:
		$(RM) $(OBJS)
		$(RM) $(TARGET).1
		$(RM) $(TARGET).2
		$(RM) $(TARGET).BIN

