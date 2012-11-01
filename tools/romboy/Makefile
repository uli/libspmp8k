#
# Makefile
#
#

TARGET	= romboy
CC		= gcc

all: $(TARGET)
$(TARGET): romboy.c
	$(CC) -lusb romboy.c -o $(TARGET)
	cp romboy ../../bin

clean:
	rm -f $(TARGET)

