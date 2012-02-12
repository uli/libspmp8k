# Name of this configuration. Generally the name of the OS.
CONFIG=windows

# C compiler, flags
CC=gcc
CFLAGS=-O2
#CFLAGS=-g

# NASM
NASM=nasm
NASMFLAGS=-f elf --prefix _

# How to install the program: either install.unix (the default)
# or install.null (does nothing).
INSTALL_TARGET=install.null

# What the binary is called on this platform. Windows uses z26.exe,
# everybody else uses z26 (or z26-static). No other values are allowed.
EXE=z26.exe

# Normally, we use the sdl-config script to set cflags/ldflags for us.
# You can set these manually if you need to.
SDLCONFIG=sdl-config
SDLLIBS=`$(SDLCONFIG) --libs`
SDLCFLAGS=`$(SDLCONFIG) --cflags`

# What z26-specific options shall we use for this platform?
Z26OPTS=-DWINDOWS -DC_ENGINE

# C core options.
sinclude conf/c_core.mak
