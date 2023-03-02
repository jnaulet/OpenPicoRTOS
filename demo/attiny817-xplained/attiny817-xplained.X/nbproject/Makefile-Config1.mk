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
ifeq "$(wildcard nbproject/Makefile-local-Config1.mk)" "nbproject/Makefile-local-Config1.mk"
include nbproject/Makefile-local-Config1.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=Config1
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
SOURCEFILES_QUOTED_IF_SPACED=../../../arch/avr/avrxmega3/picoRTOS_port.c ../../../arch/avr/avrxmega3/picoRTOS_portasm.S ../main.c ../attiny817-xplained.c ../../../drivers/adc/adc-tinyavr.c ../../../drivers/mux/mux-tinyavr.c ../../../drivers/pwm/pwm-tinyavr_tca.c ../../../drivers/clock/clock-tinyavr.c ../../../drivers/gpio/gpio-tinyavr.c ../../../drivers/spi/spi-tinyavr.c ../../../drivers/uart/uart-tinyavr_usart.c ../../../drivers/wd/wd-tinyavr.c ../../../picoRTOS.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/95298038/picoRTOS_port.o ${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1472/attiny817-xplained.o ${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o ${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o ${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o ${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o ${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o ${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o ${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o ${OBJECTDIR}/_ext/229905376/wd-tinyavr.o ${OBJECTDIR}/_ext/2124829536/picoRTOS.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/95298038/picoRTOS_port.o.d ${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o.d ${OBJECTDIR}/_ext/1472/main.o.d ${OBJECTDIR}/_ext/1472/attiny817-xplained.o.d ${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o.d ${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o.d ${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o.d ${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o.d ${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o.d ${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o.d ${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o.d ${OBJECTDIR}/_ext/229905376/wd-tinyavr.o.d ${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/95298038/picoRTOS_port.o ${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1472/attiny817-xplained.o ${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o ${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o ${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o ${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o ${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o ${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o ${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o ${OBJECTDIR}/_ext/229905376/wd-tinyavr.o ${OBJECTDIR}/_ext/2124829536/picoRTOS.o

# Source Files
SOURCEFILES=../../../arch/avr/avrxmega3/picoRTOS_port.c ../../../arch/avr/avrxmega3/picoRTOS_portasm.S ../main.c ../attiny817-xplained.c ../../../drivers/adc/adc-tinyavr.c ../../../drivers/mux/mux-tinyavr.c ../../../drivers/pwm/pwm-tinyavr_tca.c ../../../drivers/clock/clock-tinyavr.c ../../../drivers/gpio/gpio-tinyavr.c ../../../drivers/spi/spi-tinyavr.c ../../../drivers/uart/uart-tinyavr_usart.c ../../../drivers/wd/wd-tinyavr.c ../../../picoRTOS.c



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
	${MAKE}  -f nbproject/Makefile-Config1.mk ${DISTDIR}/attiny817-xplained.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=ATtiny817
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/95298038/picoRTOS_port.o: ../../../arch/avr/avrxmega3/picoRTOS_port.c  .generated_files/flags/Config1/c9b36e14084865061bbac56682a5b8faa55f85e6 .generated_files/flags/Config1/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/95298038" 
	@${RM} ${OBJECTDIR}/_ext/95298038/picoRTOS_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/95298038/picoRTOS_port.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_CONSOLE -DDEMO_WDT -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config1=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/95298038/picoRTOS_port.o.d" -MT "${OBJECTDIR}/_ext/95298038/picoRTOS_port.o.d" -MT ${OBJECTDIR}/_ext/95298038/picoRTOS_port.o -o ${OBJECTDIR}/_ext/95298038/picoRTOS_port.o ../../../arch/avr/avrxmega3/picoRTOS_port.c 
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  .generated_files/flags/Config1/7d28c7cac5c1f1d254a5f78bae94299998dee5a5 .generated_files/flags/Config1/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_CONSOLE -DDEMO_WDT -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config1=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1472/main.o.d" -MT "${OBJECTDIR}/_ext/1472/main.o.d" -MT ${OBJECTDIR}/_ext/1472/main.o -o ${OBJECTDIR}/_ext/1472/main.o ../main.c 
	
${OBJECTDIR}/_ext/1472/attiny817-xplained.o: ../attiny817-xplained.c  .generated_files/flags/Config1/4221beec72b2d8796c5cebfbad88e99d45db21d .generated_files/flags/Config1/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/attiny817-xplained.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/attiny817-xplained.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_CONSOLE -DDEMO_WDT -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config1=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1472/attiny817-xplained.o.d" -MT "${OBJECTDIR}/_ext/1472/attiny817-xplained.o.d" -MT ${OBJECTDIR}/_ext/1472/attiny817-xplained.o -o ${OBJECTDIR}/_ext/1472/attiny817-xplained.o ../attiny817-xplained.c 
	
${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o: ../../../drivers/adc/adc-tinyavr.c  .generated_files/flags/Config1/862810d83890b3a995643ef6d317050d95f6b136 .generated_files/flags/Config1/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462846893" 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_CONSOLE -DDEMO_WDT -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config1=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o.d" -MT "${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o.d" -MT ${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o -o ${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o ../../../drivers/adc/adc-tinyavr.c 
	
${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o: ../../../drivers/mux/mux-tinyavr.c  .generated_files/flags/Config1/7da3f338203572d7f30c322691d8fcc8ec9104bc .generated_files/flags/Config1/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462858973" 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_CONSOLE -DDEMO_WDT -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config1=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o.d" -MT "${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o.d" -MT ${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o -o ${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o ../../../drivers/mux/mux-tinyavr.c 
	
${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o: ../../../drivers/pwm/pwm-tinyavr_tca.c  .generated_files/flags/Config1/5b2744ba9fb10637f2fc845312bc0fcd38a4801 .generated_files/flags/Config1/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462861907" 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_CONSOLE -DDEMO_WDT -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config1=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o.d" -MT "${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o.d" -MT ${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o -o ${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o ../../../drivers/pwm/pwm-tinyavr_tca.c 
	
${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o: ../../../drivers/clock/clock-tinyavr.c  .generated_files/flags/Config1/d8a5a938d12f9c88720d92a24b84c22392b30402 .generated_files/flags/Config1/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1343658459" 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_CONSOLE -DDEMO_WDT -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config1=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o.d" -MT "${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o.d" -MT ${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o -o ${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o ../../../drivers/clock/clock-tinyavr.c 
	
${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o: ../../../drivers/gpio/gpio-tinyavr.c  .generated_files/flags/Config1/69bde027ef0e1ec26cd549297cd70ec9b3c7cf51 .generated_files/flags/Config1/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1896195998" 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_CONSOLE -DDEMO_WDT -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config1=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o.d" -MT "${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o.d" -MT ${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o -o ${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o ../../../drivers/gpio/gpio-tinyavr.c 
	
${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o: ../../../drivers/spi/spi-tinyavr.c  .generated_files/flags/Config1/d9e9dc2401caa7d6da1e14c80892d0bacb9fd0ad .generated_files/flags/Config1/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462864569" 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_CONSOLE -DDEMO_WDT -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config1=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o.d" -MT "${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o.d" -MT ${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o -o ${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o ../../../drivers/spi/spi-tinyavr.c 
	
${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o: ../../../drivers/uart/uart-tinyavr_usart.c  .generated_files/flags/Config1/c97aedbbe169c6f86a881f37956aeddf59c3a51b .generated_files/flags/Config1/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1895793055" 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o.d 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_CONSOLE -DDEMO_WDT -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config1=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o.d" -MT "${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o.d" -MT ${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o -o ${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o ../../../drivers/uart/uart-tinyavr_usart.c 
	
${OBJECTDIR}/_ext/229905376/wd-tinyavr.o: ../../../drivers/wd/wd-tinyavr.c  .generated_files/flags/Config1/89a3d563cda14a023fc48e53831d705800cb9a9c .generated_files/flags/Config1/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/229905376" 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-tinyavr.o.d 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-tinyavr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_CONSOLE -DDEMO_WDT -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config1=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/229905376/wd-tinyavr.o.d" -MT "${OBJECTDIR}/_ext/229905376/wd-tinyavr.o.d" -MT ${OBJECTDIR}/_ext/229905376/wd-tinyavr.o -o ${OBJECTDIR}/_ext/229905376/wd-tinyavr.o ../../../drivers/wd/wd-tinyavr.c 
	
${OBJECTDIR}/_ext/2124829536/picoRTOS.o: ../../../picoRTOS.c  .generated_files/flags/Config1/7f77815e6390c3e93d51fb5145e4aa14b2ac575d .generated_files/flags/Config1/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2124829536" 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_CONSOLE -DDEMO_WDT -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config1=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d" -MT "${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d" -MT ${OBJECTDIR}/_ext/2124829536/picoRTOS.o -o ${OBJECTDIR}/_ext/2124829536/picoRTOS.o ../../../picoRTOS.c 
	
else
${OBJECTDIR}/_ext/95298038/picoRTOS_port.o: ../../../arch/avr/avrxmega3/picoRTOS_port.c  .generated_files/flags/Config1/fd52136d29ecfb5caaa6177c66ab6d2483283208 .generated_files/flags/Config1/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/95298038" 
	@${RM} ${OBJECTDIR}/_ext/95298038/picoRTOS_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/95298038/picoRTOS_port.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_CONSOLE -DDEMO_WDT -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config1=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/95298038/picoRTOS_port.o.d" -MT "${OBJECTDIR}/_ext/95298038/picoRTOS_port.o.d" -MT ${OBJECTDIR}/_ext/95298038/picoRTOS_port.o -o ${OBJECTDIR}/_ext/95298038/picoRTOS_port.o ../../../arch/avr/avrxmega3/picoRTOS_port.c 
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  .generated_files/flags/Config1/4a3bd91811cebe91dd742546113769afb8a03541 .generated_files/flags/Config1/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_CONSOLE -DDEMO_WDT -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config1=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1472/main.o.d" -MT "${OBJECTDIR}/_ext/1472/main.o.d" -MT ${OBJECTDIR}/_ext/1472/main.o -o ${OBJECTDIR}/_ext/1472/main.o ../main.c 
	
${OBJECTDIR}/_ext/1472/attiny817-xplained.o: ../attiny817-xplained.c  .generated_files/flags/Config1/20702c902c964eec1c5e209e509e661c66e064c5 .generated_files/flags/Config1/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/attiny817-xplained.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/attiny817-xplained.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_CONSOLE -DDEMO_WDT -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config1=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1472/attiny817-xplained.o.d" -MT "${OBJECTDIR}/_ext/1472/attiny817-xplained.o.d" -MT ${OBJECTDIR}/_ext/1472/attiny817-xplained.o -o ${OBJECTDIR}/_ext/1472/attiny817-xplained.o ../attiny817-xplained.c 
	
${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o: ../../../drivers/adc/adc-tinyavr.c  .generated_files/flags/Config1/1176fb1b1663e3f5df43666b5aeaa991be232c89 .generated_files/flags/Config1/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462846893" 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_CONSOLE -DDEMO_WDT -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config1=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o.d" -MT "${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o.d" -MT ${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o -o ${OBJECTDIR}/_ext/1462846893/adc-tinyavr.o ../../../drivers/adc/adc-tinyavr.c 
	
${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o: ../../../drivers/mux/mux-tinyavr.c  .generated_files/flags/Config1/113d8ad4be0c1e540b1142542514938f2b10e6ad .generated_files/flags/Config1/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462858973" 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_CONSOLE -DDEMO_WDT -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config1=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o.d" -MT "${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o.d" -MT ${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o -o ${OBJECTDIR}/_ext/1462858973/mux-tinyavr.o ../../../drivers/mux/mux-tinyavr.c 
	
${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o: ../../../drivers/pwm/pwm-tinyavr_tca.c  .generated_files/flags/Config1/f78e5674cb139478ec4bc953ef9d17f78ac9625b .generated_files/flags/Config1/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462861907" 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_CONSOLE -DDEMO_WDT -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config1=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o.d" -MT "${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o.d" -MT ${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o -o ${OBJECTDIR}/_ext/1462861907/pwm-tinyavr_tca.o ../../../drivers/pwm/pwm-tinyavr_tca.c 
	
${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o: ../../../drivers/clock/clock-tinyavr.c  .generated_files/flags/Config1/6cb5fb4ad20cac26749e1d7a49be8dacf02e21e7 .generated_files/flags/Config1/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1343658459" 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_CONSOLE -DDEMO_WDT -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config1=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o.d" -MT "${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o.d" -MT ${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o -o ${OBJECTDIR}/_ext/1343658459/clock-tinyavr.o ../../../drivers/clock/clock-tinyavr.c 
	
${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o: ../../../drivers/gpio/gpio-tinyavr.c  .generated_files/flags/Config1/4b4d860ef4325daf5cbd2b165fcb9735520d8c6f .generated_files/flags/Config1/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1896195998" 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_CONSOLE -DDEMO_WDT -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config1=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o.d" -MT "${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o.d" -MT ${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o -o ${OBJECTDIR}/_ext/1896195998/gpio-tinyavr.o ../../../drivers/gpio/gpio-tinyavr.c 
	
${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o: ../../../drivers/spi/spi-tinyavr.c  .generated_files/flags/Config1/8f8b435a04f7d7f2667361938074dd5aee7b8455 .generated_files/flags/Config1/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462864569" 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_CONSOLE -DDEMO_WDT -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config1=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o.d" -MT "${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o.d" -MT ${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o -o ${OBJECTDIR}/_ext/1462864569/spi-tinyavr.o ../../../drivers/spi/spi-tinyavr.c 
	
${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o: ../../../drivers/uart/uart-tinyavr_usart.c  .generated_files/flags/Config1/2f04036ad2b3d5a42bd627c33edfec9dad6416a5 .generated_files/flags/Config1/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1895793055" 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o.d 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_CONSOLE -DDEMO_WDT -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config1=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o.d" -MT "${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o.d" -MT ${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o -o ${OBJECTDIR}/_ext/1895793055/uart-tinyavr_usart.o ../../../drivers/uart/uart-tinyavr_usart.c 
	
${OBJECTDIR}/_ext/229905376/wd-tinyavr.o: ../../../drivers/wd/wd-tinyavr.c  .generated_files/flags/Config1/4390fac9173c992f63ad3f83cc72badaeca4ebc .generated_files/flags/Config1/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/229905376" 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-tinyavr.o.d 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-tinyavr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_CONSOLE -DDEMO_WDT -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config1=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/229905376/wd-tinyavr.o.d" -MT "${OBJECTDIR}/_ext/229905376/wd-tinyavr.o.d" -MT ${OBJECTDIR}/_ext/229905376/wd-tinyavr.o -o ${OBJECTDIR}/_ext/229905376/wd-tinyavr.o ../../../drivers/wd/wd-tinyavr.c 
	
${OBJECTDIR}/_ext/2124829536/picoRTOS.o: ../../../picoRTOS.c  .generated_files/flags/Config1/c49b14c645fd4c4029b44b040148350ada24510c .generated_files/flags/Config1/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2124829536" 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_CONSOLE -DDEMO_WDT -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config1=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d" -MT "${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d" -MT ${OBJECTDIR}/_ext/2124829536/picoRTOS.o -o ${OBJECTDIR}/_ext/2124829536/picoRTOS.o ../../../picoRTOS.c 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o: ../../../arch/avr/avrxmega3/picoRTOS_portasm.S  .generated_files/flags/Config1/cf55c096e500ab9cfb2802556a8773dc33332d98 .generated_files/flags/Config1/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/95298038" 
	@${RM} ${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o.d 
	@${RM} ${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o 
	${MP_CC} -c $(MP_EXTRA_AS_PRE) -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x assembler-with-cpp -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_CONSOLE -DDEMO_WDT -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config1=$(CND_CONF)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem -Wa,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1   -MD -MP -MF "${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o.d" -MT "${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o.d" -MT ${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o -o ${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o  ../../../arch/avr/avrxmega3/picoRTOS_portasm.S 
	
else
${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o: ../../../arch/avr/avrxmega3/picoRTOS_portasm.S  .generated_files/flags/Config1/4e1efa632feb18c0e95ae83eb1332837f7caf730 .generated_files/flags/Config1/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/95298038" 
	@${RM} ${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o.d 
	@${RM} ${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o 
	${MP_CC} -c $(MP_EXTRA_AS_PRE) -mcpu=$(MP_PROCESSOR_OPTION)  -x assembler-with-cpp -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_CONSOLE -DDEMO_WDT -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config1=$(CND_CONF)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem -Wa,--defsym=__MPLAB_BUILD=1   -MD -MP -MF "${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o.d" -MT "${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o.d" -MT ${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o -o ${OBJECTDIR}/_ext/95298038/picoRTOS_portasm.o  ../../../arch/avr/avrxmega3/picoRTOS_portasm.S 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/attiny817-xplained.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=${DISTDIR}/attiny817-xplained.X.${IMAGE_TYPE}.map  -D__DEBUG=1  -DXPRJ_Config1=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"   -gdwarf-2 -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_CONSOLE -DDEMO_WDT -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     $(COMPARISON_BUILD) -Wl,--memorysummary,${DISTDIR}/memoryfile.xml -o ${DISTDIR}/attiny817-xplained.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  -o ${DISTDIR}/attiny817-xplained.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -Wl,--start-group  -Wl,-lm -Wl,--end-group  -Wl,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1
	@${RM} ${DISTDIR}/attiny817-xplained.X.${IMAGE_TYPE}.hex 
	
else
${DISTDIR}/attiny817-xplained.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=${DISTDIR}/attiny817-xplained.X.${IMAGE_TYPE}.map  -DXPRJ_Config1=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_CONSOLE -DDEMO_WDT -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     $(COMPARISON_BUILD) -Wl,--memorysummary,${DISTDIR}/memoryfile.xml -o ${DISTDIR}/attiny817-xplained.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  -o ${DISTDIR}/attiny817-xplained.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -Wl,--start-group  -Wl,-lm -Wl,--end-group 
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
