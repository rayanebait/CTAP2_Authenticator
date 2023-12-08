    # simple AVR Makefile
#
# written by michael cousins (http://github.com/mcous)
# released to the public domain

# Makefile
#
# targets:
#   all:    compiles the source code
#   test:   tests the isp connection to the mcu
#   flash:  writes compiled hex file to the mcu's flash memory
#   fuse:   writes the fuse bytes to the MCU
#   disasm: disassembles the code for debugging
#   clean:  removes all .hex, .elf, and .o files in the source code and library directories

# parameters (change this stuff accordingly)
# project name
#Mettre PROG=exerciceN pour compiler l'exercice N
ifndef PROG
	PROG = main
endif

ifndef ADD
	ADD=
endif
# avr mcu
MCU = atmega328p
# mcu clock frequency
CLK = 16000000
# avr programmer (and port if necessary)
# e.g. PRG = usbtiny -or- PRG = arduino -P /dev/tty.usbmodem411
PRG = arduino -P /dev/ttyACM0
# fuse values for avr: low, high, and extended
# these values are from an Arduino Uno (ATMega328P)
# see http://www.engbedded.com/fusecalc/ for other MCUs and options
LFU = 0xFF
HFU = 0xDE
EFU = 0x05
# program source files (not including external libraries)
EXT = ./include/utils ./include/micro-ecc ./micro-ecc
LIB = ./include/lib

EXT_SRC = ./src/utils ./micro-ecc
LIB_SRC = ./src/lib
SRC = $(EXT_SRC) $(LIB_SRC)

# where to look for external libraries (consisting of .c/.cpp files and .h files)
# e.g. EXT = ../../EyeToSee ../../YouSART


#################################################################################################
# \/ stuff nobody needs to worry about until such time that worrying about it is appropriate \/ #
#################################################################################################

# include path
INCLUDE_EXT := $(foreach dir, $(EXT), -I$(dir))
INCLUDE_LIB := $(foreach dir, $(LIB), -I$(dir))
INCLUDE      = $(INCLUDE_EXT) $(INCLUDE_LIB)
# c flags
# DEPFLAGS=-MP -MMD

CFLAGS   = -Wall -Os -DF_CPU=$(CLK) -mmcu=$(MCU) $(INCLUDE)
# any aditional flags for c++
CPPFLAGS =

# executables
AVRDUDE = avrdude -c $(PRG) -p $(MCU)
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE    = avr-size --format=avr --mcu=$(MCU)
CC      = avr-gcc

# generate list of objects
EXTC       := $(foreach dir, $(EXT_SRC), $(wildcard $(dir)/*.c))
LIBC       := $(foreach dir, $(LIB_SRC), $(wildcard $(dir)/*.c))
CFILES     := $(foreach dir, $(SRC), $(wildcard $(dir)/*.c))
CPPFILES    = $(filter %.cpp, $(SRC))
EXTCPP     := $(foreach dir, $(EXT), $(wildcard $(dir)/*.cpp))

EXTOBJ      = $(EXTC:.c=.o) 
LIBOBJ      = $(LIBC:.c=.o) 
OBJ         = $(CFILES:.c=.o) $(CPPFILES:.cpp=.o)

# DEPFILES = $(patsubst %.c,%.d,$(CFILES))

# user targets
# compile all files
all: $(PROG).hex

# test programmer connectivity
test:
	$(AVRDUDE) -v

# flash program to mcu
flash: all
	$(AVRDUDE) -U flash:w:$(PROG).hex:i

# write fuses to mcu
fuse:
	$(AVRDUDE) -U lfuse:w:$(LFU):m -U hfuse:w:$(HFU):m -U efuse:w:$(EFU):m

# generate disassembly files for debugging
disasm: $(PROG).elf
	$(OBJDUMP) -d $(PROG).elf

# remove compiled files
clean:
	rm -f $(PROG).o $(PROG).elf $(PROG).hex
	rm -f $(OBJ)

# elf file
$(PROG).elf: $(PROG).o $(OBJ)
	$(CC) $(CFLAGS) -o $(PROG).elf $(PROG).o $(OBJ)

# hex file
$(PROG).hex: $(PROG).elf
	rm -f $(PROG).hex
	$(OBJCOPY) -j .text -j .data -O ihex $(PROG).elf $(PROG).hex
	$(SIZE) $(PROG).elf

# other targets
# objects from c files

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

# objects from c++ files
.cpp.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@
