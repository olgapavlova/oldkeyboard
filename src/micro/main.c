#include "main.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * */  

// Мигаем попеременно двумя светодиодами
void blink(void) {
    DDRB |= (1 << PB0);      // пин PB0 устанавливаем как выход
    DDRD |= (1 << PD5);      // пин PD5 устанавливаем как выход
    PORTB |= (1 << PB0);     // включим один из светодиодов
    PORTD ^= (1 << PD5);     // выключим второй

    while (1) {
        PORTB ^= (1 << PB0); // инвертируем уровень на пине PB0 порта B
        PORTD ^= (1 << PD5); // инвертируем уровень на пине PD5 порта D
        _delay_ms(500);      // «делаем ничего» полсекунды
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * */  

void adc_init() {
    ADMUX = (1 << REFS0);  // опорное напряжение («попугаи»)
    ADCSRA = (1 << ADEN) |  // работай
             (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);  // помедленней 
}

uint16_t adc_read(uint8_t channel) {
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F); // канал
    ADCSRA |= (1 << ADSC);                     // начать преобразование
    while (ADCSRA & (1 << ADSC));              // ждать окончания
    return ADC;
}

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

// Посылать одну и ту же строку без конца
void uart_test (void) {
    uart_init();
    while (1) {
        uart_send_string("Hello, I am here.\r\n");
        _delay_ms(1000);
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * */  


// Точка входа, как обычно
int main(void) {
  // blink();

  adc_init();
  uart_init();

  while (1) {
      char buffer[32];
      for (uint8_t channel = 7; channel >= 4; channel--) {
          uint16_t value = adc_read(channel);
          snprintf(buffer, sizeof(buffer), "A%d: %4d\t", (7 - channel), value);
          uart_send_string(buffer);
      }
      uart_send_string("\r\n");
      _delay_ms(500);
  }
  return 0;
}
