ELF := attiny817-xplained.elf

CC := cc
CROSS := /opt/microchip/xc8/v2.40/bin/xc8-
BUILD_DIR := build

# picoRTOS
PICORTOS_DIR := $(abspath ../..)
-include $(PICORTOS_DIR)/devices/atmel/attiny817/Makefile.in

# static anaylyzer
SPLINTFLAGS := -D__ATtiny817__ -checks +unixlib

INCLUDE += -I.

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

# COMPILE
CFLAGS := -mcpu=ATtiny817 -D__DEBUG=1 -g -DDEBUG -gdwarf-2 -x c
CFLAGS += -mdfp="/opt/microchip/mplabx/v6.05/packs/Microchip/ATtiny_DFP/3.0.151/xc8"
CFLAGS += -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common
CFLAGS += -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=default -gdwarf-3
CFLAGS += -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem -MD -MP
CFLAGS += $(INCLUDE) $(DEFINE)

SFLAGS := -mcpu=ATtiny817 -D__DEBUG=1 -g -DDEBUG -gdwarf-2
SFLAGS += -mdfp="/opt/microchip/mplabx/v6.05/packs/Microchip/ATtiny_DFP/3.0.151/xc8"
SFLAGS += -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common
SFLAGS += -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=default -gdwarf-3
SFLAGS += -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem -MD -MP
SFLAGS += $(INCLUDE) $(DEFINE)

# LINK
LDFLAGS := -mcpu=ATtiny817 -D__DEBUG=1 -DXPRJ_default=default -Wl,--defsym=__MPLAB_BUILD=1
LDFLAGS += -mdfp="/opt/microchip/mplabx/v6.05/packs/Microchip/ATtiny_DFP/3.0.151/xc8" -gdwarf-2
LDFLAGS += -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common
LDFLAGS += -funsigned-char -funsigned-bitfields -Wall -gdwarf-3 -mconst-data-in-progmem
LDFLAGS += -mno-const-data-in-config-mapped-progmem
LDFLAGS += -Wl,--start-group -Wl,-lm -Wl,--end-group -Wl,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1

# Drivers
SRC_C += $(PICORTOS_DIR)/drivers/adc/adc-attiny1x.c
SRC_C += $(PICORTOS_DIR)/drivers/clock/clock-attiny1x.c
SRC_C += $(PICORTOS_DIR)/drivers/gpio/gpio-attiny1x.c
SRC_C += $(PICORTOS_DIR)/drivers/mux/mux-attiny1x.c
SRC_C += $(PICORTOS_DIR)/drivers/pwm/pwm-attiny1x_tca.c
SRC_C += $(PICORTOS_DIR)/drivers/spi/spi-attiny1x.c
SRC_C += $(PICORTOS_DIR)/drivers/uart/uart-attiny1x_usart.c
SRC_C += $(PICORTOS_DIR)/drivers/wd/wd-attiny1x.c

# demo
SRC_C += attiny817-xplained.c
SRC_C += main.c

OBJ := $(SRC_C:%.c=$(BUILD_DIR)/%.o)
OBJ += $(SRC_S:%.S=$(BUILD_DIR)/%.o)

# CONFIGS
CONFIG1 := -DDEMO_LED -DDEMO_CONSOLE -DDEMO_WDT
CONFIG2 := -DDEMO_LED -DDEMO_CONSOLE -DDEMO_SPI
CONFIG3 := -DDEMO_LED -DDEMO_ADC -DDEMO_PWM

config1: CFLAGS += $(CONFIG1)
config1: $(ELF)

config2: CFLAGS += $(CONFIG2)
config2: $(ELF)

config3: CFLAGS += $(CONFIG3)
config3: $(ELF)

$(ELF): $(OBJ)
	-mkdir -p $(@D)
	$(CROSS)$(CC) $^ $(LDFLAGS) -o $@

$(BUILD_DIR)/%.o: %.c
	-mkdir -p $(@D)
	$(CROSS)$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.S
	-mkdir -p $(@D)
	$(CROSS)$(CC) $(SFLAGS) -c $< -o $@

splint: DEFINE += -DDEMO_TEST_SPI -DDEMO_TEST_PWM
splint:
	splint $(SPLINTFLAGS) $(DEFINE) $(INCLUDE) $(SRC_C)

clean:
	-rm -rf $(BUILD_DIR)
	-rm -f $(ELF)

.PHONY: clean splint
