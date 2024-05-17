#ifndef _SYSTICK_H_
#define _SYSTICK_H_

#include <stdint.h>

#ifndef delay_ms
#define delay_ms SYSTICK_DelayMs
#endif 

void SYSTICK_Init(void);

void SYSTICK_DelayMs(uint32_t ms);

#endif
