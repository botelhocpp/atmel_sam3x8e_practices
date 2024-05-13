/**
 * Copyright (c) 2024, Pedro Botelho
 * All rights reserved.
 */ 

#ifndef PMC_H
#define PMC_H

#include "common.h"

/* PMC Registers Access  */

#define PMC_REG(offset)    REG(PMC_BASE_ADDR, offset)

/* PMC Base Addresses  */

#define PMC_BASE_ADDR      (0x400E0600)

/* PMC Registers Offsets  */

#define PMC_PCER0           (0x0010)

#endif
