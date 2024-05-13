# 
# Copyright (c) 2024, Pedro Botelho
# All rights reserved.
# 

.syntax unified

.section .vectors
    .long  _stack_top
    .long  _start

.section .text

.thumb_func
.global _start
_start:

_load_data_section:
    ldr r0, =_lmadata
    ldr r1, =_sdata
    ldr r2, =_edata
    cmp r0, r1
    beq _load_bss_section
_load_data:
    cmp r1, r2
    bge _load_bss_section
    ldr r3, [r0], #4
    str r3, [r1], #4
    b _load_data

_load_bss_section:
    ldr r1, =_sbss
    ldr r2, =_ebss
    eor r3, r3, r3
_load_bss:
    cmp r1, r2
    bge _goto_main
    str r3, [r1], #4
    b _load_bss

_goto_main:
    ldr r0, =main
    bx r0
    b .
