#
# libspmp8k
#
# demos makefile
#
#

all: build-rgbgame build-keydemo build-doom110 build-z26 build-adbg build-audio

clean:
	$(MAKE) -C rgbgame clean
#	$(MAKE) -C uarttest clean
	$(MAKE) -C keydemo clean
	$(MAKE) -C doom110 clean
	$(MAKE) -C z26 clean
	$(MAKE) -C audio clean
	$(MAKE) -C diag clean

#build-uarttest:
#	$(MAKE) -C uarttest

build-rgbgame:
	$(MAKE) -C rgbgame

build-keydemo:
	$(MAKE) -C keydemo

build-doom110:
	$(MAKE) -C doom110

build-z26:
	$(MAKE) -C z26

build-adbg:
	$(MAKE) -C adbg

build-audio:
	$(MAKE) -C audio
