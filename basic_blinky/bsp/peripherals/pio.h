/**
 * Copyright (c) 2024, Pedro Botelho
 * All rights reserved.
 */ 

#ifndef PIO_H
#define PIO_H

#include "common.h"

/* PIO Registers Access  */

#define PIOA_REG(offset)    REG(PIOA_BASE_ADDR, offset)
#define PIOB_REG(offset)    REG(PIOB_BASE_ADDR, offset)
#define PIOC_REG(offset)    REG(PIOC_BASE_ADDR, offset)
#define PIOD_REG(offset)    REG(PIOD_BASE_ADDR, offset)
#define PIOE_REG(offset)    REG(PIOE_BASE_ADDR, offset)
#define PIOF_REG(offset)    REG(PIOF_BASE_ADDR, offset)

/* PIO Base Addresses  */

#define PIOA_BASE_ADDR      (0x400E0E00)
#define PIOB_BASE_ADDR      (0x400E1000)
#define PIOC_BASE_ADDR      (0x400E1200)
#define PIOD_BASE_ADDR      (0x400E1400)
#define PIOE_BASE_ADDR      (0x400E1600)
#define PIOF_BASE_ADDR      (0x400E1800)

/* PIO Registers Offsets  */

#define PIO_PER             (0x0000)
#define PIO_PDR             (0x0004)
#define PIO_PSR             (0x0008)
#define PIO_OER             (0x0010)
#define PIO_ODR             (0x0014)
#define PIO_OSR             (0x0018)
#define PIO_IFER            (0x0020)
#define PIO_IFDR            (0x0024)
#define PIO_IFSR            (0x0028)
#define PIO_SODR            (0x0030)
#define PIO_CODR            (0x0034)
#define PIO_ODSR            (0x0038)
#define PIO_PDSR            (0x003C)
#define PIO_IER             (0x0040)
#define PIO_IDR             (0x0044)
#define PIO_IMR             (0x0048)
#define PIO_ISR             (0x004C)
#define PIO_MDER            (0x0050)
#define PIO_MDDR            (0x0054)
#define PIO_MDSR            (0x0058)
#define PIO_PUDR            (0x0060)
#define PIO_PUER            (0x0064)
#define PIO_PUSR            (0x0068)
#define PIO_ABSR            (0x0070)
#define PIO_SCIFSR          (0x0080)
#define PIO_DIFSR           (0x0084)
#define PIO_IFDGSR          (0x0088)
#define PIO_SCDR            (0x008C)
#define PIO_OWER            (0x00A0)
#define PIO_OWDR            (0x00A4)
#define PIO_OWSR            (0x00A8)

#endif
