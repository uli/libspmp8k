#
# Makefile
#
#

CC		= gcc
CXX		= g++
CXXFLAGS= -O2 -c
CFLAGS	= -O2

MKBOBJS	= Des.o mkbing.o
UNBOBJS = Des.o unbing.o

all		: mkbing unbing bin2c png2c
		cp mkbing unbing bin2c png2c ../../bin

%.cpp	: %.o
		$(CXX) $(CXXFLAGS) $< -o $@

mkbing	: $(MKBOBJS)
		$(CXX) $(MKBOBJS) -o mkbing

unbing	: $(UNBOBJS)
		$(CXX) $(UNBOBJS) -o unbing

bin2c	: bin2c.c
		$(CC) $(CFLAGS) bin2c.c -o bin2c

# requires libpng
png2c	: png2c.c
		$(CC) $(CFLAGS) -lpng png2c.c -o png2c

clean	:
		rm -f $(OBJS)
		rm -f mkbing
		rm -f unbing
		rm -f bin2c
		rm -f png2c
