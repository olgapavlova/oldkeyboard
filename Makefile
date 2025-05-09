MCU = atmega32u4
F_CPU = 16000000UL
CC = avr-gcc
BAUD = 9600
OBJCOPY = avr-objcopy
CFLAGS = -Wall -Os -DF_CPU=$(F_CPU) -mmcu=$(MCU)

TARGET = src/micro/main

all: $(TARGET).hex

%.elf: %.c
	$(CC) $(CFLAGS) -o $@ $<

%.hex: %.elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

flash:
	stty -F /dev/ttyACM0 1200
	sleep 2
	avrdude -v -p atmega32u4 -c avr109 -P /dev/ttyACM0 -b 57600 -D -U flash:w:$(TARGET).hex

clean:
	@rm -f $(TARGET).elf $(TARGET).hex
