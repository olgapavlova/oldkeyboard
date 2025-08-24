#ifndef ADC_H
#define ADC_H

#include <avr/io.h>
#include <util/delay.h>

// Инициировать АЦП
void adc_init(void);

// Прочесть значение на пине
uint16_t adc_read(uint8_t pin);

#endif
