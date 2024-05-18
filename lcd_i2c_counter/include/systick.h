#ifndef _SYSTICK_H_
#define _SYSTICK_H_

#include <stdint.h>

#ifndef delay_ms
#define delay_ms(_ms) SYSTICK_DelayMs(_ms)
#define delay_us(_us) SYSTICK_DelayUs(_us)
#endif 

void SYSTICK_Init(void);

void SYSTICK_DelayMs(uint32_t ms);

void SYSTICK_DelayUs(uint32_t us);

#endif
