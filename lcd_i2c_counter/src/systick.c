#include "systick.h"

#include <sam3x8e.h>

void SYSTICK_Init(void) {
  SysTick->CTRL &= ~SysTick_CTRL_CLKSOURCE_Msk;
}

void SYSTICK_DelayMs(uint32_t ms) {
  SysTick->VAL = 0; 
  SysTick->LOAD = ms*SysTick->CALIB;
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; 

  while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));

  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; 
}

void SYSTICK_DelayUs(uint32_t us) {
  SysTick->VAL = 0; 
  SysTick->LOAD = (us*SysTick->CALIB)/1000;
  SysTick->CTRL = SysTick_CTRL_ENABLE_Msk; 

  while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));

  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; 
}
