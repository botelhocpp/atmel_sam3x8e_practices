#include <sam3x8e.h>

#include <string.h>

#define UART_DEFAULT_BAUDRATE   (9600)
#define UART_CALC_BAUDRATE(br)  (CHIP_FREQ_CPU_MAX/(br*16))
#define UART_BAUDRATE           (UART_CALC_BAUDRATE(UART_DEFAULT_BAUDRATE))

#define LED_PIN PIO_PB27

void gpio_init(void) {
  /* Enable PIOB Clock */
  PMC->PMC_PCER0 |= ID_PIOB;

  /* Peripheral control for PA8 */
  PIOB->PIO_PER |= LED_PIN;
  PIOB->PIO_OER |= LED_PIN;
}

void uart_init(void) {
  /* Enable UART Clock */
  PMC->PMC_PCER0 |= PMC_PCER0_PID11 | PMC_PCER0_PID8;

  /* Enable Peripheral control for PA8 and PA9 */
  PIOA->PIO_PDR |= PIO_PDR_P8 | PIO_PDR_P9;

  /* Select Peripheral A (UART) for PA8 and PA9 */
  PIOA->PIO_ABSR &= ~PIO_ABSR_P8 & ~PIO_ABSR_P9;

  /* Enables Transmitter (RX) and Receiver (RX) */
  UART->UART_CR = UART_CR_RXEN | UART_CR_TXEN; 

  /* Disables Parity Check and set Channel Normal Mode */
  UART->UART_MR = UART_MR_PAR_NO | UART_MR_CHMODE_NORMAL;

  /* Set UART baudrate to 9600 bps */
  UART->UART_BRGR = UART_BRGR_CD(UART_BAUDRATE);
}

void uart_write(uint8_t byte) {
  while(!(UART->UART_SR & UART_SR_TXRDY));

  UART->UART_THR = UART_THR_TXCHR(byte);
}

void uart_print(const char *str) {
  for(int i = 0; str[i] != '\0'; i++) {
    uart_write(str[i]);
  }
}

uint8_t uart_read(void) {
  while(!(UART->UART_SR & UART_SR_RXRDY));

  return UART->UART_RHR & UART_RHR_RXCHR_Msk;
}

uint32_t uart_scan(char *str, uint32_t max_size) {
  int i;
  for(i = 0; i < max_size; i++) {
    str[i] = uart_read();
    if(str[i] == '\n') {
      break;
    }
  }
  str[i] = '\0';

  return i;
}

void board_init(void) {
  SystemInit();

  gpio_init();

  uart_init();

  SysTick->CTRL &= ~SysTick_CTRL_CLKSOURCE_Msk;
}

void delay_ms(int ms) {
  SysTick->VAL = 0;
  SysTick->LOAD = ms*SysTick->CALIB;
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; 

  while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));

  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; 
}

void serial_echo(char *str, uint32_t max_size) {
  int i;
  for(i = 0; i < max_size; i++) {
    str[i] = uart_read();
    uart_write(str[i]);
    if(str[i] == '\n') {
      break;
    }
  }
  
  str[i] = '\0';
  uart_write('\n');
}

int main() {
  char buffer[256];
  
  board_init();

  while(1) {
    uart_print("$user > ");
    serial_echo(buffer, sizeof(buffer));
    if(buffer[0] == '1') {
      PIOB->PIO_SODR |= LED_PIN;
    }
    else if(buffer[0] == '2') {
      PIOB->PIO_CODR |= LED_PIN;
    }
  }
}
