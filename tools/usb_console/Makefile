#
# Makefile
#
#

TARGET	= usb_console
CC	= gcc
CFLAGS	= -O2 -D_GNU_SOURCE

all: $(TARGET)
	cp -p $(TARGET) ../../bin

$(TARGET): usb_console.c
	$(CC) $(CFLAGS) usb_console.c -o $(TARGET)

clean:
	rm -f $(TARGET)

