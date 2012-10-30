CROSS = arm-eabi-
CFLAGS = -Wall -W -Wno-unused -Ibuild_include -Iinclude -DFT2_BUILD_LIBRARY -O2 -mcpu=arm926ej-s -g
CC = $(CROSS)gcc

LIB = libfreetype.a
SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c,%.o,$(SRCS))

all: $(LIB)

$(LIB): $(OBJS)
	rm -f $(LIB)
	$(CROSS)ar r $(LIB) $(OBJS)

clean:
	rm -f $(LIB) $(OBJS)
