ZLIB_VERSION = 1.2.7

include ../main.cfg

all: zlib freetype

zlib: lib/libz.a

lib/libz.a: zlib-$(ZLIB_VERSION)/libz.a
	mkdir -p include
	mkdir -p lib
	cp -p zlib-$(ZLIB_VERSION)/{zlib,zconf}.h include
	cp -p zlib-$(ZLIB_VERSION)/libz.a lib

zlib-$(ZLIB_VERSION)/libz.a: zlib-$(ZLIB_VERSION)/zconf.h
	$(MAKE) -C zlib-$(ZLIB_VERSION) libz.a

zlib-$(ZLIB_VERSION)/zconf.h:
	(cd zlib-$(ZLIB_VERSION) ; CC=$(TOOLCHAIN)gcc ./configure)

freetype: lib/libfreetype.a

lib/libfreetype.a: freetype-flat/libfreetype.a
	mkdir -p include
	mkdir -p lib
	cp -p freetype-flat/libfreetype.a lib
	cp -a freetype-flat/include/freetype include
	cp -p freetype-flat/include/ft2build.h include

freetype-flat/libfreetype.a:
	$(MAKE) -C freetype-flat CROSS=$(TOOLCHAIN)

clean:
	$(MAKE) -C zlib-$(ZLIB_VERSION) clean
	rm -fr lib/*.a include/*
	$(MAKE) -C freetype-flat clean
