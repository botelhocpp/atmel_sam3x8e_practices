#include <sam3x8e.h>

#define LED_PIN 27

void board_init(void) {
  SystemInit();

  PMC->PMC_PCER0 |= PMC_PCER0_PID12;

  PIOB->PIO_PER |= (1 << LED_PIN);
  PIOB->PIO_OER |= (1 << LED_PIN);

  SysTick->CTRL = 0;
}

void delay_ms(int ms) {
  SysTick->VAL = 0;
  SysTick->LOAD = ms*SysTick->CALIB;
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; 

  while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));

  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; 
}

int main() {
  board_init();

  while(1) {
    PIOB->PIO_SODR |= (1 << LED_PIN);
    delay_ms(500);
    PIOB->PIO_CODR |= (1 << LED_PIN);
    delay_ms(500);
  }
}