ZLIB_VERSION = 1.2.7
LIBMAD_VERSION = 0.15.1b

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
	(cd zlib-$(ZLIB_VERSION) ; CC=$(TOOLCHAIN)gcc CFLAGS="-O3 -Wall -W" CHOST="$(TOOLCHAIN_TARGET)" ./configure)

freetype: lib/libfreetype.a

lib/libfreetype.a: freetype-flat/libfreetype.a
	mkdir -p include
	mkdir -p lib
	cp -p freetype-flat/libfreetype.a lib
	cp -pr freetype-flat/include/freetype include
	cp -p freetype-flat/include/ft2build.h include

freetype-flat/libfreetype.a:
	$(MAKE) -C freetype-flat CROSS=$(TOOLCHAIN)

clean:
	$(MAKE) -C zlib-$(ZLIB_VERSION) clean
	rm -f zlib-$(ZLIB_VERSION)/zconf.h
	rm -fr lib/*.a include/*
	$(MAKE) -C freetype-flat clean
	rm -f $(LIBMAD_DIR)/*.o $(LIBMAD_DIR)/libmad.a

libmad: lib/libmad.a

lib/libmad.a: libmad-$(LIBMAD_VERSION)/libmad.a
	mkdir -p include
	mkdir -p lib
	cp -p libmad-$(LIBMAD_VERSION)/mad.h include
	cp -p libmad-$(LIBMAD_VERSION)/libmad.a lib

LIBMAD_SRC = bit.c decoder.c fixed.c frame.c huffman.c layer12.c layer3.c stream.c \
	synth.c timer.c version.c imdct_l_arm.S
LIBMAD_DIR = libmad-$(LIBMAD_VERSION)
LIBMAD_SRC2 = $(patsubst %,$(LIBMAD_DIR)/%,$(LIBMAD_SRC))
LIBMAD_OBJ = $(patsubst %.S,%.o,$(patsubst %.c,%.o,$(LIBMAD_SRC2)))

libmad-$(LIBMAD_VERSION)/libmad.a: libmad-$(LIBMAD_VERSION)/*.h $(LIBMAD_SRC2)
	cd $(LIBMAD_DIR) ; $(TOOLCHAIN)gcc -O2 -Wall -DASO_INTERLEAVE1 -DASO_IMDCT \
		-DOPT_SPEED -DOPT_SSO -DHAVE_ASSERT_H -DFPM_ARM -c $(LIBMAD_SRC)
	$(TOOLCHAIN)ar r $@ $(LIBMAD_OBJ)
