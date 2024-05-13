# 
# Copyright (c) 2024, Pedro Botelho
# All rights reserved.
# 

TC := arm-none-eabi
CC := $(TC)-gcc
AS := $(TC)-as
CPY := $(TC)-objcopy
PROG := bossac
CFLAGS := -Wall -mcpu=cortex-m3 -mthumb --specs=nosys.specs -nostdlib -g -Os -Ibsp
AFLAGS := -mcpu=cortex-m3
PORT := ttyACM0

.PHONY: all build upload clean

all: build upload

build: build/ bin/ bin/blink.bin

upload: bin/blink.bin
	$(PROG) --port=$(PORT) -U true -i -u -e -w -b $^ -v -R

build/main.o: src/main.c
	$(CC) $(CFLAGS) -c $^ -o $@ 

build/start.o: src/start.s
	$(AS) $(AFLAGS) -c $^ -o $@ 

build/blink.elf: build/start.o build/main.o bsp/linker/flash.ld
	$(CC) build/start.o build/main.o -o $@ -mcpu=cortex-m3 -mthumb --specs=nosys.specs -nostdlib "-Tbsp/linker/flash.ld" "-Wl,-Map,build/blink.map" -nostartfiles

bin/blink.bin: build/blink.elf
	$(CPY) -O binary $^ $@

clean:
	rm -f build/main.o build/start.o bin/blink.bin build/blink.elf build/blink.map

distclean:
	rm -rf build bin

%/:
	mkdir -p $@
