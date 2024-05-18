#include <sam3x8e.h>

#include "utils.h"
#include "twi.h"
#include "uart.h"
#include "systick.h"
#include "wdt.h"
#include "lcd.h"

#define LCD_TWI             (TWI1)
#define WDT_PERIOD_MS       (1000)

static void BOARD_Init(void);

int main() {
  BOARD_Init();

  lcd_t lcd_handle = {
    .address = LCD_NXP_EXPANSOR_ADDRESS,
    .bus = LCD_TWI,
    .backlight = true
  };

  /* Tries to Initialize the LCD */
  while(!LCD_Init(&lcd_handle));

  if(!LCD_CreateBigNumbers(&lcd_handle)) {
    while(1);
  }

  if(!LCD_SetCursor(&lcd_handle, 0, 0)) {
    while(1);
  }
  if(!LCD_Print(&lcd_handle, "Counter:")) {
    while(1);
  }

  if(!LCD_SetCursor(&lcd_handle, 0, 1)) {
    while(1);
  }
  if(!LCD_Print(&lcd_handle, "To 9999>")) {
    while(1);
  }

  int d3 = 0;
  int d2 = 0;
  int d1 = 0;
  int d0 = 0;

  while(1) {
    WDT_Reset();
    if(
      !LCD_WriteBigNumber(&lcd_handle, 14, d0)
      || !LCD_WriteBigNumber(&lcd_handle, 12, d1)
      || !LCD_WriteBigNumber(&lcd_handle, 10, d2)
      || !LCD_WriteBigNumber(&lcd_handle, 8, d3)
    ) {
      while(1);
    }

    if(d0 == 9) {
      if(d1 == 9) {
        if(d2 == 9) {
          d3 = (d3 + 1) % 10;
        }
        d2 = (d2 + 1) % 10;
      }
      d1 = (d1 + 1) % 10;
    }
    d0 = (d0 + 1) % 10;

    delay_ms(1000);
  }
}

void BOARD_Init(void) {
  SystemInit();

  WDT_Enable(WDT_PERIOD_MS);

  SYSTICK_Init();

  TWI_Init(LCD_TWI);
}
