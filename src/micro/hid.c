#include <avr/io.h>
#include <util/delay.h>

// === Константы для USB ===
#define EP_TYPE_CONTROL 0x00
#define EP_TYPE_INTERRUPT_IN 0xC1
#define HID_ENDPOINT 3
#define HID_BUFFER_SIZE 8

// === Конфигурация Endpoints ===
void usb_init() {
    UHWCON |= (1 << UVREGE);              // Включаем регулятор напряжения USB
    USBCON = (1 << USBE) | (1 << FRZCLK); // Включаем USB контроллер и замораживаем
    PLLCSR = (1 << PINDIV) | (1 << PLLE); // Настраиваем PLL для USB
    while (!(PLLCSR & (1 << PLOCK)));     // Ждём блокировки PLL
    USBCON &= ~(1 << FRZCLK);             // Размораживаем USB
    UDCON = 0;                            // Подключаемся к шине

    // === Настраиваем Endpoint 3 для передачи данных (HID) ===
    UENUM = HID_ENDPOINT;                 // Выбираем Endpoint 3
    UECONX = (1 << EPEN);                 // Включаем его
    UECFG0X = EP_TYPE_INTERRUPT_IN;       // Тип — прерывание, IN
    UECFG1X = (1 << EPSIZE1);             // Размер пакета — 8 байт

    // === Проверяем, что конфигурация прошла ===
    if (!(UESTA0X & (1 << CFGOK))) {
        while (1); // Если не удалось — зависаем
    }
}

void usb_send_keypress(uint8_t key) {
    UENUM = HID_ENDPOINT;
    if (UEINTX & (1 << RWAL)) {
        // === Формируем пакет данных (HID-отчёт) ===
        UEDATX = 0x00; // Модификатор (Ctrl, Shift и т.п.) — пусто
        UEDATX = 0x00; // Зарезервировано
        UEDATX = key;  // Код клавиши (A)
        UEDATX = 0x00;
        UEDATX = 0x00;
        UEDATX = 0x00;
        UEDATX = 0x00;
        UEDATX = 0x00;

        // === Завершаем передачу ===
        UEINTX &= ~(1 << TXINI);
    }
}

int main(void) {
    usb_init();             // Инициализация USB
    DDRB |= (1 << PB0);     // Настраиваем светодиод

    while (1) {
        PORTB ^= (1 << PB0); // Мигание светодиодом
        usb_send_keypress(0x04); // 0x04 — это HID-код для 'a'
        _delay_ms(1000);
    }
}
