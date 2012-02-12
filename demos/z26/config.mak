# Name of this configuration. Generally the name of the OS.
CONFIG=spmp8000

# C compiler, flags
CC=arm-none-eabi-gcc
CFLAGS=

# NASM
NASM=nasm
NASMFLAGS=-f elf

# What the binary is called on this platform. Windows uses z26.exe,
# everybody else uses z26 (or z26-static). No other values are allowed.
EXE=z26

# Normally, we use the sdl-config script to set cflags/ldflags for us.
# You can set these manually if you need to.
SDLCONFIG=sdl-config
SDLLIBS=`$(SDLCONFIG) --libs`
SDLCFLAGS=`$(SDLCONFIG) --cflags`

# What z26-specific options shall we use for this platform?
Z26OPTS=-DSPMP8000

# C core options.
sinclude conf/c_core.mak
