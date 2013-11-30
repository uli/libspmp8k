#
# libspmp8k
#
# tools makefile
#
#

all:	build-bingtools build-chksum build-el2mmp build-packboot \
	build-romboy build-sprdump build-usb_console build-hubpower

clean:
	$(MAKE) -C bingtools clean
	$(MAKE) -C chksum clean
	$(MAKE) -C elf2mmp clean
	$(MAKE) -C packboot clean
	$(MAKE) -C romboy clean
	$(MAKE) -C sprdump clean
	$(MAKE) -C usb_console clean

build-bingtools:
	$(MAKE) -C bingtools

build-chksum:
	$(MAKE) -C chksum

build-el2mmp:
	$(MAKE) -C elf2mmp

build-packboot:
	$(MAKE) -C packboot

build-romboy:
	$(MAKE) -C romboy

build-sprdump:
	$(MAKE) -C sprdump

build-usb_console:
	$(MAKE) -C usb_console

build-hubpower:
	$(MAKE) -C hubpower
