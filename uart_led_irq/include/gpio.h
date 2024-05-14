#ifndef _PIO_H_
#define _PIO_H_

#include <sam3x8e.h>

#include <stdint.h>
#include <stdbool.h>

typedef enum PIO_direction {
    kPIO_DIRECTION_INPUT,
    kPIO_DIRECTION_OUTPUT,
} PIO_direction;

void PIO_Init(Pio *port, uint32_t pin, PIO_direction direction);

void PIO_Write(Pio *port, uint32_t pin, uint32_t value);

uint32_t PIO_Read(Pio *port, uint32_t pin);

#endif
