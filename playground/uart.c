#include <avr/io.h>
#include <util/delay.h>

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
    UART_init(MYUBRR);
    while (1) {
        UART_sendString("Hello from Pro Micro!\r\n");
        _delay_ms(1000);
    }
}
