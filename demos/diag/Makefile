
#ifdef $(NEWLIB)
#undef $(NEWLIB)
#endif

TARGET	= diag

TOOLS_DIR = ../../bin

include ../libgame/libgame.cfg

# resources
RESOBJS	= font.o

ifdef NEWLIB
OBJS	= $(RESOBJS) game.o
else
OBJS	= $(RESOBJS) game.o printf.o
endif

LIBS	= -lgame -lc -lgcc

include ../libgame/libgame.mk
