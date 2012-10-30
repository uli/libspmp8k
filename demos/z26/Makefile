# z26 spmp8k port
# 2011 Triple Oxygen

TARGET	= z26
TOOLS_DIR = ../../bin

include ../libgame/libgame.cfg

CC		= $(TOOLCHAIN)gcc
LD		= $(TOOLCHAIN)gcc
OBJCOPY	= $(TOOLCHAIN)objcopy
MKBING	= $(TOOLS_DIR)/mkbing

LDSCRIPT= $(LIBGAME)/libgame.ld

CFLAGS	= -O2 -c -DHAVE_NEWLIB -I$(LIBGAME) -I$(NEWLIB)/include -nostdlib -march=armv5 -msoft-float
LDFLAGS	= -nostdlib -L$(LIBGAME) -L$(NEWLIB)/lib -march=armv5 -msoft-float -nostartfiles -T$(LDSCRIPT)
LIBS	= -lc -lgcc -lgame
START_O	= $(LIBGAME)/start.o

#OBJS	= z26.o globals.o \
#	2600core.o		c_cpujam.o	guigame.o	palette.o \
#	c_banks.o		guiinterface.o	pcx.o		c_soundq.o \
#	gui_sort.o		c_pitfall2.o	c_starpath.o	c_tialine.o \
#	carts.o			guisound.o	position.o	text.o \
#	c_catchuppixels.o	c_riot.o \
#	genclr.o		guitweaks.o	sdlflip.o	c_tiasnd.o \
#	c_tiawrite.o \
#	cli.o			guiabout.o	guivideo.o	\
#	controls.o		c_init.o	sdlpixcopy.o  \
#	c_cpu.o			guidebug.o	kidvid.o \
#	c_cpuhand.o		sdlsrv.o	sdlvideo.o

$(TARGET).bin: z26.o
	$(LD) $(LDFLAGS) $(START_O) z26.o $(LIBS) -o $(TARGET).1
	$(OBJCOPY) -S -O binary $(TARGET).1 $(TARGET).2
	$(MKBING) $(TARGET).2 $(TARGET).bin
	
z26.o:	z26.c \
	2600core.c		c_cpujam.c	palette.c \
	c_banks.c		c_soundq.c \
	c_pitfall2.c	c_starpath.c	c_tialine.c \
	carts.c			position.c	text.c \
	c_catchuppixels.c	c_trace.c	c_riot.c \
	genclr.c		guitweaks.c	c_tiasnd.c \
	globals.c		c_tiawrite.c \
	cli.c			\
	controls.c		c_init.c	sdlpixcopy.c  \
	c_cpu.c			kidvid.c	 \
	c_cpuhand.c		sdlsrv.c	sdlvideo.c
	$(CC) -Wall $(CFLAGS) -c z26.c

c_catchuppixels.c: catchuppixels.m4
	m4 catchuppixels.m4 > c_catchuppixels.c

c_cpu.c: cpu.m4
	m4 cpu.m4 > c_cpu.c

all: $(TARGET).bin

clean:
	rm -f core *.o z26 z26c z26-static z26.exe z26c.exe *~ *.bak *.1 *.2 *.bin
