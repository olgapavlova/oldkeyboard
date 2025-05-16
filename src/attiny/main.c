#include <avr/io.h>
#include <util/delay.h>
#include "usbdrv/usbdrv.h"

static uchar reportBuffer[8];  // Буфер для HID-отчёта

// === HID Report Descriptor ===
const PROGMEM char usbHidReportDescriptor[35] = {
    0x05, 0x01,    // Usage Page (Generic Desktop),
    0x09, 0x06,    // Usage (Keyboard),
    0xA1, 0x01,    // Collection (Application),
    0x05, 0x07,    // Usage Page (Key Codes),
    0x19, 0xE0,    // Usage Minimum (224),
    0x29, 0xE7,    // Usage Maximum (231),
    0x15, 0x00,    // Logical Minimum (0),
    0x25, 0x01,    // Logical Maximum (1),
    0x75, 0x01,    // Report Size (1),
    0x95, 0x08,    // Report Count (8),
    0x81, 0x02,    // Input (Data, Variable, Absolute), ; Modifier byte
    0x95, 0x01,    // Report Count (1),
    0x75, 0x08,    // Report Size (8),
    0x81, 0x01,    // Input (Constant),                 ; Reserved byte
    0x95, 0x06,    // Report Count (6),
    0x75, 0x08,    // Report Size (8),
    0x15, 0x00,    // Logical Minimum (0),
    0x25, 0x65,    // Logical Maximum (101),
    0x05, 0x07,    // Usage Page (Key Codes),
    0x19, 0x00,    // Usage Minimum (0),
    0x29, 0x65,    // Usage Maximum (101),
    0x81, 0x00,    // Input (Data, Array),
    0xC0           // End Collection
};

// === Настройка USB при подключении ===
usbMsgLen_t usbFunctionSetup(uchar data[8]) {
    usbMsgPtr = reportBuffer;
    return sizeof(reportBuffer);
}

// === Отправка HID-отчёта с нажатием клавиши ===
void sendKeypress(uint8_t keycode) {
    reportBuffer[0] = 0x00;    // Модификатор (Ctrl, Shift и т.п.)
    reportBuffer[1] = 0x00;    // Зарезервировано
    reportBuffer[2] = keycode; // Код клавиши (0x04 — это 'a')
    reportBuffer[3] = 0x00;
    reportBuffer[4] = 0x00;
    reportBuffer[5] = 0x00;
    reportBuffer[6] = 0x00;
    reportBuffer[7] = 0x00;

    usbSetInterrupt(reportBuffer, sizeof(reportBuffer));
    _delay_ms(100);

    // === Отпускание клавиши ===
    reportBuffer[2] = 0x00;
    usbSetInterrupt(reportBuffer, sizeof(reportBuffer));
    _delay_ms(100);
}

// === Основной цикл ===
int main(void) {
    usbInit();
    usbDeviceDisconnect();
    _delay_ms(250);
    usbDeviceConnect();

    while (1) {
        usbPoll();             // Проверяем состояние USB
        sendKeypress(0x04);    // 0x04 — HID-код для буквы 'a'
        _delay_ms(1000);       // Каждую секунду отправляем
    }
}
