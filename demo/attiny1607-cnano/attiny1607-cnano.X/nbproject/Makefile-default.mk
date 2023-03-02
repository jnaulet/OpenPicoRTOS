#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/attiny1607-cnano.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/attiny1607-cnano.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../attiny1607-cnano.c ../main.c ../../../arch/avr/avrxmega3/picoRTOS_port.c ../../../arch/avr/avrxmega3/picoRTOS_portasm.S ../../../drivers/adc/adc-tinyavr.c ../../../drivers/clock/clock-tinyavr.c ../../../drivers/gpio/gpio-tinyavr.c ../../../drivers/mux/mux-tinyavr.c ../../../drivers/pwm/pwm-tinyavr_tca.c ../../../drivers/spi/spi-tinyavr.c ../../../drivers/uart/uart-tinyavr_usart.c ../../../drivers/wd/wd-tinyavr.c ../../../picoRTOS.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1472/attiny1607-cnano.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/95298038/picoRTOS_port.o ${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o ${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o ${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o ${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o ${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o ${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o ${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o ${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o ${OBJECTDIR}/_ext/229905376/wd-tinyavr.o ${OBJECTDIR}/_ext/2124829536/picoRTOS.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1472/attiny1607-cnano.o.d ${OBJECTDIR}/_ext/1472/main.o.d ${OBJECTDIR}/_ext/95298038/picoRTOS_port.o.d ${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o.d ${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o.d ${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o.d ${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o.d ${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o.d ${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o.d ${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o.d ${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o.d ${OBJECTDIR}/_ext/229905376/wd-tinyavr.o.d ${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1472/attiny1607-cnano.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/95298038/picoRTOS_port.o ${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o ${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o ${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o ${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o ${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o ${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o ${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o ${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o ${OBJECTDIR}/_ext/229905376/wd-tinyavr.o ${OBJECTDIR}/_ext/2124829536/picoRTOS.o

# Source Files
SOURCEFILES=../attiny1607-cnano.c ../main.c ../../../arch/avr/avrxmega3/picoRTOS_port.c ../../../arch/avr/avrxmega3/picoRTOS_portasm.S ../../../drivers/adc/adc-tinyavr.c ../../../drivers/clock/clock-tinyavr.c ../../../drivers/gpio/gpio-tinyavr.c ../../../drivers/mux/mux-tinyavr.c ../../../drivers/pwm/pwm-tinyavr_tca.c ../../../drivers/spi/spi-tinyavr.c ../../../drivers/uart/uart-tinyavr_usart.c ../../../drivers/wd/wd-tinyavr.c ../../../picoRTOS.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/attiny1607-cnano.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=ATtiny1607
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/attiny1607-cnano.o: ../attiny1607-cnano.c  .generated_files/flags/default/99748fef32c66ef89d860ac2ac76e109e0b979e8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/attiny1607-cnano.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/attiny1607-cnano.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1472/attiny1607-cnano.o.d" -MT "${OBJECTDIR}/_ext/1472/attiny1607-cnano.o.d" -MT ${OBJECTDIR}/_ext/1472/attiny1607-cnano.o -o ${OBJECTDIR}/_ext/1472/attiny1607-cnano.o ../attiny1607-cnano.c 
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  .generated_files/flags/default/17ac9d525ef667b41658750fc330cac268754c73 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1472/main.o.d" -MT "${OBJECTDIR}/_ext/1472/main.o.d" -MT ${OBJECTDIR}/_ext/1472/main.o -o ${OBJECTDIR}/_ext/1472/main.o ../main.c 
	
${OBJECTDIR}/_ext/95298038/picoRTOS_port.o: ../../../arch/avr/avrxmega3/picoRTOS_port.c  .generated_files/flags/default/aea8560812b0417b132b8025a1f49ae17583f54b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/95298038" 
	@${RM} ${OBJECTDIR}/_ext/95298038/picoRTOS_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/95298038/picoRTOS_port.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/95298038/picoRTOS_port.o.d" -MT "${OBJECTDIR}/_ext/95298038/picoRTOS_port.o.d" -MT ${OBJECTDIR}/_ext/95298038/picoRTOS_port.o -o ${OBJECTDIR}/_ext/95298038/picoRTOS_port.o ../../../arch/avr/avrxmega3/picoRTOS_port.c 
	
${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o: ../../../drivers/adc/adc-tinyavr.c  .generated_files/flags/default/9aed2cbb1f812dd822e8c6ac24e54c8753ef29c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462846893" 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o.d" -MT "${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o.d" -MT ${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o -o ${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o ../../../drivers/adc/adc-tinyavr.c 
	
${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o: ../../../drivers/clock/clock-tinyavr.c  .generated_files/flags/default/2431d9c445ea54c76b532944221ae80f1be7e5b8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1343658459" 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o.d" -MT "${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o.d" -MT ${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o -o ${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o ../../../drivers/clock/clock-tinyavr.c 
	
${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o: ../../../drivers/gpio/gpio-tinyavr.c  .generated_files/flags/default/27ea91408f746d48c0a5860f4582dcddbdb0b0c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1896195998" 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o.d" -MT "${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o.d" -MT ${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o -o ${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o ../../../drivers/gpio/gpio-tinyavr.c 
	
${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o: ../../../drivers/mux/mux-tinyavr.c  .generated_files/flags/default/3fb9b604e4595b1dbb14aee8d358eb7c0889b35a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462858973" 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o.d" -MT "${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o.d" -MT ${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o -o ${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o ../../../drivers/mux/mux-tinyavr.c 
	
${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o: ../../../drivers/pwm/pwm-tinyavr_tca.c  .generated_files/flags/default/3a147311054a5fd5ce425c0da04e5e60bbf6724f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462861907" 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o.d" -MT "${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o.d" -MT ${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o -o ${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o ../../../drivers/pwm/pwm-tinyavr_tca.c 
	
${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o: ../../../drivers/spi/spi-tinyavr.c  .generated_files/flags/default/bac32d314b8f03a8edf7488f7d2b15d325e2cfd8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462864569" 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o.d" -MT "${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o.d" -MT ${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o -o ${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o ../../../drivers/spi/spi-tinyavr.c 
	
${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o: ../../../drivers/uart/uart-tinyavr_usart.c  .generated_files/flags/default/41a6f69d2477868b4db6d44b0c5c2efa5c05f875 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1895793055" 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o.d 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o.d" -MT "${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o.d" -MT ${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o -o ${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o ../../../drivers/uart/uart-tinyavr_usart.c 
	
${OBJECTDIR}/_ext/229905376/wd-tinyavr.o: ../../../drivers/wd/wd-tinyavr.c  .generated_files/flags/default/e5c3e7ca57898a334cecb546214f6212b03e916b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/229905376" 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-tinyavr.o.d 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-tinyavr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/229905376/wd-tinyavr.o.d" -MT "${OBJECTDIR}/_ext/229905376/wd-tinyavr.o.d" -MT ${OBJECTDIR}/_ext/229905376/wd-tinyavr.o -o ${OBJECTDIR}/_ext/229905376/wd-tinyavr.o ../../../drivers/wd/wd-tinyavr.c 
	
${OBJECTDIR}/_ext/2124829536/picoRTOS.o: ../../../picoRTOS.c  .generated_files/flags/default/3610a16e1c3f1b854a60190c36ee191799b05c21 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2124829536" 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d" -MT "${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d" -MT ${OBJECTDIR}/_ext/2124829536/picoRTOS.o -o ${OBJECTDIR}/_ext/2124829536/picoRTOS.o ../../../picoRTOS.c 
	
else
${OBJECTDIR}/_ext/1472/attiny1607-cnano.o: ../attiny1607-cnano.c  .generated_files/flags/default/e22bc664689161c69c43791246651338234161b0 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/attiny1607-cnano.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/attiny1607-cnano.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1472/attiny1607-cnano.o.d" -MT "${OBJECTDIR}/_ext/1472/attiny1607-cnano.o.d" -MT ${OBJECTDIR}/_ext/1472/attiny1607-cnano.o -o ${OBJECTDIR}/_ext/1472/attiny1607-cnano.o ../attiny1607-cnano.c 
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  .generated_files/flags/default/4768e9bf65ad17d4271ab5530155377957267519 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1472/main.o.d" -MT "${OBJECTDIR}/_ext/1472/main.o.d" -MT ${OBJECTDIR}/_ext/1472/main.o -o ${OBJECTDIR}/_ext/1472/main.o ../main.c 
	
${OBJECTDIR}/_ext/95298038/picoRTOS_port.o: ../../../arch/avr/avrxmega3/picoRTOS_port.c  .generated_files/flags/default/5f4a9c02fbf99e13c7334539fed06a8d88acecd4 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/95298038" 
	@${RM} ${OBJECTDIR}/_ext/95298038/picoRTOS_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/95298038/picoRTOS_port.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/95298038/picoRTOS_port.o.d" -MT "${OBJECTDIR}/_ext/95298038/picoRTOS_port.o.d" -MT ${OBJECTDIR}/_ext/95298038/picoRTOS_port.o -o ${OBJECTDIR}/_ext/95298038/picoRTOS_port.o ../../../arch/avr/avrxmega3/picoRTOS_port.c 
	
${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o: ../../../drivers/adc/adc-tinyavr.c  .generated_files/flags/default/40c2bf287802263d514b480c8b3b2e4c59e91426 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462846893" 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o.d" -MT "${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o.d" -MT ${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o -o ${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o ../../../drivers/adc/adc-tinyavr.c 
	
${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o: ../../../drivers/clock/clock-tinyavr.c  .generated_files/flags/default/cbbfee9d5188b11b8bbf6510d6e83bda21bb5277 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1343658459" 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o.d" -MT "${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o.d" -MT ${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o -o ${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o ../../../drivers/clock/clock-tinyavr.c 
	
${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o: ../../../drivers/gpio/gpio-tinyavr.c  .generated_files/flags/default/cb94955e602f25ea5dd65b4450d6dcc63c97a9ca .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1896195998" 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o.d" -MT "${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o.d" -MT ${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o -o ${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o ../../../drivers/gpio/gpio-tinyavr.c 
	
${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o: ../../../drivers/mux/mux-tinyavr.c  .generated_files/flags/default/6ef4e7c0cab5953d405c56a360fdc6d83c6c15b3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462858973" 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o.d" -MT "${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o.d" -MT ${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o -o ${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o ../../../drivers/mux/mux-tinyavr.c 
	
${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o: ../../../drivers/pwm/pwm-tinyavr_tca.c  .generated_files/flags/default/fd9afd2b7f777a07afef65d24397a41c3138f33b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462861907" 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o.d" -MT "${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o.d" -MT ${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o -o ${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o ../../../drivers/pwm/pwm-tinyavr_tca.c 
	
${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o: ../../../drivers/spi/spi-tinyavr.c  .generated_files/flags/default/d1adc1f4a806e869a28bbfb21aaab6fcd61c5d1a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462864569" 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o.d" -MT "${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o.d" -MT ${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o -o ${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o ../../../drivers/spi/spi-tinyavr.c 
	
${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o: ../../../drivers/uart/uart-tinyavr_usart.c  .generated_files/flags/default/211a7a03f7378db4798631bbff5e41b6a2f1293b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1895793055" 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o.d 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o.d" -MT "${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o.d" -MT ${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o -o ${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o ../../../drivers/uart/uart-tinyavr_usart.c 
	
${OBJECTDIR}/_ext/229905376/wd-tinyavr.o: ../../../drivers/wd/wd-tinyavr.c  .generated_files/flags/default/1cce3c40c4a98c00a8813301e4a364c12e2b447f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/229905376" 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-tinyavr.o.d 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-tinyavr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/229905376/wd-tinyavr.o.d" -MT "${OBJECTDIR}/_ext/229905376/wd-tinyavr.o.d" -MT ${OBJECTDIR}/_ext/229905376/wd-tinyavr.o -o ${OBJECTDIR}/_ext/229905376/wd-tinyavr.o ../../../drivers/wd/wd-tinyavr.c 
	
${OBJECTDIR}/_ext/2124829536/picoRTOS.o: ../../../picoRTOS.c  .generated_files/flags/default/b036f350c298e852f0dbb1b272c3c51d7e49de4a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2124829536" 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d" -MT "${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d" -MT ${OBJECTDIR}/_ext/2124829536/picoRTOS.o -o ${OBJECTDIR}/_ext/2124829536/picoRTOS.o ../../../picoRTOS.c 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o: ../../../arch/avr/avrxmega3/picoRTOS_portasm.S  .generated_files/flags/default/4124cf6927cd7e8c1212e24b7590fcc14c19fd30 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/95298038" 
	@${RM} ${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o.d 
	@${RM} ${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o 
	${MP_CC} -c $(MP_EXTRA_AS_PRE) -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x assembler-with-cpp -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem -Wa,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1   -MD -MP -MF "${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o.d" -MT "${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o.d" -MT ${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o -o ${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o  ../../../arch/avr/avrxmega3/picoRTOS_portasm.S 
	
else
${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o: ../../../arch/avr/avrxmega3/picoRTOS_portasm.S  .generated_files/flags/default/ed03ec6af6c77b700432dc97ac6b7facc0e8f335 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/95298038" 
	@${RM} ${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o.d 
	@${RM} ${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o 
	${MP_CC} -c $(MP_EXTRA_AS_PRE) -mcpu=$(MP_PROCESSOR_OPTION)  -x assembler-with-cpp -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem -Wa,--defsym=__MPLAB_BUILD=1   -MD -MP -MF "${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o.d" -MT "${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o.d" -MT ${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o -o ${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o  ../../../arch/avr/avrxmega3/picoRTOS_portasm.S 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/attiny1607-cnano.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=${DISTDIR}/attiny1607-cnano.X.${IMAGE_TYPE}.map  -D__DEBUG=1  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"   -gdwarf-2 -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     $(COMPARISON_BUILD) -Wl,--memorysummary,${DISTDIR}/memoryfile.xml -o ${DISTDIR}/attiny1607-cnano.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  -o ${DISTDIR}/attiny1607-cnano.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -Wl,--start-group  -Wl,-lm -Wl,--end-group  -Wl,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1
	@${RM} ${DISTDIR}/attiny1607-cnano.X.${IMAGE_TYPE}.hex 
	
else
${DISTDIR}/attiny1607-cnano.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=${DISTDIR}/attiny1607-cnano.X.${IMAGE_TYPE}.map  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     $(COMPARISON_BUILD) -Wl,--memorysummary,${DISTDIR}/memoryfile.xml -o ${DISTDIR}/attiny1607-cnano.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  -o ${DISTDIR}/attiny1607-cnano.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -Wl,--start-group  -Wl,-lm -Wl,--end-group 
	${MP_CC_DIR}/avr-objcopy -O ihex "${DISTDIR}/attiny1607-cnano.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "${DISTDIR}/attiny1607-cnano.X.${IMAGE_TYPE}.hex"
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
