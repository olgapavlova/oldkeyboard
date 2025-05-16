# Название проекта
PROJECT = main

# Настройки микроконтроллера
MCU = attiny85
F_CPU = 16000000UL
CC = avr-gcc
OBJCOPY = avr-objcopy
CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -I./usbdrv

# Список исходников
SRC = main.c usbdrv/usbdrv.c usbdrv/usbdrvasm.S
OBJ = $(SRC:.c=.o)

# Имя конечного файла
TARGET = $(PROJECT).hex

# Задачи
all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(CC) $(CFLAGS) -x assembler-with-cpp -c $< -o $@

$(PROJECT).elf: $(OBJ)
	$(CC) -mmcu=$(MCU) -o $@ $(OBJ)

$(TARGET): $(PROJECT).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

# Команда для прошивки
flash: $(TARGET)
	micronucleus --run $(TARGET)

# Очистка всех временных файлов
clean:
	rm -f usbdrv/*.o *.o *.elf *.hex
