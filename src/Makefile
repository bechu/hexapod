
MCU = atmega640
TARGET = test
OBJFILES = main.o

CC = avr-gcc
CFLAGS = -mmcu=$(MCU)
LDFLAGS = 

all: build

build: $(TARGET).bin
	avr-objcopy -O ihex -R .eeprom $(TARGET).bin $(TARGET).hex
	avr-size --target=ihex $(TARGET).hex

$(TARGET).bin: $(OBJFILES)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJFILES) -o $(TARGET).bin

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f *.o $(TARGET).bin $(TARGET).hex

program:
	./bootloader -d /def/ttyUSB0 -b 115200 -p $(TARGET).hex

