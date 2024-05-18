#ifndef _WDT_H_
#define _WDT_H_

#include <stdint.h>
#include <stdbool.h>

void WDT_Enable(uint32_t time_ms);

void WDT_Disable(void);

void WDT_Reset(void);

#endif
