#include "uart.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * */  

// Конфигурация UART конкретно для нашего случая
#define BAUD 9600
#define MYUBRR (F_CPU/16/BAUD-1)

// Настроить регистры микропроцессора, отвечающие за UART
void uart_init(void) {
    UBRR1H = (uint8_t)(MYUBRR >> 8);
    UBRR1L = (uint8_t)MYUBRR;
    UCSR1B = (1 << TXEN1) | (1 << RXEN1);
    UCSR1C = (1 << UCSZ11) | (1 << UCSZ10);
}

// Отправить один символ через UART
void uart_send_char(char c) {
    while (!(UCSR1A & (1 << UDRE1)));
    UDR1 = c;
}

// Отправить строку (до '\0') через UART
void uart_send_string(const char *s) {
    while (*s) {
        uart_send_char(*s++);
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * */  

// Точка входа, как обычно
#if 0
int main(void) {
  // blink();
  // uart_test();

  uart_init();
  adc_init();

  while (1) {
    kb_report();
    _delay_ms(500);
  }

  return 0;
}
#endif
