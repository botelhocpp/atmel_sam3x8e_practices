#include "wdt.h"

#include <sam3x8e.h>

#define WDT_CLOCK_FREQ         (CHIP_FREQ_XTAL_32K/128)
#define WDT_PERIOD(_ms)        ((_ms << 8)/1000 - 1)

void WDT_Enable(uint32_t time_ms) {
    uint32_t wdv = WDT_PERIOD(time_ms);

    /* WDT reset everything, doesn't stop in Idle or Debug mode, WDD = WDV and don't trigger INT */
    WDT->WDT_MR = WDT_MR_WDRSTEN
        | WDT_MR_WDD(wdv) 
        | WDT_MR_WDV(wdv) 
        | WDT_MR_WDDBGHLT 
        | WDT_MR_WDIDLEHLT;
}

void WDT_Disable(void) {
    WDT->WDT_MR = WDT_MR_WDDIS;
}

void WDT_Reset(void) {
    WDT->WDT_CR = WDT_CR_KEY_PASSWD | WDT_CR_WDRSTT;
}
