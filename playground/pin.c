#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

void ADC_init() {
    ADMUX = (1 << REFS0);         // АЦП от 5В, вход — A0 (PF7, ADC7)
    ADCSRA = (1 << ADEN) |        // Включаем АЦП
             (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Предделитель на 128 (16МГц/128 = 125кГц)
}

uint16_t ADC_read(uint8_t channel) {
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F); // Выбор канала
    ADCSRA |= (1 << ADSC);                     // Начать преобразование
    while (ADCSRA & (1 << ADSC));              // Ждать окончания
    return ADC;
}

// Настраиваем скорость UART (9600 бод)
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

void UART_init(unsigned int ubrr) {
    // Установка скорости (регистр делителя частоты)
    UBRR1H = (unsigned char)(ubrr >> 8);
    UBRR1L = (unsigned char)ubrr;

    // Настройка: 8 бит данных, 1 стоп-бит
    UCSR1B = (1 << TXEN1) | (1 << RXEN1);  // Включаем передатчик и приёмник
    UCSR1C = (1 << UCSZ11) | (1 << UCSZ10); // 8 бит данных
}

void UART_sendChar(char data) {
    while (!(UCSR1A & (1 << UDRE1))); // Ждём, пока буфер освободится
    UDR1 = data;  // Отправляем байт
}

void UART_sendString(const char *str) {
    while (*str) {
        UART_sendChar(*str++);
    }
}

int main(void) {
    ADC_init();
    
    UART_init(MYUBRR);

    while (1) {
        char buffer[32];
        for (uint8_t channel = 7; channel >= 4; channel--) {
            uint16_t value = ADC_read(channel);
            snprintf(buffer, sizeof(buffer), "A%d: %4d\t", (7 - channel), value);
            UART_sendString(buffer);
        }
        UART_sendString("\r\n");
        _delay_ms(500);
    }
}

