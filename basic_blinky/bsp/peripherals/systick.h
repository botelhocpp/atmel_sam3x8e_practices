/**
 * Copyright (c) 2024, Pedro Botelho
 * All rights reserved.
 */ 

#ifndef SYSTICK_H
#define SYSTICK_H

#include "common.h"

/* SYSTICK Registers Access  */

#define SYSTICK_REG(offset)    REG(SYSTICK_BASE_ADDR, offset)

/* SYSTICK Base Addresses  */

#define SYSTICK_BASE_ADDR       (0xE000E010)

/* SYSTICK Registers Offsets  */

#define SYSTICK_CTRL            (0x0000)
#define SYSTICK_LOAD            (0x0004)
#define SYSTICK_VAL             (0x0008)
#define SYSTICK_CALIB           (0x000C)

/* SYSTICK Registers Fields Mask */

#define SYSTICK_CTRL_ENABLE     (1 << 0)
#define SYSTICK_CTRL_TICKINT    (1 << 1)
#define SYSTICK_CTRL_CLKSOURCE  (1 << 2)
#define SYSTICK_CTRL_COUNTFLAG  (1 << 16)

#endif
