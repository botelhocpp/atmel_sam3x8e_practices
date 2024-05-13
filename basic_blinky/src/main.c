/**
 * Copyright (c) 2024, Pedro Botelho
 * All rights reserved.
 */ 

#include "bsp.h"

#define LED_PIN 27

#define SYSTICK_FREQ      (CPU_FREQ/8)
#define SYSTICK_COUNT_MS  (SYSTICK_FREQ/1000)

void SystemInit(void) {
  /* Enable PIOB Clock */
  PMC_REG(PMC_PCER0) |= 1 << ID_PIOB;

  /* Setup PTB27 */
  PIOB_REG(PIO_PER) |= 1 << LED_PIN;
  PIOB_REG(PIO_OER) |= 1 << LED_PIN;
  PIOB_REG(PIO_OWER) |= 1 << LED_PIN;

  /* Setup SYSTICK to run at 500kHz */
  SYSTICK_REG(SYSTICK_CTRL) &= ~SYSTICK_CTRL_CLKSOURCE; 
}

void delay(uint32_t ms) {
  SYSTICK_REG(SYSTICK_VAL) = 0;
  SYSTICK_REG(SYSTICK_LOAD) = ms*SYSTICK_COUNT_MS;
  SYSTICK_REG(SYSTICK_CTRL) |= SYSTICK_CTRL_ENABLE;

  while(!(SYSTICK_REG(SYSTICK_CTRL) & SYSTICK_CTRL_COUNTFLAG));
  
  SYSTICK_REG(SYSTICK_CTRL) &= ~SYSTICK_CTRL_ENABLE;
}

int main() {
  SystemInit();

  while (1) {
    PIOB_REG(PIO_SODR) |= 1 << LED_PIN;
    delay(500);
    PIOB_REG(PIO_CODR) |= 1 << LED_PIN;
    delay(500);
  }
}
