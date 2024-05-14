#include <sam3x8e.h>

#include "gpio.h"
#include "uart.h"

#define BAUDRATE  (9600)

#define LED_PORT  (PIOB)
#define LED_PIN   (27)

#define WELCOME_MESSAGE   ("LED Control\n\r")
#define LED_ON_MESSAGE    ("LED ON!\n\r")
#define LED_OFF_MESSAGE   ("LED OFF!\n\r")

void BOARD_Init(void) {
  SystemInit();

  PMC->PMC_PCER0 |= ID_PIOB;

  WDT->WDT_MR |= WDT_MR_WDDIS;

  PIO_Init(LED_PORT, LED_PIN, kPIO_DIRECTION_OUTPUT);

  UART_Init(BAUDRATE, true, true);
}

typedef struct message_t {
  int8_t *data;
  uint32_t size;
  uint32_t pointer;
  bool finished;
} message_t;

message_t message_tx = {
  .data = WELCOME_MESSAGE,
  .size = sizeof(WELCOME_MESSAGE) - 1,
  .pointer = 0,
  .finished = false
};

uint8_t message_rx = '0';

int main() {
  BOARD_Init();

  while(1) {
    if(message_rx == '1') {
      if(message_tx.finished) {
        message_tx = (message_t) {
          .data = LED_ON_MESSAGE,
          .size = sizeof(LED_ON_MESSAGE) - 1,
          .pointer = 0,
          .finished = false
        };
        UART_EnableInterrupt(kUART_INTERRUPT_TX);
        message_rx = 0;
      }
      PIO_Write(LED_PORT, LED_PIN, 1);
    }
    else if(message_rx == '0') {
      if(message_tx.finished) {
        message_tx = (message_t) {
          .data = LED_OFF_MESSAGE,
          .size = sizeof(LED_OFF_MESSAGE) - 1,
          .pointer = 0,
          .finished = false
        };
        UART_EnableInterrupt(kUART_INTERRUPT_TX);
        message_rx = 0;
      }
      PIO_Write(LED_PORT, LED_PIN, 0);
    }
  }
}

void UART_Handler(void) {
  if(UART_RxReady()) {
    message_rx = UART_Read();
  }
  else if(UART_TxReady()) {
    if(message_tx.pointer < message_tx.size) {
      UART_Write(message_tx.data[message_tx.pointer++]);
    }
    else {
      message_tx.finished = true;
      UART_DisableInterrupt(kUART_INTERRUPT_TX);
    }
  } 
}
