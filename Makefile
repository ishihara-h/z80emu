CC = gcc
CFLAGS = -Wall -ansi -pedantic -O2 -fomit-frame-pointer

all: monitor

tables.h: maketables.c
	$(CC) -Wall $< -o maketables
	./maketables > $@

z80emu.o: z80emu.c z80emu.h instructions.h macros.h tables.h
	$(CC) $(CFLAGS) -c $<

monitor.o: monitor.c z80emu.h
	$(CC) -Wall -c $<

OBJECT_FILES = monitor.o z80emu.o

monitor: $(OBJECT_FILES)
	$(CC) $(OBJECT_FILES) -o $@
