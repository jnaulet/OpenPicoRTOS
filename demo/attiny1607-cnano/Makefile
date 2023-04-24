ELF := attiny1607-cnano.elf

CC := cc
CROSS := xc8-
BUILD := build

# picoRTOS
PICORTOS_DIR := $(abspath ../..)
-include $(PICORTOS_DIR)/devices/atmel/attiny1607/Makefile.in

# static anaylyzer
SPLINTFLAGS := -D__ATtiny1607__ -checks +unixlib
CPPCHECKFLAGS += --enable=all --inline-suppr -DNDEBUG
CPPCHECKFLAGS += --suppress=missingIncludeSystem
CPPCHECKFLAGS += --suppress=unusedFunction

INCLUDE += -I.

CFLAGS += -Wall -Wextra -std=c99 $(INCLUDE) $(DEFINE)
CFLAGS += -mcpu=ATtiny1607 -x c -D__ATtiny1607__
CFLAGS += -mdfp="/opt/microchip/mplabx/v6.05/packs/Microchip/ATtiny_DFP/3.0.151/xc8"
CFLAGS += -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums
CFLAGS += -fno-common -funsigned-char -funsigned-bitfields
CFLAGS += -DXPRJ_default=default -gdwarf-3 -mconst-data-in-progmem
CFLAGS += -mno-const-data-in-config-mapped-progmem

SFLAGS += -x assembler-with-cpp $(DEFINE) $(INCLUDE)
SFLAGS += -mdfp="/opt/microchip/mplabx/v6.05/packs/Microchip/ATtiny_DFP/3.0.151/xc8"
SFLAGS += -mcpu=ATtiny1607 -D__ATtiny1607__

LDFLAGS += -mcpu=ATtiny1607 -Wl,-Map=$(ELF).map -DXPRJ_default=default -Wl,--defsym=__MPLAB_BUILD=1
LDFLAGS += -mdfp="/opt/microchip/mplabx/v6.05/packs/Microchip/ATtiny_DFP/3.0.151/xc8"
LDFLAGS += -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums
LDFLAGS += -fno-common -funsigned-char -funsigned-bitfields -Wall -gdwarf-3
LDFLAGS += -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem
LDFLAGS += -Wl,--start-group  -Wl,-lm -Wl,--end-group

# Drivers
INCLUDE += -I$(PICORTOS_DIR)/drivers/include
INCLUDE += -I$(PICORTOS_DIR)/drivers/adc
INCLUDE += -I$(PICORTOS_DIR)/drivers/clock
INCLUDE += -I$(PICORTOS_DIR)/drivers/gpio
INCLUDE += -I$(PICORTOS_DIR)/drivers/mux
INCLUDE += -I$(PICORTOS_DIR)/drivers/pwm
INCLUDE += -I$(PICORTOS_DIR)/drivers/spi
INCLUDE += -I$(PICORTOS_DIR)/drivers/uart
INCLUDE += -I$(PICORTOS_DIR)/drivers/wd

# Drivers
SRC_C += $(PICORTOS_DIR)/drivers/adc/adc-tinyavr.c
SRC_C += $(PICORTOS_DIR)/drivers/clock/clock-tinyavr.c
SRC_C += $(PICORTOS_DIR)/drivers/gpio/gpio-tinyavr.c
SRC_C += $(PICORTOS_DIR)/drivers/mux/mux-tinyavr.c
SRC_C += $(PICORTOS_DIR)/drivers/pwm/pwm-tinyavr_tca.c
SRC_C += $(PICORTOS_DIR)/drivers/spi/spi-tinyavr.c
SRC_C += $(PICORTOS_DIR)/drivers/uart/uart-tinyavr_usart.c
SRC_C += $(PICORTOS_DIR)/drivers/wd/wd-tinyavr.c

# demo
SRC_C += attiny1607-cnano.c
SRC_C += main.c

OBJ := $(SRC_C:%.c=$(BUILD)/%.o)
OBJ += $(SRC_S:%.S=$(BUILD)/%.o)

all: $(ELF)

$(ELF): $(OBJ)
	-mkdir -p $(@D)
	$(CROSS)$(CC) $^ $(LDFLAGS) -o $@

$(BUILD)/%.o: %.c
	-mkdir -p $(@D)
	$(CROSS)$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: %.S
	-mkdir -p $(@D)
	$(CROSS)$(CC) $(SFLAGS) -c $< -o $@

splint:
	splint $(SPLINTFLAGS) $(DEFINE) $(INCLUDE) $(SRC_C)

cppcheck:
	cppcheck $(CPPCHECKFLAGS) $(DEFINE) $(INCLUDE) $(SRC_C)

clean:
	-rm -rf $(BUILD)
	-rm -f *.elf

.PHONY: clean splint
