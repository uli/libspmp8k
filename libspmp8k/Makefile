#
# libspmp8k Makefile
#
#
#

TOOLCHAIN_PATH=/home/tripleoxygen/re/cybergame/code/toolchain/install/bin
TOOLCHAIN=$(TOOLCHAIN_PATH)/arm-elf-

TARGET	= libspmp8k.a

CC		= $(TOOLCHAIN)gcc
AR		= $(TOOLCHAIN)ar
RM		= rm -f

OBJS	= uart.o

ARFLAGS	= cr
CFLAGS	= -c -O0

lib		: $(OBJS)
		$(AR) $(ARFLAGS) $(TARGET) $(OBJS) 

%.o		: %.c
		$(CC) $(CFLAGS) $< -o $@

clean	:
		$(RM) $(OBJS)
		$(RM) $(TARGET)

