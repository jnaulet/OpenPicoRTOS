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
ifeq "$(wildcard nbproject/Makefile-local-Config3.mk)" "nbproject/Makefile-local-Config3.mk"
include nbproject/Makefile-local-Config3.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=Config3
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/attiny817-xplained.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/attiny817-xplained.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
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
SOURCEFILES_QUOTED_IF_SPACED=../../../arch/avr/avrxmega3/picoRTOS_port.c ../../../arch/avr/avrxmega3/picoRTOS_portasm.S ../main.c ../attiny817-xplained.c ../../../drivers/adc/adc-attiny1x.c ../../../drivers/mux/mux-attiny1x.c ../../../drivers/pwm/pwm-attiny1x_tca.c ../../../drivers/clock/clock-attiny1x.c ../../../drivers/gpio/gpio-attiny1x.c ../../../drivers/spi/spi-attiny1x.c ../../../drivers/uart/uart-attiny1x_usart.c ../../../drivers/wd/wd-attiny1x.c ../../../picoRTOS.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/95298038/picoRTOS_port.o ${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1472/attiny817-xplained.o ${OBJECTDIR}/_ext/1462846893/adc-attiny1x.o ${OBJECTDIR}/_ext/1462858973/mux-attiny1x.o ${OBJECTDIR}/_ext/1462861907/pwm-attiny1x_tca.o ${OBJECTDIR}/_ext/1343658459/clock-attiny1x.o ${OBJECTDIR}/_ext/1896195998/gpio-attiny1x.o ${OBJECTDIR}/_ext/1462864569/spi-attiny1x.o ${OBJECTDIR}/_ext/1895793055/uart-attiny1x_usart.o ${OBJECTDIR}/_ext/229905376/wd-attiny1x.o ${OBJECTDIR}/_ext/2124829536/picoRTOS.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/95298038/picoRTOS_port.o.d ${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o.d ${OBJECTDIR}/_ext/1472/main.o.d ${OBJECTDIR}/_ext/1472/attiny817-xplained.o.d ${OBJECTDIR}/_ext/1462846893/adc-attiny1x.o.d ${OBJECTDIR}/_ext/1462858973/mux-attiny1x.o.d ${OBJECTDIR}/_ext/1462861907/pwm-attiny1x_tca.o.d ${OBJECTDIR}/_ext/1343658459/clock-attiny1x.o.d ${OBJECTDIR}/_ext/1896195998/gpio-attiny1x.o.d ${OBJECTDIR}/_ext/1462864569/spi-attiny1x.o.d ${OBJECTDIR}/_ext/1895793055/uart-attiny1x_usart.o.d ${OBJECTDIR}/_ext/229905376/wd-attiny1x.o.d ${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/95298038/picoRTOS_port.o ${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1472/attiny817-xplained.o ${OBJECTDIR}/_ext/1462846893/adc-attiny1x.o ${OBJECTDIR}/_ext/1462858973/mux-attiny1x.o ${OBJECTDIR}/_ext/1462861907/pwm-attiny1x_tca.o ${OBJECTDIR}/_ext/1343658459/clock-attiny1x.o ${OBJECTDIR}/_ext/1896195998/gpio-attiny1x.o ${OBJECTDIR}/_ext/1462864569/spi-attiny1x.o ${OBJECTDIR}/_ext/1895793055/uart-attiny1x_usart.o ${OBJECTDIR}/_ext/229905376/wd-attiny1x.o ${OBJECTDIR}/_ext/2124829536/picoRTOS.o

# Source Files
SOURCEFILES=../../../arch/avr/avrxmega3/picoRTOS_port.c ../../../arch/avr/avrxmega3/picoRTOS_portasm.S ../main.c ../attiny817-xplained.c ../../../drivers/adc/adc-attiny1x.c ../../../drivers/mux/mux-attiny1x.c ../../../drivers/pwm/pwm-attiny1x_tca.c ../../../drivers/clock/clock-attiny1x.c ../../../drivers/gpio/gpio-attiny1x.c ../../../drivers/spi/spi-attiny1x.c ../../../drivers/uart/uart-attiny1x_usart.c ../../../drivers/wd/wd-attiny1x.c ../../../picoRTOS.c



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
	${MAKE}  -f nbproject/Makefile-Config3.mk ${DISTDIR}/attiny817-xplained.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=ATtiny817
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/95298038/picoRTOS_port.o: ../../../arch/avr/avrxmega3/picoRTOS_port.c  .generated_files/flags/Config3/c5e9d80459108393e4b4a14105aa3270d6ccc222 .generated_files/flags/Config3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/95298038" 
	@${RM} ${OBJECTDIR}/_ext/95298038/picoRTOS_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/95298038/picoRTOS_port.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_ADC -DDEMO_PWM -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config3=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/95298038/picoRTOS_port.o.d" -MT "${OBJECTDIR}/_ext/95298038/picoRTOS_port.o.d" -MT ${OBJECTDIR}/_ext/95298038/picoRTOS_port.o -o ${OBJECTDIR}/_ext/95298038/picoRTOS_port.o ../../../arch/avr/avrxmega3/picoRTOS_port.c 
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  .generated_files/flags/Config3/a63f1cd3425d50d5c42b0696906fed61b1c24bd3 .generated_files/flags/Config3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_ADC -DDEMO_PWM -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config3=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1472/main.o.d" -MT "${OBJECTDIR}/_ext/1472/main.o.d" -MT ${OBJECTDIR}/_ext/1472/main.o -o ${OBJECTDIR}/_ext/1472/main.o ../main.c 
	
${OBJECTDIR}/_ext/1472/attiny817-xplained.o: ../attiny817-xplained.c  .generated_files/flags/Config3/c46dea343117a040f5bfadce4c084ab66f1df67b .generated_files/flags/Config3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/attiny817-xplained.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/attiny817-xplained.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_ADC -DDEMO_PWM -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config3=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1472/attiny817-xplained.o.d" -MT "${OBJECTDIR}/_ext/1472/attiny817-xplained.o.d" -MT ${OBJECTDIR}/_ext/1472/attiny817-xplained.o -o ${OBJECTDIR}/_ext/1472/attiny817-xplained.o ../attiny817-xplained.c 
	
${OBJECTDIR}/_ext/1462846893/adc-attiny1x.o: ../../../drivers/adc/adc-attiny1x.c  .generated_files/flags/Config3/7147df9978689b250f2609b686055944375a2f78 .generated_files/flags/Config3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462846893" 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-attiny1x.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-attiny1x.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_ADC -DDEMO_PWM -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config3=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1462846893/adc-attiny1x.o.d" -MT "${OBJECTDIR}/_ext/1462846893/adc-attiny1x.o.d" -MT ${OBJECTDIR}/_ext/1462846893/adc-attiny1x.o -o ${OBJECTDIR}/_ext/1462846893/adc-attiny1x.o ../../../drivers/adc/adc-attiny1x.c 
	
${OBJECTDIR}/_ext/1462858973/mux-attiny1x.o: ../../../drivers/mux/mux-attiny1x.c  .generated_files/flags/Config3/da26f5160d2c7098ea95d56021fb92be44445fce .generated_files/flags/Config3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462858973" 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-attiny1x.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-attiny1x.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_ADC -DDEMO_PWM -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config3=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1462858973/mux-attiny1x.o.d" -MT "${OBJECTDIR}/_ext/1462858973/mux-attiny1x.o.d" -MT ${OBJECTDIR}/_ext/1462858973/mux-attiny1x.o -o ${OBJECTDIR}/_ext/1462858973/mux-attiny1x.o ../../../drivers/mux/mux-attiny1x.c 
	
${OBJECTDIR}/_ext/1462861907/pwm-attiny1x_tca.o: ../../../drivers/pwm/pwm-attiny1x_tca.c  .generated_files/flags/Config3/d36ebfa33ec8551b5fa32f4ea61e91c968cd2737 .generated_files/flags/Config3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462861907" 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-attiny1x_tca.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-attiny1x_tca.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_ADC -DDEMO_PWM -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config3=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1462861907/pwm-attiny1x_tca.o.d" -MT "${OBJECTDIR}/_ext/1462861907/pwm-attiny1x_tca.o.d" -MT ${OBJECTDIR}/_ext/1462861907/pwm-attiny1x_tca.o -o ${OBJECTDIR}/_ext/1462861907/pwm-attiny1x_tca.o ../../../drivers/pwm/pwm-attiny1x_tca.c 
	
${OBJECTDIR}/_ext/1343658459/clock-attiny1x.o: ../../../drivers/clock/clock-attiny1x.c  .generated_files/flags/Config3/ce1c841a727aeff880d6a223f90df691c270f88c .generated_files/flags/Config3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1343658459" 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-attiny1x.o.d 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-attiny1x.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_ADC -DDEMO_PWM -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config3=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1343658459/clock-attiny1x.o.d" -MT "${OBJECTDIR}/_ext/1343658459/clock-attiny1x.o.d" -MT ${OBJECTDIR}/_ext/1343658459/clock-attiny1x.o -o ${OBJECTDIR}/_ext/1343658459/clock-attiny1x.o ../../../drivers/clock/clock-attiny1x.c 
	
${OBJECTDIR}/_ext/1896195998/gpio-attiny1x.o: ../../../drivers/gpio/gpio-attiny1x.c  .generated_files/flags/Config3/1dcf7d83ac4aa45773ea569cea1b4ab5fa605bfc .generated_files/flags/Config3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1896195998" 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-attiny1x.o.d 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-attiny1x.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_ADC -DDEMO_PWM -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config3=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1896195998/gpio-attiny1x.o.d" -MT "${OBJECTDIR}/_ext/1896195998/gpio-attiny1x.o.d" -MT ${OBJECTDIR}/_ext/1896195998/gpio-attiny1x.o -o ${OBJECTDIR}/_ext/1896195998/gpio-attiny1x.o ../../../drivers/gpio/gpio-attiny1x.c 
	
${OBJECTDIR}/_ext/1462864569/spi-attiny1x.o: ../../../drivers/spi/spi-attiny1x.c  .generated_files/flags/Config3/702e3f5ebfc1d99b15c98124e38c99be4ad111c6 .generated_files/flags/Config3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462864569" 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-attiny1x.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-attiny1x.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_ADC -DDEMO_PWM -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config3=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1462864569/spi-attiny1x.o.d" -MT "${OBJECTDIR}/_ext/1462864569/spi-attiny1x.o.d" -MT ${OBJECTDIR}/_ext/1462864569/spi-attiny1x.o -o ${OBJECTDIR}/_ext/1462864569/spi-attiny1x.o ../../../drivers/spi/spi-attiny1x.c 
	
${OBJECTDIR}/_ext/1895793055/uart-attiny1x_usart.o: ../../../drivers/uart/uart-attiny1x_usart.c  .generated_files/flags/Config3/90681e4d4d35b3ed83207c3aa04a1efe4c73859 .generated_files/flags/Config3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1895793055" 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-attiny1x_usart.o.d 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-attiny1x_usart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_ADC -DDEMO_PWM -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config3=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1895793055/uart-attiny1x_usart.o.d" -MT "${OBJECTDIR}/_ext/1895793055/uart-attiny1x_usart.o.d" -MT ${OBJECTDIR}/_ext/1895793055/uart-attiny1x_usart.o -o ${OBJECTDIR}/_ext/1895793055/uart-attiny1x_usart.o ../../../drivers/uart/uart-attiny1x_usart.c 
	
${OBJECTDIR}/_ext/229905376/wd-attiny1x.o: ../../../drivers/wd/wd-attiny1x.c  .generated_files/flags/Config3/9e306e75841ad9fd819962a4b5687c0fa3727d03 .generated_files/flags/Config3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/229905376" 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-attiny1x.o.d 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-attiny1x.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_ADC -DDEMO_PWM -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config3=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/229905376/wd-attiny1x.o.d" -MT "${OBJECTDIR}/_ext/229905376/wd-attiny1x.o.d" -MT ${OBJECTDIR}/_ext/229905376/wd-attiny1x.o -o ${OBJECTDIR}/_ext/229905376/wd-attiny1x.o ../../../drivers/wd/wd-attiny1x.c 
	
${OBJECTDIR}/_ext/2124829536/picoRTOS.o: ../../../picoRTOS.c  .generated_files/flags/Config3/c5b9e039bc88f0bf2083b2f3f97f264685a5a7a7 .generated_files/flags/Config3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2124829536" 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_ADC -DDEMO_PWM -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config3=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d" -MT "${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d" -MT ${OBJECTDIR}/_ext/2124829536/picoRTOS.o -o ${OBJECTDIR}/_ext/2124829536/picoRTOS.o ../../../picoRTOS.c 
	
else
${OBJECTDIR}/_ext/95298038/picoRTOS_port.o: ../../../arch/avr/avrxmega3/picoRTOS_port.c  .generated_files/flags/Config3/3e80c2d0a73bb64b2c85df38c0481d4c6192a54e .generated_files/flags/Config3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/95298038" 
	@${RM} ${OBJECTDIR}/_ext/95298038/picoRTOS_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/95298038/picoRTOS_port.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_ADC -DDEMO_PWM -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config3=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/95298038/picoRTOS_port.o.d" -MT "${OBJECTDIR}/_ext/95298038/picoRTOS_port.o.d" -MT ${OBJECTDIR}/_ext/95298038/picoRTOS_port.o -o ${OBJECTDIR}/_ext/95298038/picoRTOS_port.o ../../../arch/avr/avrxmega3/picoRTOS_port.c 
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  .generated_files/flags/Config3/cfb44ecaab55ad0e8f889814fa47f79e1f0867db .generated_files/flags/Config3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_ADC -DDEMO_PWM -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config3=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1472/main.o.d" -MT "${OBJECTDIR}/_ext/1472/main.o.d" -MT ${OBJECTDIR}/_ext/1472/main.o -o ${OBJECTDIR}/_ext/1472/main.o ../main.c 
	
${OBJECTDIR}/_ext/1472/attiny817-xplained.o: ../attiny817-xplained.c  .generated_files/flags/Config3/8106f6aff69b9baf5a057bf990fd89e4591e3ebd .generated_files/flags/Config3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/attiny817-xplained.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/attiny817-xplained.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_ADC -DDEMO_PWM -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config3=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1472/attiny817-xplained.o.d" -MT "${OBJECTDIR}/_ext/1472/attiny817-xplained.o.d" -MT ${OBJECTDIR}/_ext/1472/attiny817-xplained.o -o ${OBJECTDIR}/_ext/1472/attiny817-xplained.o ../attiny817-xplained.c 
	
${OBJECTDIR}/_ext/1462846893/adc-attiny1x.o: ../../../drivers/adc/adc-attiny1x.c  .generated_files/flags/Config3/f69a5588719fd60bbb594398214998e4cfac4bba .generated_files/flags/Config3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462846893" 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-attiny1x.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-attiny1x.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_ADC -DDEMO_PWM -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config3=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1462846893/adc-attiny1x.o.d" -MT "${OBJECTDIR}/_ext/1462846893/adc-attiny1x.o.d" -MT ${OBJECTDIR}/_ext/1462846893/adc-attiny1x.o -o ${OBJECTDIR}/_ext/1462846893/adc-attiny1x.o ../../../drivers/adc/adc-attiny1x.c 
	
${OBJECTDIR}/_ext/1462858973/mux-attiny1x.o: ../../../drivers/mux/mux-attiny1x.c  .generated_files/flags/Config3/640e3e985ceceae125d9d0022aca2a44741f840f .generated_files/flags/Config3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462858973" 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-attiny1x.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-attiny1x.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_ADC -DDEMO_PWM -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config3=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1462858973/mux-attiny1x.o.d" -MT "${OBJECTDIR}/_ext/1462858973/mux-attiny1x.o.d" -MT ${OBJECTDIR}/_ext/1462858973/mux-attiny1x.o -o ${OBJECTDIR}/_ext/1462858973/mux-attiny1x.o ../../../drivers/mux/mux-attiny1x.c 
	
${OBJECTDIR}/_ext/1462861907/pwm-attiny1x_tca.o: ../../../drivers/pwm/pwm-attiny1x_tca.c  .generated_files/flags/Config3/c5b12a959c8f39683678904b080acfdca8772595 .generated_files/flags/Config3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462861907" 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-attiny1x_tca.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-attiny1x_tca.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_ADC -DDEMO_PWM -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config3=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1462861907/pwm-attiny1x_tca.o.d" -MT "${OBJECTDIR}/_ext/1462861907/pwm-attiny1x_tca.o.d" -MT ${OBJECTDIR}/_ext/1462861907/pwm-attiny1x_tca.o -o ${OBJECTDIR}/_ext/1462861907/pwm-attiny1x_tca.o ../../../drivers/pwm/pwm-attiny1x_tca.c 
	
${OBJECTDIR}/_ext/1343658459/clock-attiny1x.o: ../../../drivers/clock/clock-attiny1x.c  .generated_files/flags/Config3/37927e2efbf5d7355ac02d25622029402d5a1e06 .generated_files/flags/Config3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1343658459" 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-attiny1x.o.d 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-attiny1x.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_ADC -DDEMO_PWM -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config3=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1343658459/clock-attiny1x.o.d" -MT "${OBJECTDIR}/_ext/1343658459/clock-attiny1x.o.d" -MT ${OBJECTDIR}/_ext/1343658459/clock-attiny1x.o -o ${OBJECTDIR}/_ext/1343658459/clock-attiny1x.o ../../../drivers/clock/clock-attiny1x.c 
	
${OBJECTDIR}/_ext/1896195998/gpio-attiny1x.o: ../../../drivers/gpio/gpio-attiny1x.c  .generated_files/flags/Config3/dfa33d706af5650a223687a4125eeee37591580e .generated_files/flags/Config3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1896195998" 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-attiny1x.o.d 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-attiny1x.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_ADC -DDEMO_PWM -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config3=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1896195998/gpio-attiny1x.o.d" -MT "${OBJECTDIR}/_ext/1896195998/gpio-attiny1x.o.d" -MT ${OBJECTDIR}/_ext/1896195998/gpio-attiny1x.o -o ${OBJECTDIR}/_ext/1896195998/gpio-attiny1x.o ../../../drivers/gpio/gpio-attiny1x.c 
	
${OBJECTDIR}/_ext/1462864569/spi-attiny1x.o: ../../../drivers/spi/spi-attiny1x.c  .generated_files/flags/Config3/15e17190a5cb6c592eac30ce3e8700bfeb5bd3c4 .generated_files/flags/Config3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462864569" 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-attiny1x.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-attiny1x.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_ADC -DDEMO_PWM -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config3=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1462864569/spi-attiny1x.o.d" -MT "${OBJECTDIR}/_ext/1462864569/spi-attiny1x.o.d" -MT ${OBJECTDIR}/_ext/1462864569/spi-attiny1x.o -o ${OBJECTDIR}/_ext/1462864569/spi-attiny1x.o ../../../drivers/spi/spi-attiny1x.c 
	
${OBJECTDIR}/_ext/1895793055/uart-attiny1x_usart.o: ../../../drivers/uart/uart-attiny1x_usart.c  .generated_files/flags/Config3/97098154bbe52ee938ef8455e92bd84a00ad92f2 .generated_files/flags/Config3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1895793055" 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-attiny1x_usart.o.d 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-attiny1x_usart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_ADC -DDEMO_PWM -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config3=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1895793055/uart-attiny1x_usart.o.d" -MT "${OBJECTDIR}/_ext/1895793055/uart-attiny1x_usart.o.d" -MT ${OBJECTDIR}/_ext/1895793055/uart-attiny1x_usart.o -o ${OBJECTDIR}/_ext/1895793055/uart-attiny1x_usart.o ../../../drivers/uart/uart-attiny1x_usart.c 
	
${OBJECTDIR}/_ext/229905376/wd-attiny1x.o: ../../../drivers/wd/wd-attiny1x.c  .generated_files/flags/Config3/a0d6ea165537229c29b534ad608151a74e727ee .generated_files/flags/Config3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/229905376" 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-attiny1x.o.d 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-attiny1x.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_ADC -DDEMO_PWM -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config3=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/229905376/wd-attiny1x.o.d" -MT "${OBJECTDIR}/_ext/229905376/wd-attiny1x.o.d" -MT ${OBJECTDIR}/_ext/229905376/wd-attiny1x.o -o ${OBJECTDIR}/_ext/229905376/wd-attiny1x.o ../../../drivers/wd/wd-attiny1x.c 
	
${OBJECTDIR}/_ext/2124829536/picoRTOS.o: ../../../picoRTOS.c  .generated_files/flags/Config3/a03ff66b8be2ac3295017bdc581f5ff5c500b592 .generated_files/flags/Config3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2124829536" 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_ADC -DDEMO_PWM -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config3=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d" -MT "${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d" -MT ${OBJECTDIR}/_ext/2124829536/picoRTOS.o -o ${OBJECTDIR}/_ext/2124829536/picoRTOS.o ../../../picoRTOS.c 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o: ../../../arch/avr/avrxmega3/picoRTOS_portasm.S  .generated_files/flags/Config3/221b49ea57837009b78d280d7cb0679d46b728a0 .generated_files/flags/Config3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/95298038" 
	@${RM} ${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o.d 
	@${RM} ${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o 
	${MP_CC} -c $(MP_EXTRA_AS_PRE) -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x assembler-with-cpp -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_ADC -DDEMO_PWM -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config3=$(CND_CONF)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem -Wa,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1   -MD -MP -MF "${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o.d" -MT "${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o.d" -MT ${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o -o ${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o  ../../../arch/avr/avrxmega3/picoRTOS_portasm.S 
	
else
${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o: ../../../arch/avr/avrxmega3/picoRTOS_portasm.S  .generated_files/flags/Config3/4b11dbf9cb504972417c59bfe653e191c40eff84 .generated_files/flags/Config3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/95298038" 
	@${RM} ${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o.d 
	@${RM} ${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o 
	${MP_CC} -c $(MP_EXTRA_AS_PRE) -mcpu=$(MP_PROCESSOR_OPTION)  -x assembler-with-cpp -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_ADC -DDEMO_PWM -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config3=$(CND_CONF)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem -Wa,--defsym=__MPLAB_BUILD=1   -MD -MP -MF "${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o.d" -MT "${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o.d" -MT ${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o -o ${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o  ../../../arch/avr/avrxmega3/picoRTOS_portasm.S 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/attiny817-xplained.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=${DISTDIR}/attiny817-xplained.X.${IMAGE_TYPE}.map  -D__DEBUG=1  -DXPRJ_Config3=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"   -gdwarf-2 -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_ADC -DDEMO_PWM -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     $(COMPARISON_BUILD) -Wl,--memorysummary,${DISTDIR}/memoryfile.xml -o ${DISTDIR}/attiny817-xplained.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  -o ${DISTDIR}/attiny817-xplained.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -Wl,--start-group  -Wl,-lm -Wl,--end-group  -Wl,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1
	@${RM} ${DISTDIR}/attiny817-xplained.X.${IMAGE_TYPE}.hex 
	
else
${DISTDIR}/attiny817-xplained.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=${DISTDIR}/attiny817-xplained.X.${IMAGE_TYPE}.map  -DXPRJ_Config3=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_ADC -DDEMO_PWM -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     $(COMPARISON_BUILD) -Wl,--memorysummary,${DISTDIR}/memoryfile.xml -o ${DISTDIR}/attiny817-xplained.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  -o ${DISTDIR}/attiny817-xplained.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -Wl,--start-group  -Wl,-lm -Wl,--end-group 
	${MP_CC_DIR}/avr-objcopy -O ihex "${DISTDIR}/attiny817-xplained.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "${DISTDIR}/attiny817-xplained.X.${IMAGE_TYPE}.hex"
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
