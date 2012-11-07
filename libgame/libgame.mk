#
# make rules for SPMP8000 bin games
#
#

include $(LIBSPMP8K)/main.cfg

CC		= $(TOOLCHAIN)gcc
AS		= $(TOOLCHAIN)as
LD		= $(TOOLCHAIN)gcc
CPP		= $(TOOLCHAIN)cpp
OBJCOPY	= $(TOOLCHAIN)objcopy
MKBING	= $(TOOLS_DIR)/mkbing
BIN2C	= $(TOOLS_DIR)/bin2c
PNG2C	= $(TOOLS_DIR)/png2c
RM		= rm -f

LDSCRIPT= $(LIBGAME)/libgame.ld

CFLAGS	+= -O2 -Wall -Wno-format -W -g -c -DHAVE_NEWLIB -I$(LIBGAME) -I$(NEWLIB)/include -I$(3RDPARTY)/include -nostdlib -mcpu=arm926ej-s -msoft-float
LDFLAGS	+= -nostdlib -L$(LIBGAME) -L$(NEWLIB)/lib -L$(3RDPARTY)/lib -march=armv5 -msoft-float -nostartfiles -T$(LDSCRIPT)
#LIBS	= -lgcc -lgame -lc

START_O	= $(LIBGAME)/start.o


all		: $(TARGET).bin

$(TARGET).bin: $(OBJS)
		$(LD) $(LDFLAGS) $(START_O) $(OBJS) $(LIBS) -o $(TARGET).1
		$(OBJCOPY) -S -O binary $(TARGET).1 $(TARGET).2
		$(MKBING) $(TARGET).2 $(TARGET).bin

%.o		: %.c
		$(CC) $(CFLAGS) $< -o $@
		@mkdir -p .deps/$(dir $*.d)
		@$(CC) -MM -MT $@ $(CFLAGS) $< > .deps/$*.d

%.o		: %.s
		$(CPP) $< -o $<-tmp.s
		$(AS) $(ASFLAGS) $<-tmp.s -o $@
		@mkdir -p .deps/$(dir $*.d)
		@$(CC) -MM -MT $@ $(ASFLAGS) $< > .deps/$*.d
		$(RM) $<-tmp.s

%.c		: %.png
		$(PNG2C) $< $@

%.c		: %.mid
		$(BIN2C) $< $@
		
%.c		: %.wav
		$(BIN2C) $< $@

clean	:
		$(RM) $(OBJS)
		$(RM) $(TARGET).1
		$(RM) $(TARGET).2
		$(RM) $(TARGET).bin
		$(RM) -r .deps

-include $(OBJS:%.o=.deps/%.d)
