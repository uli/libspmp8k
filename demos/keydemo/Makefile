
#ifdef $(NEWLIB)
#undef $(NEWLIB)
#endif

TARGET	= keydemo

TOOLS_DIR = ../../bin

include ../libgame/libgame.cfg

# resources
RESOBJS	= font.o

ifdef NEWLIB
OBJS	= $(RESOBJS) game.o
else
OBJS	= $(RESOBJS) game.o printf.o
endif

LIBS	= -lc -lgcc -lgame

include ../libgame/libgame.mk
