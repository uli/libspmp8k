#
# libspmp8k Makefile
#
#
#

TARGET	= libspmp8k.a
TOOLCHAIN=arm-elf-

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

