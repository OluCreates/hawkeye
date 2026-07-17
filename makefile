MCU = atmega328p
F_CPU = 16000000UL
CC = avr-gcc
CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -Wall
OBJCOPY = avr-objcopy
AVRDUDE = avrdude
PORT = COM7
BAUD = 115200

TARGET = hawkeye
SRCS = main.c i2c.c uart.c tasks.c queue.c list.c timers.c event_groups.c stream_buffer.c heap_3.c hooks.c port.c fusion.c
$(TARGET).elf: $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET).elf $(SRCS)

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex $(TARGET).elf $(TARGET).hex

flash: $(TARGET).hex
	$(AVRDUDE) -p $(MCU) -c arduino -P $(PORT) -b $(BAUD) -U flash:w:$(TARGET).hex

clean:
	del $(TARGET).elf $(TARGET).hex