
#ifdef $(NEWLIB)
#undef $(NEWLIB)
#endif

TARGET	= diag

# resources
RESOBJS	= font.o

ifdef NEWLIB
OBJS	= $(RESOBJS) game.o
else
OBJS	= $(RESOBJS) game.o printf.o
endif

LIBS	= -lgame -lc -lgcc

include ../../libgame/libgame.mk
