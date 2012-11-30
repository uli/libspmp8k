#
# Makefile
#
#

CC		= gcc
CXX		= g++
CFLAGS	= -O2 -c -Wall -g
CXXFLAGS= $(CFLAGS)

MKBOBJS	= Des.o mkbing.o
UNBOBJS = Des.o unbing.o

all		: mkbing unbing bin2c png2c
		cp -p mkbing unbing bin2c png2c ../../bin

mkbing	: $(MKBOBJS)
		$(CXX) $(MKBOBJS) -o mkbing

unbing	: $(UNBOBJS)
		$(CXX) $(UNBOBJS) -o unbing

bin2c	: bin2c.o
		$(CC) $< -o bin2c

# requires libpng
png2c	: png2c.o
		$(CC) $< -o png2c -lpng

clean	:
		rm -f *.o
		rm -f mkbing
		rm -f unbing
		rm -f bin2c
		rm -f png2c
		rm -f ../../bin/{mkbing,unbing,bin2c,png2c}
