#include "kb.h"

// Модуль числа
int _abs(int x) {
  return (x > 0) ? x : -1 * x;
} 

// Преобразовать данные с АЦП в код нажатия
int kb_key_code(int val) {
  int result = -1;
  for(int i = 0; i < CODES_Q; i++) {
    if (_abs(kb_codes[i] - val) < DELTA) {
      result = i;
      break;
    }
  }
  return result;
}

// Получить значение клавиши по коду нажатия и группе
int kb_key_value(int group, int code) {
  return kb_layout[group * CODES_Q + code];
}

// Определить, какая клавиша нажата
int kb_key_pressed() {
  int result = -1;
  for(int i = 0; i < GROUPS_Q; i++) {
    uint16_t value = kb_key_code(adc_read(7 - i));
    if (value > -1) {
      result = kb_key_value(i, value);
    }
  }
  return result;
}

// Отправить по UART отчёт о состоянии одного пина
void kb_pin_report(uint8_t pin) {
    char buffer[32];
    uint16_t value = kb_key_code(adc_read(pin));
    //uint16_t value = adc_read(pin);
    snprintf(buffer, sizeof(buffer), "A%d: %4d\t", (7 - pin), value);
    uart_send_string(buffer);
}

// Отправить по UART отчёт о состоянии клавиатуры
void kb_report(void) {
    for (uint8_t pin = 7; pin >= 4; pin--) {
      kb_pin_report(pin);
    }
    uart_send_string("\r\n");
}
