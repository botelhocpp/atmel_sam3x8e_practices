#include <sam3x8e.h>

#include "utils.h"
#include "twi.h"
#include "uart.h"
#include "systick.h"
#include "wdt.h"
#include "accelerometer.h"

#define ACCEL_TWI           (TWI0)
#define WDT_PERIOD_MS       (1000)

static void SERIAL_Clear(void);

static void BOARD_Init(void);

int main() {
  BOARD_Init();

  while(!ACCEL_CheckDevice(ACCEL_TWI)) {
    SERIAL_Clear();
    UART_WriteString("[error] MPU6050 not found! Trying again...\n\r");
    delay_ms(200);
  }

  while(!ACCEL_Init(ACCEL_TWI)) {
    SERIAL_Clear();
    UART_WriteString("[error] MPU6050 not ready yet...\n\r");
    delay_ms(200);
  }

  UART_WriteString("[ok] MPU6050 is ready!\n\r");
  delay_ms(500);

  axis_t axis_angles;
  float temperature;
  static char buffer[33];
  while(1) {
      WDT_Reset();

      if(!ACCEL_GetAngles(ACCEL_TWI, &axis_angles) || !ACCEL_GetTemperature(ACCEL_TWI, &temperature)) {
        UART_WriteString("[error] MPU6050 disconnected.\n\r");

        ACCEL_Reset(ACCEL_TWI);
        delay_ms(100);

        while(!ACCEL_Init(ACCEL_TWI)) {
          SERIAL_Clear();
          UART_WriteString("[error] MPU6050 disconnected. Trying to connect...\n\r");
          delay_ms(200);
        }
        continue;
      }

      /* Faulty wiring could quickly turn off and on, setting sleep mode in reset */
      if(ACCEL_IsAsleep(ACCEL_TWI)) {
        ACCEL_DisableSleepMode(ACCEL_TWI);
      }

      SERIAL_Clear();

      UART_WriteString("X = ");
      itoa(axis_angles.y, buffer, 10);
      UART_WriteString(buffer);
      UART_WriteString("\n\r");

      UART_WriteString("Y = ");
      itoa(axis_angles.x, buffer, 10);
      UART_WriteString(buffer);
      UART_WriteString("\n\r");

      UART_WriteString("Z = ");
      itoa(axis_angles.z, buffer, 10);
      UART_WriteString(buffer);
      UART_WriteString("\n\r");

      UART_WriteString("T = ");
      ftoa(temperature, buffer, 2);
      UART_WriteString(buffer);
      UART_WriteString("\n\r");

      delay_ms(200);
  }
}

void SERIAL_Clear(void) {
    UART_WriteString("\033[H\033[J\r");
}

void BOARD_Init(void) {
  SystemInit();

  WDT_Enable(WDT_PERIOD_MS);

  SYSTICK_Init();

  TWI_Init(TWI0);

  UART_Init(9600, false, false);
}
