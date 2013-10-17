################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS :=  \
../cc30000


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../cc30000/cc3000_common.c \
../cc30000/evnt_handler.c \
../cc30000/hci.c \
../cc30000/netapp.c \
../cc30000/nvmem.c \
../cc30000/security.c \
../cc30000/socket.c \
../cc30000/spi.c \
../cc30000/wlan.c \
../leds.c \
../TLC5947.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
cc30000/cc3000_common.o \
cc30000/evnt_handler.o \
cc30000/hci.o \
cc30000/netapp.o \
cc30000/nvmem.o \
cc30000/security.o \
cc30000/socket.o \
cc30000/spi.o \
cc30000/wlan.o \
leds.o \
TLC5947.o

OBJS_AS_ARGS +=  \
cc30000/cc3000_common.o \
cc30000/evnt_handler.o \
cc30000/hci.o \
cc30000/netapp.o \
cc30000/nvmem.o \
cc30000/security.o \
cc30000/socket.o \
cc30000/spi.o \
cc30000/wlan.o \
leds.o \
TLC5947.o

C_DEPS +=  \
cc30000/cc3000_common.d \
cc30000/evnt_handler.d \
cc30000/hci.d \
cc30000/netapp.d \
cc30000/nvmem.d \
cc30000/security.d \
cc30000/socket.d \
cc30000/spi.d \
cc30000/wlan.d \
leds.d \
TLC5947.d

C_DEPS_AS_ARGS +=  \
cc30000/cc3000_common.d \
cc30000/evnt_handler.d \
cc30000/hci.d \
cc30000/netapp.d \
cc30000/nvmem.d \
cc30000/security.d \
cc30000/socket.d \
cc30000/spi.d \
cc30000/wlan.d \
leds.d \
TLC5947.d

OUTPUT_FILE_PATH +=leds.elf

OUTPUT_FILE_PATH_AS_ARGS +=leds.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

# AVR32/GNU C Compiler























cc30000/%.o: ../cc30000/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 3.4.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.1002\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -funsigned-char -funsigned-bitfields -DF_CPU=8000000UL -DNDEBUG  -I"cc3000"  -O3 -fpack-struct -fshort-enums -Wall -mmcu=atxmega32a4u -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<"
	@echo Finished building: $<
	

./%.o: .././%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 3.4.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.1002\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -funsigned-char -funsigned-bitfields -DF_CPU=8000000UL -DNDEBUG  -I"cc3000"  -O3 -fpack-struct -fshort-enums -Wall -mmcu=atxmega32a4u -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<"
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker : 3.4.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.1002\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="leds.map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mmcu=atxmega32a4u 
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.1002\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature  "leds.elf" "leds.hex"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.1002\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "leds.elf" "leds.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.1002\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "leds.elf" > "leds.lss"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.1002\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature  "leds.elf" "leds.srec"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.1002\avr8-gnu-toolchain\bin\avr-size.exe" "leds.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "leds.elf" "leds.a" "leds.hex" "leds.lss" "leds.eep" "leds.map" "leds.srec"
	