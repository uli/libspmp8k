TARGET = arm-eabi

BINUTILS_VERSION = 2.22.90
GCC_VERSION = 4.7.2
NEWLIB_VERSION = 1.20.0

SUDO = sudo -E

# extra configure options for OSX
#EXTRA_GCC_CONFIG = -with-gmp=/opt/local --with-mpfr=/opt/local --with-mpc=/opt/local --with-libiconv-prefix=/usr

# extra configure options for mingw32/cross
#CONFIGURE_FLAGS = --host=i686-w64-mingw32
#INSTALL_FLAGS = DESTDIR=/sources/spmp8k/libspmp8k/toolchain/win32

.PHONY: binutils gcc-1st newlib gcc-final
all: gcc-final

binutils: build-binutils/.installed

build-binutils/.installed: build-binutils/binutils/readelf.o
	$(SUDO) make -C build-binutils install $(INSTALL_FLAGS)
	touch build-binutils/.installed

build-binutils/binutils/readelf: build-binutils/Makefile
	$(MAKE) -C build-binutils

build-binutils/Makefile: binutils-$(BINUTILS_VERSION)/.extracted
	mkdir -p build-binutils
	(cd build-binutils ; ../binutils-$(BINUTILS_VERSION)/configure --target=$(TARGET) --disable-werror $(CONFIGURE_FLAGS))

binutils-$(BINUTILS_VERSION)/.extracted: binutils-$(BINUTILS_VERSION).tar.bz2
	tar xf binutils-$(BINUTILS_VERSION).tar.bz2
	touch binutils-$(BINUTILS_VERSION)/.extracted

gcc-1st: build-gcc-1st/.installed

build-gcc-1st/.installed: build-gcc-1st/gcc/crtn.o
	$(SUDO) make -C build-gcc-1st install $(INSTALL_FLAGS)
	touch build-gcc-1st/.installed

build-gcc-1st/gcc/cc1: build-gcc-1st/Makefile
	$(MAKE) -C build-gcc-1st

build-gcc-1st/Makefile: gcc-$(GCC_VERSION)/.extracted build-binutils/.installed
	mkdir -p build-gcc-1st
	(cd build-gcc-1st ; ../gcc-$(GCC_VERSION)/configure --target=$(TARGET) --with-gnu-as --with-gnu-ld --disable-nls --enable-languages=c --disable-libssp --disable-libgomp --disable-libmudflap --with-float=soft $(EXTRA_GCC_CONFIG) $(CONFIGURE_FLAGS))

gcc-$(GCC_VERSION)/.extracted: gcc-$(GCC_VERSION).tar.bz2
	tar xf gcc-$(GCC_VERSION).tar.bz2
	touch gcc-$(GCC_VERSION)/.extracted

newlib: build-newlib/.installed

build-newlib/.installed: build-newlib/$(TARGET)/newlib/libc.a
	$(SUDO) make -C build-newlib install $(INSTALL_FLAGS)
	touch build-newlib/.installed

build-newlib/$(TARGET)/newlib/libc.a: build-newlib/Makefile
	$(MAKE) -C build-newlib

build-newlib/Makefile: newlib-$(NEWLIB_VERSION)/.extracted build-gcc-1st/.installed
	mkdir -p build-newlib
	(cd build-newlib ; ../newlib-$(NEWLIB_VERSION)/configure --disable-newlib-supplied-syscalls --target=$(TARGET) $(CONFIGURE_FLAGS))

newlib-$(NEWLIB_VERSION)/.extracted: newlib-$(NEWLIB_VERSION).tar.gz
	tar xf newlib-$(NEWLIB_VERSION).tar.gz
	touch newlib-$(NEWLIB_VERSION)/.extracted

gcc-final: build-gcc-final/.installed

build-gcc-final/.installed: build-gcc-final/gcc/cc1
	$(SUDO) make -C build-gcc-final install $(INSTALL_FLAGS)
	touch build-gcc-final/.installed

build-gcc-final/gcc/cc1: build-gcc-final/Makefile
	$(MAKE) -C build-gcc-final

build-gcc-final/Makefile: gcc-$(GCC_VERSION)/.extracted build-newlib/.installed
	mkdir -p build-gcc-final
	(cd build-gcc-final ; ../gcc-$(GCC_VERSION)/configure --target=$(TARGET) --with-gnu-as --with-gnu-ld --disable-nls --enable-languages=c,c++ --disable-libssp --disable-libgomp --disable-libmudflap --with-newlib --with-float=soft $(EXTRA_GCC_CONFIG) $(CONFIGURE_FLAGS))

clean:
	rm -fr build-* gcc-$(GCC_VERSION) binutils-$(BINUTILS_VERSION) newlib-$(NEWLIB_VERSION)
