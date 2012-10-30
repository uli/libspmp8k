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

OBJS	= game.o

LIBS = -lgame -lc

include ../libgame/libgame.mk
