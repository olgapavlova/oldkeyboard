# Кросс-компиляция кода для микроконтроллера ATmega32u4
# Использует билдер фреймворка LUFA

# Настройки железа
MCU          = atmega32u4
ARCH         = AVR8
BOARD        = USBKEY
F_CPU        = 16000000
F_USB        = $(F_CPU)
OPTIMIZATION = s
DEVICE 		 = /dev/ttyACM0
DRIVER 		 = avrdude

# Настройки компиляции
TARGET       = main
SRC          = $(TARGET).c Keyboard.c Descriptors.c $(LUFA_SRC_USB) $(LUFA_SRC_USBCLASS) adc.c uart.c kb.c
LUFA_PATH    = /home/op/tools/lufa/LUFA
CC_FLAGS     = -DUSE_LUFA_CONFIG_HEADER -IConfig/ -I.
LD_FLAGS     =

# Цель по умолчанию
all:

# Загрузить прошивку в микроконтроллер
flash: $(TARGET).hex
	stty -F $(DEVICE) 1200
	sleep 2
	$(DRIVER) -v -p $(MCU) -c avr109 -P $(DEVICE) -b 57600 -U flash:w:$<

# Мониторить UART-выход микроконтроллера (обычно логи)
uart:
	minicom -D /dev/ttyUSB0 -b 9600

# Мониторить входящие сигналы клавиатуры
input:
	sudo evtest



# Специфические цели билдера фреймворка LUFA

# Include LUFA-specific DMBS extension modules
DMBS_LUFA_PATH ?= $(LUFA_PATH)/Build/LUFA
include $(DMBS_LUFA_PATH)/lufa-sources.mk
include $(DMBS_LUFA_PATH)/lufa-gcc.mk

# Include common DMBS build system modules
DMBS_PATH      ?= $(LUFA_PATH)/Build/DMBS/DMBS
include $(DMBS_PATH)/core.mk
include $(DMBS_PATH)/cppcheck.mk
include $(DMBS_PATH)/doxygen.mk
include $(DMBS_PATH)/dfu.mk
include $(DMBS_PATH)/gcc.mk
include $(DMBS_PATH)/hid.mk
include $(DMBS_PATH)/avrdude.mk
include $(DMBS_PATH)/atprogram.mk
