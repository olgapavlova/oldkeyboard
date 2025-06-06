MCU = atmega32u4
F_CPU = 16000000UL
CC = avr-gcc
BAUD = 9600
OBJCOPY = avr-objcopy
CFLAGS = -std=gnu99 -Wall -Os -DF_CPU=$(F_CPU) -mmcu=$(MCU)
LDFLAGS = -mmcu=$(MCU) -Wl,--no-gc-sections

TARGET = proto/uex

all: clean $(TARGET).hex flash

%.elf: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

%.hex: %.elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

flash: $(TARGET).hex
	stty -F /dev/ttyACM0 1200
	sleep 2
	avrdude -v -p $(MCU) -c avr109 -P /dev/ttyACM0 -b 57600 -U flash:w:$<

monitor:
	minicom -D /dev/ttyUSB0 -b 9600

clean:
	@rm -f $(TARGET) $(TARGET).o $(TARGET).elf $(TARGET).hex
