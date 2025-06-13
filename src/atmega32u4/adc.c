#include "adc.h"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * */  

// Инициируем АЦП
void adc_init(void) {
    ADMUX = (1 << REFS0) ;  // опорное напряжение («попугаи»)
    ADCSRA = (1 << ADEN) |  // работай
              (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);  // помедленней 
}

// Читаем значение на пине
uint16_t adc_read(uint8_t pin) {
    ADMUX = (ADMUX & 0xF0) | (pin & 0x0F);     // пин 
    ADCSRA |= (1 << ADSC);                     // начать преобразование
    while (ADCSRA & (1 << ADSC));              // ждать окончания
    return ADC;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * */  


