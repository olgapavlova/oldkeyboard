#include "main.h"

// Определены в Keyboard.c
extern uint8_t * PrevKeyboardHIDReportBuffer;
extern USB_ClassInfo_HID_Device_t Keyboard_HID_Interface;

// Точка входа в программу
int main(void)
{
  uart_init();
  adc_init();
	SetupHardware();

	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
	GlobalInterruptEnable();

	for (;;)
	{
		HID_Device_USBTask(&Keyboard_HID_Interface);
		USB_USBTask();
	}
}
