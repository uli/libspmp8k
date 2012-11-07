#
# game demo Makefile
#
#
#

LIBSPMP8K = ../..
include $(LIBSPMP8K)/main.cfg

TARGET	= rgbgame

CC		= $(TOOLCHAIN)gcc
AS		= $(TOOLCHAIN)as
LD		= $(TOOLCHAIN)gcc
CPP		= $(TOOLCHAIN)cpp
OBJCOPY	= $(TOOLCHAIN)objcopy
MKBING	= ../../tools/bingtools/mkbing
RM		= rm -f

CFLAGS	= -O2 -g -c
ASFLAGS	= -mcpu=arm9

OBJS	= game.o

LIBS = -lgame -lc

include $(LIBGAME)/libgame.mk
