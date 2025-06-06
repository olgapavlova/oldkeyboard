#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define F_CPU 16000000UL
#define BAUD 9600
#define UBRR_VALUE ((F_CPU / (16UL * BAUD)) - 1)
#define LED_PIN PD5

volatile uint8_t adc_event = 0;

void uart_init() {
    UBRR1H = (uint8_t)(UBRR_VALUE >> 8);
    UBRR1L = (uint8_t)(UBRR_VALUE);
    UCSR1B = (1 << TXEN1) | (1 << RXEN1);
    UCSR1C = (1 << UCSZ11) | (1 << UCSZ10);
}

void uart_send(char c) {
    while (!(UCSR1A & (1 << UDRE1)));
    UDR1 = c;
}

void led_init() {
    DDRD |= (1 << LED_PIN);     // LED как выход
    PORTD &= ~(1 << LED_PIN);   // Выключить
}

void adc_init() {
    // Используем канал ADC0 (пин A0), опорное напряжение AVcc
    ADMUX = (1 << REFS0);  // AVcc с внешним конденсатором на AREF
    ADMUX = (ADMUX & 0xF0) | 0x07;

    // Включаем АЦП, прерывания, предделитель = 128 (16 МГц / 128 = 125 кГц)
    ADCSRA = (1 << ADEN)  |  // включить АЦП
             (1 << ADIE)  |  // разрешить прерывания
             (1 << ADATE) |  // автотриггер (free running)
             (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

    ADCSRB &= ~((1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0));

    // Запускаем первое преобразование
    ADCSRA |= (1 << ADSC);
}

/*
ISR(BADISR_vect) {
    uart_send('*');  // если ты видишь *, значит вектор сброшен
}
*/

ISR(ADC_vect) {
    uint16_t result = ADC;  // читаем результат

    if (result != 0) {
      adc_event = 1;
    }

}

int main(void) {
    led_init();
    // uart_init();
    adc_init();

    sei();  // включаем глобальные прерывания

    // extern void __vector_21(void);
    // void (*tmp)(void) = __vector_21;  // ADC_vect = вектор 21

    while (1) {
      // Запускаем следующее преобразование
          PORTD ^= (1 << LED_PIN);
      if (adc_event) {
          _delay_ms(100);
          adc_event = 0;
        }
    }
}
