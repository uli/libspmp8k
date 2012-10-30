ZLIB_VERSION = 1.2.7

all: zlib

zlib: zlib-$(ZLIB_VERSION)/libz.a

zlib-$(ZLIB_VERSION)/libz.a: zlib-$(ZLIB_VERSION)/zconf.h
	make -C zlib-$(ZLIB_VERSION) libz.a

zlib-$(ZLIB_VERSION)/zconf.h:
	(cd zlib-$(ZLIB_VERSION) ; CC=arm-eabi-gcc ./configure)
