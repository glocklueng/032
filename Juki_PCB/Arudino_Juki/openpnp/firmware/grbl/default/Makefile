###############################################################################
# Makefile for the project grbl
###############################################################################

## General Flags
PROJECT = grbl
MCU = atmega2560
TARGET = grbl.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99 -ffunction-sections            -DF_CPU=16000000UL -O2 -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=grbl.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Library Directories
LIBDIRS = -L"C:\WinAVR-20100110\avr\lib\avr6" 

## Libraries
LIBS = -lm 

## Objects that must be built in order to link
OBJECTS = coolant_control.o eeprom.o gcode.o main.o motion_control.o planner.o serial_protocol.o settings.o spindle_control.o stepper.o wiring_serial.o atc_control.o head_control.o panel_control.o vacuum_control.o tape_knock.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) grbl.hex grbl.eep grbl.lss size

## Compile
coolant_control.o: ../coolant_control.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

eeprom.o: ../eeprom.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

gcode.o: ../gcode.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

main.o: ../main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

motion_control.o: ../motion_control.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

planner.o: ../planner.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

serial_protocol.o: ../serial_protocol.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

settings.o: ../settings.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

spindle_control.o: ../spindle_control.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

stepper.o: ../stepper.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

wiring_serial.o: ../wiring_serial.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

atc_control.o: ../atc_control.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

head_control.o: ../head_control.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

panel_control.o: ../panel_control.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

vacuum_control.o: ../vacuum_control.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

tape_knock.o: ../tape_knock.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) grbl.elf dep/* grbl.hex grbl.eep grbl.lss grbl.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

