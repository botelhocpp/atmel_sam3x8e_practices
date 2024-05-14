#ifndef _UART_H_
#define _UART_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum uart_interrupt {
    kUART_INTERRUPT_RX,
    kUART_INTERRUPT_TX,
} uart_interrupt;

#define UART_MIN_BAUDRATE   (1)
#define UART_MAX_BAUDRATE   (65535)

void UART_Init(uint32_t baudrate, bool rx_int, bool tx_int);

void UART_EnableInterrupt(uart_interrupt interrupt);

void UART_DisableInterrupt(uart_interrupt interrupt);

void UART_Write(uint8_t byte);

uint8_t UART_Read(void);

bool UART_RxReady(void);

bool UART_TxReady(void);

void UART_WaitRx(void);

void UART_WaitTx(void);

void UART_WaitRx(void);

void UART_WriteBlocking(uint8_t byte);

uint8_t UART_ReadBlocking(void);

void UART_WriteString(const char *str);

uint32_t UART_ReadString(char *str, uint32_t max_size);

#endif
