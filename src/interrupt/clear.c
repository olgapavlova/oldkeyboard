#include <avr/io.h>
#include <avr/wdt.h>

int main(void) {
    wdt_disable();

    DDRB |= (1 << PB0);
    PORTB |= (1 << PB0);  // просто зажигаем пин
    while (1);
}
