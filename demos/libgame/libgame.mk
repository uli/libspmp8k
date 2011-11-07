#
# make rules for SPMP8000 bin games
#
#

TOOLCHAIN=arm-elf-

CC		= $(TOOLCHAIN)gcc
AS		= $(TOOLCHAIN)gcc
LD		= $(TOOLCHAIN)gcc
CPP		= $(TOOLCHAIN)cpp
OBJCOPY	= $(TOOLCHAIN)objcopy
MKBING	= $(TOOLS_DIR)/mkbing
BIN2C	= $(TOOLS_DIR)/bin2c
PNG2C	= $(TOOLS_DIR)/png2c
RM		= rm -f

LDSCRIPT= $(LIBGAME)/libgame.ld

ifdef NEWLIB
CFLAGS	= -O2 -c -DHAVE_NEWLIB -I$(LIBGAME) -I$(NEWLIB)/include -nostdlib -march=armv5 -msoft-float
LDFLAGS	= -nostdlib -L$(LIBGAME) -L$(NEWLIB)/lib -march=armv5 -msoft-float -nostartfiles -T$(LDSCRIPT)
#LIBS	= -lc -lgcc -lgame
else
CFLAGS	= -O2 -c -I$(LIBGAME) -nostdlib -march=armv5 -msoft-float
LDFLAGS	= -nostdlib -L$(LIBGAME) -march=armv5 -msoft-float -nostartfiles -T$(LDSCRIPT)
#LIBS	= -lgcc -lgame
endif

START_O	= $(LIBGAME)/start.o


all		: $(OBJS)
		$(LD) $(LDFLAGS) $(START_O) $(OBJS) $(LIBS) -o $(TARGET).1
		$(OBJCOPY) -S -O binary $(TARGET).1 $(TARGET).2
		$(MKBING) $(TARGET).2 $(TARGET).bin

%.o		: %.c
		$(CC) $(CFLAGS) $< -o $@

%.o		: %.s
		$(CPP) $< -o $<-tmp.s
		$(AS) $(ASFLAGS) $<-tmp.s -o $@
		$(RM) $<-tmp.s

%.c		: %.png
		$(PNG2C) $< $@

%.c		: %.mid
		$(BIN2C) $< $@

clean	:
		$(RM) $(OBJS)
		$(RM) $(TARGET).1
		$(RM) $(TARGET).2
		$(RM) $(TARGET).bin

