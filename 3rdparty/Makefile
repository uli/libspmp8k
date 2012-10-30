ZLIB_VERSION = 1.2.7

all: zlib

zlib: lib/libz.a

lib/libz.a:
	mkdir -p include
	mkdir -p lib
	cp -p zlib-$(ZLIB_VERSION)/{zlib,zconf}.h include
	cp -p zlib-$(ZLIB_VERSION)/libz.a lib

zlib-$(ZLIB_VERSION)/libz.a: zlib-$(ZLIB_VERSION)/zconf.h
	make -C zlib-$(ZLIB_VERSION) libz.a

zlib-$(ZLIB_VERSION)/zconf.h:
	(cd zlib-$(ZLIB_VERSION) ; CC=arm-eabi-gcc ./configure)
