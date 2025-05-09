#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    // Установим PB0 (пин 17) как выход
    DDRB |= (1 << PB0);

    while (1) {
        PORTB ^= (1 << PB0); // Инвертировать состояние
        _delay_ms(500);
    }
}
