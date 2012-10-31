TARGET = arm-eabi

BINUTILS_VERSION = 2.22.90
GCC_VERSION = 4.7.2
NEWLIB_VERSION = 1.20.0

.PHONY: binutils gcc-1st newlib gcc-final
all: gcc-final

binutils: build-binutils/.installed

build-binutils/.installed: build-binutils/binutils/readelf
	sudo make -C build-binutils install
	touch build-binutils/.installed

build-binutils/binutils/readelf: build-binutils/Makefile
	$(MAKE) -C build-binutils

build-binutils/Makefile: binutils-$(BINUTILS_VERSION)/.extracted
	mkdir -p build-binutils
	(cd build-binutils ; ../binutils-$(BINUTILS_VERSION)/configure --target=$(TARGET))

binutils-$(BINUTILS_VERSION)/.extracted: binutils-$(BINUTILS_VERSION).tar.bz2
	tar xf binutils-$(BINUTILS_VERSION).tar.bz2
	touch binutils-$(BINUTILS_VERSION)/.extracted

gcc-1st: build-gcc-1st/.installed

build-gcc-1st/.installed: build-gcc-1st/gcc/cc1
	sudo make -C build-gcc-1st install
	touch build-gcc-1st/.installed

build-gcc-1st/gcc/cc1: build-gcc-1st/Makefile
	$(MAKE) -C build-gcc-1st

build-gcc-1st/Makefile: gcc-$(GCC_VERSION)/.extracted build-binutils/.installed
	mkdir -p build-gcc-1st
	(cd build-gcc-1st ; ../gcc-$(GCC_VERSION)/configure --target=$(TARGET) --with-gnu-as --with-gnu-ld --disable-nls --enable-languages=c --disable-libssp --disable-libgomp --disable-libmudflap --with-float=soft)

gcc-$(GCC_VERSION)/.extracted: gcc-$(GCC_VERSION).tar.bz2
	tar xf gcc-$(GCC_VERSION).tar.bz2
	touch gcc-$(GCC_VERSION)/.extracted

newlib: build-newlib/.installed

build-newlib/.installed: build-newlib/$(TARGET)/newlib/libc.a
	sudo -E make -C build-newlib install
	touch build-newlib/.installed

build-newlib/$(TARGET)/newlib/libc.a: build-newlib/Makefile
	$(MAKE) -C build-newlib

build-newlib/Makefile: newlib-$(NEWLIB_VERSION)/.extracted build-gcc-1st/.installed
	mkdir -p build-newlib
	(cd build-newlib ; ../newlib-$(NEWLIB_VERSION)/configure --disable-newlib-supplied-syscalls --target=$(TARGET))

newlib-$(NEWLIB_VERSION)/.extracted: newlib-$(NEWLIB_VERSION).tar.gz
	tar xf newlib-$(NEWLIB_VERSION).tar.gz
	touch newlib-$(NEWLIB_VERSION)/.extracted

gcc-final: build-gcc-final/.installed

build-gcc-final/.installed: build-gcc-final/gcc/cc1
	sudo make -C build-gcc-final install
	touch build-gcc-final/.installed

build-gcc-final/gcc/cc1: build-gcc-final/Makefile
	$(MAKE) -C build-gcc-final

build-gcc-final/Makefile: gcc-$(GCC_VERSION)/.extracted build-newlib/.installed
	mkdir -p build-gcc-final
	(cd build-gcc-final ; ../gcc-$(GCC_VERSION)/configure --target=$(TARGET) --with-gnu-as --with-gnu-ld --disable-nls --enable-languages=c,c++ --disable-libssp --disable-libgomp --disable-libmudflap --with-newlib --with-float=soft)

clean:
	rm -fr build-* gcc-$(GCC_VERSION) binutils-$(BINUTILS_VERSION) newlib-$(NEWLIB_VERSION)
