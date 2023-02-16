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
	
${OBJECTDIR}/_ext/1462846893/adc-attiny1x.o: ../../../drivers/adc/adc-attiny1x.c  .generated_files/flags/Config1/5dbd622a0dc248246cac0758615c8289adfec226 .generated_files/flags/Config1/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462846893" 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-attiny1x.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-attiny1x.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_CONSOLE -DDEMO_WDT -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config1=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1462846893/adc-attiny1x.o.d" -MT "${OBJECTDIR}/_ext/1462846893/adc-attiny1x.o.d" -MT ${OBJECTDIR}/_ext/1462846893/adc-attiny1x.o -o ${OBJECTDIR}/_ext/1462846893/adc-attiny1x.o ../../../drivers/adc/adc-attiny1x.c 
	
${OBJECTDIR}/_ext/1462858973/mux-attiny1x.o: ../../../drivers/mux/mux-attiny1x.c  .generated_files/flags/Config1/eaa808ab1cdd224b1099d471bdd14a814e6b325a .generated_files/flags/Config1/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462858973" 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-attiny1x.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-attiny1x.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_CONSOLE -DDEMO_WDT -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config1=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1462858973/mux-attiny1x.o.d" -MT "${OBJECTDIR}/_ext/1462858973/mux-attiny1x.o.d" -MT ${OBJECTDIR}/_ext/1462858973/mux-attiny1x.o -o ${OBJECTDIR}/_ext/1462858973/mux-attiny1x.o ../../../drivers/mux/mux-attiny1x.c 
	
${OBJECTDIR}/_ext/1462861907/pwm-attiny1x_tca.o: ../../../drivers/pwm/pwm-attiny1x_tca.c  .generated_files/flags/Config1/3b6457510705448c6d6e06e1d0f2cf98c7a27de3 .generated_files/flags/Config1/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462861907" 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-attiny1x_tca.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-attiny1x_tca.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_CONSOLE -DDEMO_WDT -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config1=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1462861907/pwm-attiny1x_tca.o.d" -MT "${OBJECTDIR}/_ext/1462861907/pwm-attiny1x_tca.o.d" -MT ${OBJECTDIR}/_ext/1462861907/pwm-attiny1x_tca.o -o ${OBJECTDIR}/_ext/1462861907/pwm-attiny1x_tca.o ../../../drivers/pwm/pwm-attiny1x_tca.c 
	
${OBJECTDIR}/_ext/1343658459/clock-attiny1x.o: ../../../drivers/clock/clock-attiny1x.c  .generated_files/flags/Config1/f8e282ef4f1d9bd1d7156daabb4445f37815189b .generated_files/flags/Config1/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1343658459" 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-attiny1x.o.d 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-attiny1x.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_CONSOLE -DDEMO_WDT -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config1=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1343658459/clock-attiny1x.o.d" -MT "${OBJECTDIR}/_ext/1343658459/clock-attiny1x.o.d" -MT ${OBJECTDIR}/_ext/1343658459/clock-attiny1x.o -o ${OBJECTDIR}/_ext/1343658459/clock-attiny1x.o ../../../drivers/clock/clock-attiny1x.c 
	
${OBJECTDIR}/_ext/1896195998/gpio-attiny1x.o: ../../../drivers/gpio/gpio-attiny1x.c  .generated_files/flags/Config1/acadf078372d6132ad7d344ac89710b85db96cd9 .generated_files/flags/Config1/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1896195998" 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-attiny1x.o.d 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-attiny1x.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_CONSOLE -DDEMO_WDT -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config1=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1896195998/gpio-attiny1x.o.d" -MT "${OBJECTDIR}/_ext/1896195998/gpio-attiny1x.o.d" -MT ${OBJECTDIR}/_ext/1896195998/gpio-attiny1x.o -o ${OBJECTDIR}/_ext/1896195998/gpio-attiny1x.o ../../../drivers/gpio/gpio-attiny1x.c 
	
${OBJECTDIR}/_ext/1462864569/spi-attiny1x.o: ../../../drivers/spi/spi-attiny1x.c  .generated_files/flags/Config1/c4ebc535da8fed9b495120de5dc039b8c9fba2c7 .generated_files/flags/Config1/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462864569" 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-attiny1x.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-attiny1x.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_CONSOLE -DDEMO_WDT -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config1=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1462864569/spi-attiny1x.o.d" -MT "${OBJECTDIR}/_ext/1462864569/spi-attiny1x.o.d" -MT ${OBJECTDIR}/_ext/1462864569/spi-attiny1x.o -o ${OBJECTDIR}/_ext/1462864569/spi-attiny1x.o ../../../drivers/spi/spi-attiny1x.c 
	
${OBJECTDIR}/_ext/1895793055/uart-attiny1x_usart.o: ../../../drivers/uart/uart-attiny1x_usart.c  .generated_files/flags/Config1/475f6e16d00e06d873a85eea213bf6092b2cd2a8 .generated_files/flags/Config1/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1895793055" 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-attiny1x_usart.o.d 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-attiny1x_usart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_CONSOLE -DDEMO_WDT -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config1=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1895793055/uart-attiny1x_usart.o.d" -MT "${OBJECTDIR}/_ext/1895793055/uart-attiny1x_usart.o.d" -MT ${OBJECTDIR}/_ext/1895793055/uart-attiny1x_usart.o -o ${OBJECTDIR}/_ext/1895793055/uart-attiny1x_usart.o ../../../drivers/uart/uart-attiny1x_usart.c 
	
${OBJECTDIR}/_ext/229905376/wd-attiny1x.o: ../../../drivers/wd/wd-attiny1x.c  .generated_files/flags/Config1/e994af7ed03f2ed460a7340c46f0995ed6c9952b .generated_files/flags/Config1/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/229905376" 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-attiny1x.o.d 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-attiny1x.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_CONSOLE -DDEMO_WDT -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config1=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/229905376/wd-attiny1x.o.d" -MT "${OBJECTDIR}/_ext/229905376/wd-attiny1x.o.d" -MT ${OBJECTDIR}/_ext/229905376/wd-attiny1x.o -o ${OBJECTDIR}/_ext/229905376/wd-attiny1x.o ../../../drivers/wd/wd-attiny1x.c 
	
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
	
${OBJECTDIR}/_ext/1462846893/adc-attiny1x.o: ../../../drivers/adc/adc-attiny1x.c  .generated_files/flags/Config1/acf10b80240d3aad42d11708d4e3ac35038f8790 .generated_files/flags/Config1/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462846893" 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-attiny1x.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-attiny1x.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_CONSOLE -DDEMO_WDT -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config1=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1462846893/adc-attiny1x.o.d" -MT "${OBJECTDIR}/_ext/1462846893/adc-attiny1x.o.d" -MT ${OBJECTDIR}/_ext/1462846893/adc-attiny1x.o -o ${OBJECTDIR}/_ext/1462846893/adc-attiny1x.o ../../../drivers/adc/adc-attiny1x.c 
	
${OBJECTDIR}/_ext/1462858973/mux-attiny1x.o: ../../../drivers/mux/mux-attiny1x.c  .generated_files/flags/Config1/7213d7a5cdbe8b5c1396cd9df85fd414c85bd9e .generated_files/flags/Config1/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462858973" 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-attiny1x.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-attiny1x.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_CONSOLE -DDEMO_WDT -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config1=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1462858973/mux-attiny1x.o.d" -MT "${OBJECTDIR}/_ext/1462858973/mux-attiny1x.o.d" -MT ${OBJECTDIR}/_ext/1462858973/mux-attiny1x.o -o ${OBJECTDIR}/_ext/1462858973/mux-attiny1x.o ../../../drivers/mux/mux-attiny1x.c 
	
${OBJECTDIR}/_ext/1462861907/pwm-attiny1x_tca.o: ../../../drivers/pwm/pwm-attiny1x_tca.c  .generated_files/flags/Config1/1f85ff99dead17016942d8fa0b9fe0aee0d462e0 .generated_files/flags/Config1/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462861907" 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-attiny1x_tca.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-attiny1x_tca.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_CONSOLE -DDEMO_WDT -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config1=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1462861907/pwm-attiny1x_tca.o.d" -MT "${OBJECTDIR}/_ext/1462861907/pwm-attiny1x_tca.o.d" -MT ${OBJECTDIR}/_ext/1462861907/pwm-attiny1x_tca.o -o ${OBJECTDIR}/_ext/1462861907/pwm-attiny1x_tca.o ../../../drivers/pwm/pwm-attiny1x_tca.c 
	
${OBJECTDIR}/_ext/1343658459/clock-attiny1x.o: ../../../drivers/clock/clock-attiny1x.c  .generated_files/flags/Config1/d1eeb5ce12c035062a69d4dd415ead1130963b51 .generated_files/flags/Config1/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1343658459" 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-attiny1x.o.d 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-attiny1x.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_CONSOLE -DDEMO_WDT -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config1=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1343658459/clock-attiny1x.o.d" -MT "${OBJECTDIR}/_ext/1343658459/clock-attiny1x.o.d" -MT ${OBJECTDIR}/_ext/1343658459/clock-attiny1x.o -o ${OBJECTDIR}/_ext/1343658459/clock-attiny1x.o ../../../drivers/clock/clock-attiny1x.c 
	
${OBJECTDIR}/_ext/1896195998/gpio-attiny1x.o: ../../../drivers/gpio/gpio-attiny1x.c  .generated_files/flags/Config1/8ff7fc3956f6ee1de271efdba523501c0249c239 .generated_files/flags/Config1/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1896195998" 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-attiny1x.o.d 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-attiny1x.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_CONSOLE -DDEMO_WDT -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config1=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1896195998/gpio-attiny1x.o.d" -MT "${OBJECTDIR}/_ext/1896195998/gpio-attiny1x.o.d" -MT ${OBJECTDIR}/_ext/1896195998/gpio-attiny1x.o -o ${OBJECTDIR}/_ext/1896195998/gpio-attiny1x.o ../../../drivers/gpio/gpio-attiny1x.c 
	
${OBJECTDIR}/_ext/1462864569/spi-attiny1x.o: ../../../drivers/spi/spi-attiny1x.c  .generated_files/flags/Config1/a513cc53659c09fff44b4876bb81438289d4f4bd .generated_files/flags/Config1/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462864569" 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-attiny1x.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-attiny1x.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_CONSOLE -DDEMO_WDT -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config1=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1462864569/spi-attiny1x.o.d" -MT "${OBJECTDIR}/_ext/1462864569/spi-attiny1x.o.d" -MT ${OBJECTDIR}/_ext/1462864569/spi-attiny1x.o -o ${OBJECTDIR}/_ext/1462864569/spi-attiny1x.o ../../../drivers/spi/spi-attiny1x.c 
	
${OBJECTDIR}/_ext/1895793055/uart-attiny1x_usart.o: ../../../drivers/uart/uart-attiny1x_usart.c  .generated_files/flags/Config1/e199d871a0493d0177b3db1189c4eef3dbb262ef .generated_files/flags/Config1/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1895793055" 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-attiny1x_usart.o.d 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-attiny1x_usart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_CONSOLE -DDEMO_WDT -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config1=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/1895793055/uart-attiny1x_usart.o.d" -MT "${OBJECTDIR}/_ext/1895793055/uart-attiny1x_usart.o.d" -MT ${OBJECTDIR}/_ext/1895793055/uart-attiny1x_usart.o -o ${OBJECTDIR}/_ext/1895793055/uart-attiny1x_usart.o ../../../drivers/uart/uart-attiny1x_usart.c 
	
${OBJECTDIR}/_ext/229905376/wd-attiny1x.o: ../../../drivers/wd/wd-attiny1x.c  .generated_files/flags/Config1/aca9fddf07e193aed62ef5dd9be43a4d77232864 .generated_files/flags/Config1/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/229905376" 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-attiny1x.o.d 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-attiny1x.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -DDEMO_LED -DDEMO_CONSOLE -DDEMO_WDT -I"../" -I"../../../" -I"../../../arch/include" -I"../../../arch/avr/avrxmega3" -I"../../../devices/atmel/attiny817" -I"../../../drivers/include" -I"../../../drivers/adc" -I"../../../drivers/mux" -I"../../../drivers/pwm" -I"../../../drivers/clock" -I"../../../drivers/gpio" -I"../../../drivers/spi" -I"../../../drivers/uart" -I"../../../drivers/wd" -Wall -DXPRJ_Config1=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mno-const-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/_ext/229905376/wd-attiny1x.o.d" -MT "${OBJECTDIR}/_ext/229905376/wd-attiny1x.o.d" -MT ${OBJECTDIR}/_ext/229905376/wd-attiny1x.o -o ${OBJECTDIR}/_ext/229905376/wd-attiny1x.o ../../../drivers/wd/wd-attiny1x.c 
	
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
