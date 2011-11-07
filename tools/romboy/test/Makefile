#
# Makefile
#
#

# enter the "bin" path to your toolchain here
TC_DIR	= 
# enter the prefix of your toolchain here
TOOLCHAIN=$(TC_DIR)arm-elf-

TOOLS	= ../../../bin

TARGET	= test

ELF2MMP = $(TOOLS)/elf2mmp

CC		=$(TOOLCHAIN)gcc
CPP		=$(TOOLCHAIN)cpp
LD		=$(TOOLCHAIN)ld
AS		=$(TOOLCHAIN)as
OBJCOPY	=$(TOOLCHAIN)objcopy
OBJDUMP	=$(TOOLCHAIN)objdump
RM		=rm -f
CP		= cp

LDSCRIPT= spmp8k.ld

CFLAGS	= -O0 -nostdlib -c
LDFLAGS	= -T$(LDSCRIPT) -nostdlib

#LIBDIR	= -L/usr/local/lib/gcc/arm-elf/4.3.3/


OBJS	= start.o main.o


all   : $(TARGET).mmp
		$(CP) $(TARGET).mmp ../$(TARGET).mmp

%.elf : $(OBJS)
		$(CC) $(LDFLAGS) $(LIBDIR) $(OBJS) $(LIBS) -o $(TARGET).elf

%.o : %.c
		$(CC) $(CFLAGS) $< -o $@
		
%.o : %.s
		$(CPP) $< -o $<.tmp
		$(AS) $<.tmp -o $@
		$(RM) $<.tmp

%.bin : %.elf
		$(OBJCOPY) -S -O binary $< $@
		
%.mmp : %.elf
		$(ELF2MMP) test.elf test.mmp
		
%.asm : %.elf
		$(OBJDUMP) -D -m arm $(TARGET).elf

clean	:
		$(RM) $(OBJS)
		$(RM) $(TARGET).mmp
		$(RM) $(TARGET).elf
		$(RM) $(TARGET).bin
		$(RM) ../$(TARGET).mmp

