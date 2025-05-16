#include <avr/io.h>
#include <util/delay.h>

void gpio_init() {
    DDRB |= (1 << PB0); // Настраиваем PB0 как выход
}

int main(void) {
    gpio_init(); // Настройка GPIO

    while (1) {
        PORTB ^= (1 << PB0); // Инвертируем пин
        _delay_ms(500);      // Задержка 500 мс
    }
}
