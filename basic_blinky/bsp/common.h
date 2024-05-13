/**
 * Copyright (c) 2024, Pedro Botelho
 * All rights reserved.
 */ 

#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

#define REG(base, offset)   (*((volatile uint32_t *) (base + offset)))

#endif
