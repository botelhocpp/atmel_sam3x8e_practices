#include "gpio.h"

void PIO_Init(Pio *port, uint32_t pin, PIO_direction direction) {
  port->PIO_PER |= (1 << pin);

  switch(direction) {
    case kPIO_DIRECTION_INPUT:
        port->PIO_ODR |= (1 << pin);
        break;
    case kPIO_DIRECTION_OUTPUT:
        port->PIO_OER |= (1 << pin);
        break;
    default:
        return;
  }
}

void PIO_Write(Pio *port, uint32_t pin, uint32_t value) {
    if(value != 0) {
        port->PIO_SODR |= (1 << pin);
    }
    else {
        port->PIO_CODR |= (1 << pin);
    }
}

uint32_t PIO_Read(Pio *port, uint32_t pin) {
    return (port->PIO_PDSR >> pin) & 1;
}
