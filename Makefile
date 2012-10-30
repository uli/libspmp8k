#
# libspmp8k
#
# root makefile
#
#

SPMP8K_BINDIR	= ./bin

all: build-tools build-lib build-3rdparty build-demos

clean:
	make -C libspmp8k clean
	make -C demos clean
	make -C tools clean

build-lib:
	make -C libspmp8k

build-demos:
	make -C demos

build-tools:
	make -C tools

build-3rdparty:
	make -C 3rdparty
