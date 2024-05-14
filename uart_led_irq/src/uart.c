#include "uart.h"

#include <sam3x8e.h>

#define UART_CALC_BAUDRATE(br)  (CHIP_FREQ_CPU_MAX/(br*16))

void UART_Init(uint32_t baudrate, bool rx_int, bool tx_int) {
    PMC->PMC_PCER0 |= PMC_PCER0_PID8;

    PIOA->PIO_PDR |= PIO_PDR_P8 | PIO_PDR_P9;
    PIOA->PIO_ABSR &= ~PIO_ABSR_P8 & ~PIO_ABSR_P9;

    UART->UART_CR = UART_CR_RXEN | UART_CR_TXEN; 
    UART->UART_MR = UART_MR_PAR_NO | UART_MR_CHMODE_NORMAL;
    UART->UART_BRGR = UART_BRGR_CD(UART_CALC_BAUDRATE(baudrate));

    if(rx_int || tx_int) {
        NVIC->ISER[0] |= (1 << UART_IRQn);
    }

    if(rx_int) {
        UART_EnableInterrupt(kUART_INTERRUPT_RX);
    }

    if(tx_int) {
        UART_EnableInterrupt(kUART_INTERRUPT_TX);
    }
}

void UART_EnableInterrupt(uart_interrupt interrupt) {
    uint32_t interrupt_mask;
    switch(interrupt) {
        case kUART_INTERRUPT_RX:
            interrupt_mask = UART_IER_RXRDY;
            break;
        case kUART_INTERRUPT_TX:
            interrupt_mask = UART_IER_TXRDY;
            break;
        default:
            return;
    }
    UART->UART_IER |= interrupt_mask;
}

void UART_DisableInterrupt(uart_interrupt interrupt) {
    uint32_t interrupt_mask;
    switch(interrupt) {
        case kUART_INTERRUPT_RX:
            interrupt_mask = UART_IDR_RXRDY;
            break;
        case kUART_INTERRUPT_TX:
            interrupt_mask = UART_IDR_TXRDY;
            break;
        default:
            return;
    }
    UART->UART_IDR |= interrupt_mask;
}

void UART_Write(uint8_t byte) {
    UART->UART_THR = UART_THR_TXCHR(byte);
}

uint8_t UART_Read(void) {
    return UART->UART_RHR & UART_RHR_RXCHR_Msk;
}

bool UART_RxReady(void) {
    return (UART->UART_SR & UART_SR_RXRDY) != 0;
}

bool UART_TxReady(void) {
    return (UART->UART_SR & UART_SR_TXRDY) != 0;
}

void UART_WaitRx(void) {
    while(!(UART->UART_SR & UART_SR_RXRDY));
}

void UART_WaitTx(void) {
    while(!(UART->UART_SR & UART_SR_TXRDY));
}

void UART_WriteBlocking(uint8_t byte) {
    UART_WaitTx();
    UART_Write(byte);
}

uint8_t UART_ReadBlocking(void) {
    UART_WaitRx();
    return UART_Read();
}

void UART_WriteString(const char *str) {
    for(int i = 0; str[i] != '\0'; i++) {
        UART_WriteBlocking(str[i]);
    }
}

uint32_t UART_ReadString(char *str, uint32_t max_size) {
    int i;
    for(i = 0; i < max_size; i++) {
        str[i] = UART_ReadBlocking();
        if(str[i] == '\n') {
            break;
        }
    }
    str[i] = '\0';

    return i;
}
