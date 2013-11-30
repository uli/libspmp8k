#
# Makefile
#
#

TARGET	= hubpower
CC	= gcc
CFLAGS	= -O2 -D_GNU_SOURCE -Wall -W

all: $(TARGET)
	cp -p $(TARGET) ../../bin

$(TARGET): hubpower.c
	$(CC) $(CFLAGS) hubpower.c -o $(TARGET)

clean:
	rm -f $(TARGET)

