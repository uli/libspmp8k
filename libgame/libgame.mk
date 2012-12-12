#
# make rules for SPMP8000 bin games
#
#

include $(LIBSPMP8K)/main.cfg

CC		= $(TOOLCHAIN)gcc
CXX		= $(TOOLCHAIN)g++
AS		= $(TOOLCHAIN)as
LD		= $(TOOLCHAIN)gcc
CPP		= $(TOOLCHAIN)cpp
OBJCOPY	= $(TOOLCHAIN)objcopy
MKBING	= $(TOOLS_DIR)/mkbing
BIN2C	= $(TOOLS_DIR)/bin2c
PNG2C	= $(TOOLS_DIR)/png2c
RM		= rm -f

LDSCRIPT= $(LIBGAME)/libgame.ld

CFLAGS	+= -DHAVE_NEWLIB -I$(LIBGAME) -I$(NEWLIB)/include -I$(3RDPARTY)/include -mcpu=arm926ej-s -msoft-float
CXXFLAGS = $(CFLAGS)
LDFLAGS	+= -L$(LIBGAME) -L$(NEWLIB)/lib -L$(3RDPARTY)/lib -march=armv5 -msoft-float -nostartfiles -T$(LDSCRIPT)
#LIBS	= -lgcc -lgame -lc

all		: $(TARGET).bin

$(TARGET).bin: $(OBJS)
		$(LD) $(LDFLAGS) $(OBJS) $(LIBS) -o $(TARGET).1
		$(OBJCOPY) -S -O binary $(TARGET).1 $(TARGET).2
		$(MKBING) $(TARGET).2 $(TARGET).bin

%.o		: %.c
		$(CC) $(CFLAGS) -c $< -o $@
		@mkdir -p .deps/$(dir $*.d)
		@$(CC) -MM -MT $@ $(CFLAGS) -c $< > .deps/$*.d

%.o		: %.cpp
		$(CXX) $(CXXFLAGS) -c $< -o $@
		@mkdir -p .deps/$(dir $*.d)
		@$(CXX) -MM -MT $@ $(CXXFLAGS) -c $< > .deps/$*.d

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
		$(RM) $(EXTRA_CLEAN)

-include $(OBJS:%.o=.deps/%.d)
