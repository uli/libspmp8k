#
# Makefile
#
#

TARGET	= romboy
CC		= gcc

all:
	$(CC) -lusb romboy.c -o $(TARGET)
	cp romboy ../../bin

clean:
	rm -f $(TARGET)

