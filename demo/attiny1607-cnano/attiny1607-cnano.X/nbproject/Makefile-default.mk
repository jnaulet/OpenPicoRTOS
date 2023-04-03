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
${OBJECTDIR}/_ext/1472/attiny1607-cnano.o: ../attiny1607-cnano.c  .generated_files/flags/default/1fc79f810bba00b1e0d92295e77b4fe8cf95a638 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/attiny1607-cnano.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/attiny1607-cnano.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1472/attiny1607-cnano.o.d" -MT "${OBJECTDIR}/_ext/1472/attiny1607-cnano.o.d" -MT ${OBJECTDIR}/_ext/1472/attiny1607-cnano.o -o ${OBJECTDIR}/_ext/1472/attiny1607-cnano.o ../attiny1607-cnano.c 
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  .generated_files/flags/default/c445f9d730a69b826ddfb3cbd930cd448d22dac1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1472/main.o.d" -MT "${OBJECTDIR}/_ext/1472/main.o.d" -MT ${OBJECTDIR}/_ext/1472/main.o -o ${OBJECTDIR}/_ext/1472/main.o ../main.c 
	
${OBJECTDIR}/_ext/95298038/picoRTOS_port.o: ../../../arch/avr/avrxmega3/picoRTOS_port.c  .generated_files/flags/default/f1775ca4b8585864fbfc3f0857fe0255aaf331ec .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/95298038" 
	@${RM} ${OBJECTDIR}/_ext/95298038/picoRTOS_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/95298038/picoRTOS_port.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/95298038/picoRTOS_port.o.d" -MT "${OBJECTDIR}/_ext/95298038/picoRTOS_port.o.d" -MT ${OBJECTDIR}/_ext/95298038/picoRTOS_port.o -o ${OBJECTDIR}/_ext/95298038/picoRTOS_port.o ../../../arch/avr/avrxmega3/picoRTOS_port.c 
	
${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o: ../../../drivers/adc/adc-tinyavr.c  .generated_files/flags/default/73fa6a60390d874dfff94a0a6fe9d18b0595fd2b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462846893" 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o.d" -MT "${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o.d" -MT ${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o -o ${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o ../../../drivers/adc/adc-tinyavr.c 
	
${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o: ../../../drivers/clock/clock-tinyavr.c  .generated_files/flags/default/80f6c8037313c02cc7027bef07b3c47a495054bc .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1343658459" 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o.d" -MT "${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o.d" -MT ${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o -o ${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o ../../../drivers/clock/clock-tinyavr.c 
	
${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o: ../../../drivers/gpio/gpio-tinyavr.c  .generated_files/flags/default/dcce6200e958daecc90a1ac7660d2386628d2fc1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1896195998" 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o.d" -MT "${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o.d" -MT ${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o -o ${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o ../../../drivers/gpio/gpio-tinyavr.c 
	
${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o: ../../../drivers/mux/mux-tinyavr.c  .generated_files/flags/default/a2b5ec3429b952c99ed91d71fcc118de1fe2cfd5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462858973" 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o.d" -MT "${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o.d" -MT ${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o -o ${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o ../../../drivers/mux/mux-tinyavr.c 
	
${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o: ../../../drivers/pwm/pwm-tinyavr_tca.c  .generated_files/flags/default/c15aa31732ee0e557ceaa603b618d8800ec6dc85 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462861907" 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o.d" -MT "${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o.d" -MT ${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o -o ${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o ../../../drivers/pwm/pwm-tinyavr_tca.c 
	
${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o: ../../../drivers/spi/spi-tinyavr.c  .generated_files/flags/default/ba1753cef238f6945c4b8b8bfa1af04bb7532643 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462864569" 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o.d" -MT "${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o.d" -MT ${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o -o ${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o ../../../drivers/spi/spi-tinyavr.c 
	
${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o: ../../../drivers/uart/uart-tinyavr_usart.c  .generated_files/flags/default/c6d15c610a97bff6d649580bb8555c6db9a2f3aa .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1895793055" 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o.d 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o.d" -MT "${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o.d" -MT ${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o -o ${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o ../../../drivers/uart/uart-tinyavr_usart.c 
	
${OBJECTDIR}/_ext/229905376/wd-tinyavr.o: ../../../drivers/wd/wd-tinyavr.c  .generated_files/flags/default/f42543f2ae2e188c723d2605f8907d68d98310b4 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/229905376" 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-tinyavr.o.d 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-tinyavr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/229905376/wd-tinyavr.o.d" -MT "${OBJECTDIR}/_ext/229905376/wd-tinyavr.o.d" -MT ${OBJECTDIR}/_ext/229905376/wd-tinyavr.o -o ${OBJECTDIR}/_ext/229905376/wd-tinyavr.o ../../../drivers/wd/wd-tinyavr.c 
	
${OBJECTDIR}/_ext/2124829536/picoRTOS.o: ../../../picoRTOS.c  .generated_files/flags/default/de7a534068646965af54d82f6997a0aede959b2d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2124829536" 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d" -MT "${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d" -MT ${OBJECTDIR}/_ext/2124829536/picoRTOS.o -o ${OBJECTDIR}/_ext/2124829536/picoRTOS.o ../../../picoRTOS.c 
	
else
${OBJECTDIR}/_ext/1472/attiny1607-cnano.o: ../attiny1607-cnano.c  .generated_files/flags/default/99f4c1c5f83299bb758e4f06279d8d55398cc943 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/attiny1607-cnano.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/attiny1607-cnano.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1472/attiny1607-cnano.o.d" -MT "${OBJECTDIR}/_ext/1472/attiny1607-cnano.o.d" -MT ${OBJECTDIR}/_ext/1472/attiny1607-cnano.o -o ${OBJECTDIR}/_ext/1472/attiny1607-cnano.o ../attiny1607-cnano.c 
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  .generated_files/flags/default/750e8aa4590da02a9703e6baf5a11fe21e98c518 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1472/main.o.d" -MT "${OBJECTDIR}/_ext/1472/main.o.d" -MT ${OBJECTDIR}/_ext/1472/main.o -o ${OBJECTDIR}/_ext/1472/main.o ../main.c 
	
${OBJECTDIR}/_ext/95298038/picoRTOS_port.o: ../../../arch/avr/avrxmega3/picoRTOS_port.c  .generated_files/flags/default/9be774f19d05950ed1bb91370da4ed69d81d5d84 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/95298038" 
	@${RM} ${OBJECTDIR}/_ext/95298038/picoRTOS_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/95298038/picoRTOS_port.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/95298038/picoRTOS_port.o.d" -MT "${OBJECTDIR}/_ext/95298038/picoRTOS_port.o.d" -MT ${OBJECTDIR}/_ext/95298038/picoRTOS_port.o -o ${OBJECTDIR}/_ext/95298038/picoRTOS_port.o ../../../arch/avr/avrxmega3/picoRTOS_port.c 
	
${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o: ../../../drivers/adc/adc-tinyavr.c  .generated_files/flags/default/7ce0f2ff0bd7adc412bc040f651f2b399745c0bc .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462846893" 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o.d" -MT "${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o.d" -MT ${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o -o ${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o ../../../drivers/adc/adc-tinyavr.c 
	
${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o: ../../../drivers/clock/clock-tinyavr.c  .generated_files/flags/default/5c0e4cebb33ff9903a69825ca6eec91879c6f64 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1343658459" 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o.d" -MT "${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o.d" -MT ${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o -o ${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o ../../../drivers/clock/clock-tinyavr.c 
	
${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o: ../../../drivers/gpio/gpio-tinyavr.c  .generated_files/flags/default/dad1cbc52f67919fa5fe9b2df5aed610ca56096 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1896195998" 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o.d" -MT "${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o.d" -MT ${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o -o ${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o ../../../drivers/gpio/gpio-tinyavr.c 
	
${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o: ../../../drivers/mux/mux-tinyavr.c  .generated_files/flags/default/e220994d069f90b63c6ad74ba9559f5ce4679449 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462858973" 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o.d" -MT "${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o.d" -MT ${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o -o ${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o ../../../drivers/mux/mux-tinyavr.c 
	
${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o: ../../../drivers/pwm/pwm-tinyavr_tca.c  .generated_files/flags/default/ce796c481e22884647770737e9ddd5541b46f5a0 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462861907" 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o.d" -MT "${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o.d" -MT ${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o -o ${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o ../../../drivers/pwm/pwm-tinyavr_tca.c 
	
${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o: ../../../drivers/spi/spi-tinyavr.c  .generated_files/flags/default/b3bec99e440af49aa8095679e0b5df3e0c70dffc .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462864569" 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o.d" -MT "${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o.d" -MT ${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o -o ${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o ../../../drivers/spi/spi-tinyavr.c 
	
${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o: ../../../drivers/uart/uart-tinyavr_usart.c  .generated_files/flags/default/8d42c1a7ad477063c4e5f0eca270ce637f1d65f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1895793055" 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o.d 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o.d" -MT "${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o.d" -MT ${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o -o ${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o ../../../drivers/uart/uart-tinyavr_usart.c 
	
${OBJECTDIR}/_ext/229905376/wd-tinyavr.o: ../../../drivers/wd/wd-tinyavr.c  .generated_files/flags/default/d172397b811820d68af1dffc34381403a7a0bcf .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/229905376" 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-tinyavr.o.d 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-tinyavr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/229905376/wd-tinyavr.o.d" -MT "${OBJECTDIR}/_ext/229905376/wd-tinyavr.o.d" -MT ${OBJECTDIR}/_ext/229905376/wd-tinyavr.o -o ${OBJECTDIR}/_ext/229905376/wd-tinyavr.o ../../../drivers/wd/wd-tinyavr.c 
	
${OBJECTDIR}/_ext/2124829536/picoRTOS.o: ../../../picoRTOS.c  .generated_files/flags/default/55ec219244c51753ed61fc454be7b49f0e8468fd .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
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
${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o: ../../../arch/avr/avrxmega3/picoRTOS_portasm.S  .generated_files/flags/default/a3a85be40ef3b2f0be76a081dc8a907a35c15e78 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/95298038" 
	@${RM} ${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o.d 
	@${RM} ${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o 
	${MP_CC} -c $(MP_EXTRA_AS_PRE) -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x assembler-with-cpp -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny1607" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_default=$(CND_CONF)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem -Wa,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1   -MD -MP -MF "${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o.d" -MT "${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o.d" -MT ${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o -o ${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o  ../../../arch/avr/avrxmega3/picoRTOS_portasm.S 
	
else
${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o: ../../../arch/avr/avrxmega3/picoRTOS_portasm.S  .generated_files/flags/default/dc3143d31854ceff7203b1eb3bc84af465ad3ab7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
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
