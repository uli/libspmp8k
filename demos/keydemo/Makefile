
ifdef $(NEWLIB)
undef $(NEWLIB)
endif

TARGET	= keydemo

TOOLS_DIR = ../../bin
LIBGAME	= ../libgame

# resources
RESOBJS	= font.o

ifdef $(NEWLIB)
OBJS	= $(RESOBJS) game.o
else
OBJS	= $(RESOBJS) game.o printf.o
endif

LIBS	= -lc -lgcc -lgame

include ../libgame/libgame.mk
