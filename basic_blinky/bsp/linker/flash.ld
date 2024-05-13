/**
 * Copyright (c) 2024, Pedro Botelho
 * All rights reserved.
 */ 

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)

MEMORY
{
	flash (rx)  : ORIGIN = 0x00080000, LENGTH = 0x00080000 /* Flash = 512K */
	sram (rwx)  : ORIGIN = 0x20070000, LENGTH = 0x00018000 /* SRAM = 96K */
}

PHDRS
{
  flash PT_LOAD;
  sram_init PT_LOAD;
  sram PT_NULL;
}

SECTIONS
{
    _stack_size = DEFINED(_stack_size) ? _stack_size : 1K;
    
    .text :
    {
        . = ALIGN(4);
        KEEP(*(.vectors));
    
        . = ALIGN(4);
        *(.text .text.*);

        . = ALIGN(4);
        *(.rodata .rodata.*);
    } > flash AT>flash : flash

    .lmadata :
    {
        . = ALIGN(4);
        PROVIDE(_lmadata = .);
    } > flash AT>flash : flash

    .data : 
    {
        . = ALIGN(4);
        PROVIDE(_sdata = .);

        . = ALIGN(4);
        *(.data .data.*);

        . = ALIGN(4);
        PROVIDE(_edata = .);
    } > sram AT>flash : sram_init
    
    .bss :
    {
        . = ALIGN(4);
        PROVIDE(_sbss = .);
        
        . = ALIGN(4);
        *(.bss .bss.*);

        . = ALIGN(4);
        PROVIDE(_ebss = .);
    } > sram AT>sram : sram
    
    .stack ORIGIN(sram) + LENGTH(sram) - _stack_size :
    {
        PROVIDE(_stack_limit = .);
        . = _stack_size;
        PROVIDE(_stack_top = .);
    } > sram AT>sram : sram
}
