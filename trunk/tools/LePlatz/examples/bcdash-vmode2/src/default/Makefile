###############################################################################
# Makefile for the project B.C. Dash
###############################################################################

## General Flags
PROJECT = platz_demo
GAME= platz_demo
MCU = atmega644
TARGET = $(GAME).elf
CC = avr-gcc
INFO=../gameinfo.properties
## CC Flags: -fverbose-asm -S -fno-inline-small-functions

## Escape spaces in mixer path (due to custom sounds.inc)
EMPTY:=
SPACE:= $(EMPTY) $(EMPTY)
SPACE_ESC:= \\$(SPACE)
MIX_PATH:= $(realpath ../data/sounds.inc)
MIX_PATH_ESC:= $(subst $(SPACE),$(SPACE_ESC),$(MIX_PATH))

## Kernel settings
KERNEL_DIR = ../kernel
KERNEL_OPTIONS  = -DVIDEO_MODE=2 -DINTRO_LOGO=0 -DSCREEN_SECTIONS_COUNT=3 -DSOUND_CHANNEL_4_ENABLE=1 -DSOUND_CHANNEL_2_ENABLE=1 -DSOUND_CHANNEL_3_ENABLE=1
KERNEL_OPTIONS  += -DMIXER_WAVES=\"$(MIX_PATH_ESC)\" -DVRAM_TILES_V=26

## Platz settings
	## MAX_MOVING_PLATFORMS and MAX_ANIMATED_BGS are per slice. Set to 0 (or delete) to disable.
	## VIEWPORT_SLACK should be small enough so as to not allow your player to wander offscreen.
	## SS_OFFSET_Y is the offset in tiles from the top of the screen that your Platz screenSection begins (for VIDEO_MODE=2).
	## PLATZ_SLIM can save 1k or more of flash at the cost of minor graphical compromises. Set to zero to disable (enabled by default).
PLATZ_OPTIONS = -DVIEWPORT_SLACK=0 -DMAX_MOVING_PLATFORMS=3 -DMAX_ANIMATED_BGS=5 -DMUTABLE_BGS -DMP_HACK -DSS_OFFSET_Y=3 -DPLATZ_SLIM=1

## B.C. Dash settings
BC_DASH_OPTIONS = -DSLICE_COUNT=51 -DSTART_Y=40

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99 -DF_CPU=28636360UL -Os -fsigned-char -ffunction-sections 
##CFLAGS += -Wall -gstabs -std=gnu99 -DF_CPU=28636360UL -Os -fsigned-char		## Emu debug setting
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 
CFLAGS += $(KERNEL_OPTIONS)
CFLAGS += $(PLATZ_OPTIONS)
CFLAGS += $(BC_DASH_OPTIONS)

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS += -Wl,-Map=$(GAME).map 
LDFLAGS += -Wl,-gc-sections 


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Objects that must be built in order to link
OBJECTS = uzeboxVideoEngineCore.o uzeboxCore.o uzeboxSoundEngine.o uzeboxSoundEngineCore.o uzeboxVideoEngine.o platz.o $(GAME).o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Include Directories
INCLUDES = -I"$(KERNEL_DIR)" 

## Build
all: $(TARGET) $(GAME).hex $(GAME).eep $(GAME).lss $(GAME).uze size

## Compile Kernel files
uzeboxVideoEngineCore.o: $(KERNEL_DIR)/uzeboxVideoEngineCore.s
	$(CC) $(INCLUDES) $(ASMFLAGS) -c  $<

uzeboxSoundEngineCore.o: $(KERNEL_DIR)/uzeboxSoundEngineCore.s
	$(CC) $(INCLUDES) $(ASMFLAGS) -c  $<

uzeboxCore.o: $(KERNEL_DIR)/uzeboxCore.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

uzeboxSoundEngine.o: $(KERNEL_DIR)/uzeboxSoundEngine.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

uzeboxVideoEngine.o: $(KERNEL_DIR)/uzeboxVideoEngine.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

## Compile game sources
platz.o: ../platz.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

$(GAME).o: ../$(GAME).c
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

%.uze: $(TARGET)
	@echo
	-packrom $(GAME).hex $@ $(INFO)

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) $(GAME).* dep/*

## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

