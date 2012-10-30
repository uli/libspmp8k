#
# libspmp8k
#
# root makefile
#
#

SPMP8K_BINDIR	= ./bin

all: build-tools build-lib build-3rdparty build-demos

clean:
	$(MAKE) -C libspmp8k clean
	$(MAKE) -C libgame clean
	$(MAKE) -C demos clean
	$(MAKE) -C tools clean
	$(MAKE) -C 3rdparty clean

build-lib:
	$(MAKE) -C libspmp8k
	$(MAKE) -C libgame

build-demos: build-lib
	$(MAKE) -C demos

build-tools:
	$(MAKE) -C tools

build-3rdparty:
	$(MAKE) -C 3rdparty
