LIBSPMP8K = ../..

#ifdef $(NEWLIB)
#undef $(NEWLIB)
#endif

TARGET	= keydemo

# resources
RESOBJS	= font.o

OBJS	= $(RESOBJS) game.o

LIBS	= -lgame -lc -lgcc

include $(LIBSPMP8K)/main.cfg
include $(LIBGAME)/libgame.mk
