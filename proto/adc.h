#ifndef ADC_H
#define ADC_H

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

void adc_init(void);
uint16_t adc_read(uint8_t pin);

#endif
