#
# Makefile
#
#

TOOLCHAIN= arm-elf-
TARGET	= test

CC		=$(TOOLCHAIN)gcc
LD		=$(TOOLCHAIN)ld
AS		=$(TOOLCHAIN)as
OBJCOPY	=$(TOOLCHAIN)objcopy
RM		=rm -f

LDSCRIPT= spmp8k.ld

CFLAGS	= -O0 -nostdlib -c -I../../libspmp8k
LDFLAGS	= -T$(LDSCRIPT) -nostdlib

LIBDIR	= -L/usr/local/lib/gcc/arm-elf/4.3.3/ -L../../libspmp8k
LIBS	= -lgcc -lspmp8k

# start.o comes first
OBJS	= start.o main.o printf.o

all : $(OBJS)
		$(LD) $(LDFLAGS) $(LIBDIR) $(OBJS) $(LIBS) -o $(TARGET).elf

%.o : %.c
		$(CC) $(CFLAGS) $< -o $@

%.bin : %.elf
		$(OBJCOPY) -S -O binary $< $@

clean	:
		$(RM) $(OBJS)
		$(RM) $(TARGET).elf
		$(RM) $(TARGET).bin

