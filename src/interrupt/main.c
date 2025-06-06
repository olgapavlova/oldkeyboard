#include <avr/io.h>
#include <avr/interrupt.h>

#define LED_PIN PD5  // встроенный TX светодиод на Pro Micro

ISR(TIMER1_OVF_vect) {
    // Мигание при каждом переполнении (примерно раз в 4.19 сек)
    PORTD ^= (1 << LED_PIN);
}

int main(void) {
    // 🔒 Полный сброс всего лишнего
    TIMSK1 = 0;
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;

    ADCSRA = 0;
    ADCSRB = 0;
    EIMSK = 0;

    DDRB |= (1 << PB0);
    PORTB |= (1 << PB0);  // ← должен загореться сразу после прошивки

    // 💡 Настраиваем PD5 как выход
    DDRD |= (1 << LED_PIN);
    PORTD &= ~(1 << LED_PIN);

    // ⏱ Таймер 1: Normal mode + делитель 1024
    TCCR1B = (1 << CS12) | (1 << CS10);  // clk/1024
    TIMSK1 |= (1 << TOIE1);             // Разрешаем прерывание по переполнению

    // 🚀 Разрешаем глобальные прерывания
    sei();

    // ⏳ Ждём прерываний
    while (1) {
        // пусто
    }
}
