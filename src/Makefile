
MCU = atmega640
TARGET = hexapod
OBJFILES = main.o

COMMON = -mmcu=$(MCU)

CC = avr-gcc
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99  -DF_CPU=16000000UL -O0 -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += 
LDFLAGS = $(COMMON)

## Include Directories
INCLUDES = -I"./webbotavrclib" 

## Library Directories
LIBDIRS = -L"./webbotavrclib" 

LIBS =  -lWebbot-ATMega640

all: build

build: $(TARGET).bin
	avr-objcopy -O ihex -R .eeprom $(TARGET).bin $(TARGET).hex
	avr-size --target=ihex $(TARGET).hex

$(TARGET).bin: $(OBJFILES)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJFILES)  $(LIBDIRS) $(LIBS) -o $(TARGET).bin

%.o : %.c
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(TARGET).bin $(TARGET).hex

program:
	./bootloader -d /def/ttyUSB0 -b 115200 -p $(TARGET).hex

