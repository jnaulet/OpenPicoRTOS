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
FINAL_IMAGE=${DISTDIR}/adafruit-itsybitsy-m4.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/adafruit-itsybitsy-m4.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../../../arch/arm/cm4f/picoRTOS_port.c ../../../arch/arm/cm4f/picoRTOS_portasm.S ../adafruit-itsybitsy-m4.c ../main.c ../../../drivers/clock/clock-same5x.c ../../../drivers/mux/mux-same5x.c ../../../drivers/gpio/gpio-same5x.c ../../../drivers/spi/spi-sercom.c ../../../drivers/pwm/pwm-same5x_tc.c ../../../drivers/uart/uart-sercom.c ../../../picoRTOS.c startup_samd51g19a.c system_samd51g19a.c ../../../drivers/adc/adc-same5x.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1328606823/picoRTOS_port.o ${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o ${OBJECTDIR}/_ext/1472/adafruit-itsybitsy-m4.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1343658459/clock-same5x.o ${OBJECTDIR}/_ext/1462858973/mux-same5x.o ${OBJECTDIR}/_ext/1896195998/gpio-same5x.o ${OBJECTDIR}/_ext/1462864569/spi-sercom.o ${OBJECTDIR}/_ext/1462861907/pwm-same5x_tc.o ${OBJECTDIR}/_ext/1895793055/uart-sercom.o ${OBJECTDIR}/_ext/2124829536/picoRTOS.o ${OBJECTDIR}/startup_samd51g19a.o ${OBJECTDIR}/system_samd51g19a.o ${OBJECTDIR}/_ext/1462846893/adc-same5x.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1328606823/picoRTOS_port.o.d ${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o.d ${OBJECTDIR}/_ext/1472/adafruit-itsybitsy-m4.o.d ${OBJECTDIR}/_ext/1472/main.o.d ${OBJECTDIR}/_ext/1343658459/clock-same5x.o.d ${OBJECTDIR}/_ext/1462858973/mux-same5x.o.d ${OBJECTDIR}/_ext/1896195998/gpio-same5x.o.d ${OBJECTDIR}/_ext/1462864569/spi-sercom.o.d ${OBJECTDIR}/_ext/1462861907/pwm-same5x_tc.o.d ${OBJECTDIR}/_ext/1895793055/uart-sercom.o.d ${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d ${OBJECTDIR}/startup_samd51g19a.o.d ${OBJECTDIR}/system_samd51g19a.o.d ${OBJECTDIR}/_ext/1462846893/adc-same5x.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1328606823/picoRTOS_port.o ${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o ${OBJECTDIR}/_ext/1472/adafruit-itsybitsy-m4.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1343658459/clock-same5x.o ${OBJECTDIR}/_ext/1462858973/mux-same5x.o ${OBJECTDIR}/_ext/1896195998/gpio-same5x.o ${OBJECTDIR}/_ext/1462864569/spi-sercom.o ${OBJECTDIR}/_ext/1462861907/pwm-same5x_tc.o ${OBJECTDIR}/_ext/1895793055/uart-sercom.o ${OBJECTDIR}/_ext/2124829536/picoRTOS.o ${OBJECTDIR}/startup_samd51g19a.o ${OBJECTDIR}/system_samd51g19a.o ${OBJECTDIR}/_ext/1462846893/adc-same5x.o

# Source Files
SOURCEFILES=../../../arch/arm/cm4f/picoRTOS_port.c ../../../arch/arm/cm4f/picoRTOS_portasm.S ../adafruit-itsybitsy-m4.c ../main.c ../../../drivers/clock/clock-same5x.c ../../../drivers/mux/mux-same5x.c ../../../drivers/gpio/gpio-same5x.c ../../../drivers/spi/spi-sercom.c ../../../drivers/pwm/pwm-same5x_tc.c ../../../drivers/uart/uart-sercom.c ../../../picoRTOS.c startup_samd51g19a.c system_samd51g19a.c ../../../drivers/adc/adc-same5x.c

# Pack Options 
PACK_COMMON_OPTIONS=-I "${DFP_DIR}/samd51a/include"  -I "${CMSIS_DIR}/CMSIS/Core/Include"



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
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/adafruit-itsybitsy-m4.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=SAMD51G19A
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o: ../../../arch/arm/cm4f/picoRTOS_portasm.S  .generated_files/flags/default/e0b8719cd656c315a2d4074c5a92d524b7eb3c28 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1328606823" 
	@${RM} ${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o 
	@${RM} ${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o.ok ${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o.err 
	${MP_CC} $(MP_EXTRA_AS_PRE) -mcpu=cortex-m4  -D__DEBUG  -gdwarf-2  -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -MP -MMD -MF "${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o.d"  -o ${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o ../../../arch/arm/cm4f/picoRTOS_portasm.S  -DXPRJ_default=$(CND_CONF)  -mfpu=fpv4-sp-d16 -g -O0 -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o.asm.d",--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1 ${PACK_COMMON_OPTIONS} 
	
else
${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o: ../../../arch/arm/cm4f/picoRTOS_portasm.S  .generated_files/flags/default/96fc6a8269fa59a70b02f62f1d594b03da1d27b2 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1328606823" 
	@${RM} ${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o 
	@${RM} ${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o.ok ${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o.err 
	${MP_CC} $(MP_EXTRA_AS_PRE) -mcpu=cortex-m4  -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -MP -MMD -MF "${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o.d"  -o ${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o ../../../arch/arm/cm4f/picoRTOS_portasm.S  -DXPRJ_default=$(CND_CONF)  -mfpu=fpv4-sp-d16 -g -O0 -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o.asm.d" ${PACK_COMMON_OPTIONS} 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1328606823/picoRTOS_port.o: ../../../arch/arm/cm4f/picoRTOS_port.c  .generated_files/flags/default/eca0f734055f61b144a682936251c3b71f7b093e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1328606823" 
	@${RM} ${OBJECTDIR}/_ext/1328606823/picoRTOS_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1328606823/picoRTOS_port.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1328606823/picoRTOS_port.o.d" -o ${OBJECTDIR}/_ext/1328606823/picoRTOS_port.o ../../../arch/arm/cm4f/picoRTOS_port.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1472/adafruit-itsybitsy-m4.o: ../adafruit-itsybitsy-m4.c  .generated_files/flags/default/9e6eb1f9aa916c7ff2b6a193fad6449f72eba8ca .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/adafruit-itsybitsy-m4.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/adafruit-itsybitsy-m4.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1472/adafruit-itsybitsy-m4.o.d" -o ${OBJECTDIR}/_ext/1472/adafruit-itsybitsy-m4.o ../adafruit-itsybitsy-m4.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  .generated_files/flags/default/ab82f5525967df226d3291664a008d8b616ce96d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d" -o ${OBJECTDIR}/_ext/1472/main.o ../main.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1343658459/clock-same5x.o: ../../../drivers/clock/clock-same5x.c  .generated_files/flags/default/735edee2cadd6c87a2f4b8c04e667481672d1831 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1343658459" 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-same5x.o.d 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-same5x.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1343658459/clock-same5x.o.d" -o ${OBJECTDIR}/_ext/1343658459/clock-same5x.o ../../../drivers/clock/clock-same5x.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1462858973/mux-same5x.o: ../../../drivers/mux/mux-same5x.c  .generated_files/flags/default/4f2937fe3aeb587bbdb01adce1952027b5e9a718 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462858973" 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-same5x.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-same5x.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1462858973/mux-same5x.o.d" -o ${OBJECTDIR}/_ext/1462858973/mux-same5x.o ../../../drivers/mux/mux-same5x.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1896195998/gpio-same5x.o: ../../../drivers/gpio/gpio-same5x.c  .generated_files/flags/default/905829f7fc0dbc3be760956bf166dd8c4fe9c58c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1896195998" 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-same5x.o.d 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-same5x.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1896195998/gpio-same5x.o.d" -o ${OBJECTDIR}/_ext/1896195998/gpio-same5x.o ../../../drivers/gpio/gpio-same5x.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1462864569/spi-sercom.o: ../../../drivers/spi/spi-sercom.c  .generated_files/flags/default/914946c379953c1d1d2dbff4863f6a5945360ebd .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462864569" 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-sercom.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-sercom.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1462864569/spi-sercom.o.d" -o ${OBJECTDIR}/_ext/1462864569/spi-sercom.o ../../../drivers/spi/spi-sercom.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1462861907/pwm-same5x_tc.o: ../../../drivers/pwm/pwm-same5x_tc.c  .generated_files/flags/default/8284b9233f2a28031e0a0da515e0e1b93bd5a9bf .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462861907" 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-same5x_tc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-same5x_tc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1462861907/pwm-same5x_tc.o.d" -o ${OBJECTDIR}/_ext/1462861907/pwm-same5x_tc.o ../../../drivers/pwm/pwm-same5x_tc.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1895793055/uart-sercom.o: ../../../drivers/uart/uart-sercom.c  .generated_files/flags/default/52fb38a8c9dc8c757a528b94fff9db0f964d064f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1895793055" 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-sercom.o.d 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-sercom.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1895793055/uart-sercom.o.d" -o ${OBJECTDIR}/_ext/1895793055/uart-sercom.o ../../../drivers/uart/uart-sercom.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/2124829536/picoRTOS.o: ../../../picoRTOS.c  .generated_files/flags/default/ab63e2a388673a756b27bd9a7741fa7ce390caac .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2124829536" 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d" -o ${OBJECTDIR}/_ext/2124829536/picoRTOS.o ../../../picoRTOS.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/startup_samd51g19a.o: startup_samd51g19a.c  .generated_files/flags/default/7887d2de680de23a34cb38bbb4dbf085b243b591 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/startup_samd51g19a.o.d 
	@${RM} ${OBJECTDIR}/startup_samd51g19a.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/startup_samd51g19a.o.d" -o ${OBJECTDIR}/startup_samd51g19a.o startup_samd51g19a.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/system_samd51g19a.o: system_samd51g19a.c  .generated_files/flags/default/efee0fd39eb8782a7e35084e852c0a20cd00cf58 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/system_samd51g19a.o.d 
	@${RM} ${OBJECTDIR}/system_samd51g19a.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/system_samd51g19a.o.d" -o ${OBJECTDIR}/system_samd51g19a.o system_samd51g19a.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1462846893/adc-same5x.o: ../../../drivers/adc/adc-same5x.c  .generated_files/flags/default/d64c1370d84d141b7bc9e9b60a30b80a8521bb52 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462846893" 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-same5x.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-same5x.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1462846893/adc-same5x.o.d" -o ${OBJECTDIR}/_ext/1462846893/adc-same5x.o ../../../drivers/adc/adc-same5x.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
else
${OBJECTDIR}/_ext/1328606823/picoRTOS_port.o: ../../../arch/arm/cm4f/picoRTOS_port.c  .generated_files/flags/default/fe0bf32f4f5c0d242dc37b90caba46ba61e07cd7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1328606823" 
	@${RM} ${OBJECTDIR}/_ext/1328606823/picoRTOS_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1328606823/picoRTOS_port.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1328606823/picoRTOS_port.o.d" -o ${OBJECTDIR}/_ext/1328606823/picoRTOS_port.o ../../../arch/arm/cm4f/picoRTOS_port.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1472/adafruit-itsybitsy-m4.o: ../adafruit-itsybitsy-m4.c  .generated_files/flags/default/a0a873c0a2a6c10a3ebc2fc5c367d1b71e2a0ec8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/adafruit-itsybitsy-m4.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/adafruit-itsybitsy-m4.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1472/adafruit-itsybitsy-m4.o.d" -o ${OBJECTDIR}/_ext/1472/adafruit-itsybitsy-m4.o ../adafruit-itsybitsy-m4.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  .generated_files/flags/default/a55a9f1c3d8d3ae20de63f5322cd10affa86956d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d" -o ${OBJECTDIR}/_ext/1472/main.o ../main.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1343658459/clock-same5x.o: ../../../drivers/clock/clock-same5x.c  .generated_files/flags/default/f56de07de4e5b3076ea5946cadca3fd1dc14160f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1343658459" 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-same5x.o.d 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-same5x.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1343658459/clock-same5x.o.d" -o ${OBJECTDIR}/_ext/1343658459/clock-same5x.o ../../../drivers/clock/clock-same5x.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1462858973/mux-same5x.o: ../../../drivers/mux/mux-same5x.c  .generated_files/flags/default/cfb487253978a2f535033268811dfc9c0c0ed9a3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462858973" 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-same5x.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-same5x.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1462858973/mux-same5x.o.d" -o ${OBJECTDIR}/_ext/1462858973/mux-same5x.o ../../../drivers/mux/mux-same5x.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1896195998/gpio-same5x.o: ../../../drivers/gpio/gpio-same5x.c  .generated_files/flags/default/d25d696d2cfcfbbb30f024d047de94c1508f5e8f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1896195998" 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-same5x.o.d 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-same5x.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1896195998/gpio-same5x.o.d" -o ${OBJECTDIR}/_ext/1896195998/gpio-same5x.o ../../../drivers/gpio/gpio-same5x.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1462864569/spi-sercom.o: ../../../drivers/spi/spi-sercom.c  .generated_files/flags/default/5f7a7b855c66822ce02d4e6fc5b214011723a242 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462864569" 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-sercom.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-sercom.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1462864569/spi-sercom.o.d" -o ${OBJECTDIR}/_ext/1462864569/spi-sercom.o ../../../drivers/spi/spi-sercom.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1462861907/pwm-same5x_tc.o: ../../../drivers/pwm/pwm-same5x_tc.c  .generated_files/flags/default/afb518b9004a4af2a9e3402aa0623e64e0abf2a4 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462861907" 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-same5x_tc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-same5x_tc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1462861907/pwm-same5x_tc.o.d" -o ${OBJECTDIR}/_ext/1462861907/pwm-same5x_tc.o ../../../drivers/pwm/pwm-same5x_tc.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1895793055/uart-sercom.o: ../../../drivers/uart/uart-sercom.c  .generated_files/flags/default/7a73a32016273f4e67071986dd22eb170fe7002e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1895793055" 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-sercom.o.d 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-sercom.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1895793055/uart-sercom.o.d" -o ${OBJECTDIR}/_ext/1895793055/uart-sercom.o ../../../drivers/uart/uart-sercom.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/2124829536/picoRTOS.o: ../../../picoRTOS.c  .generated_files/flags/default/b6273954ef851ab1871fa2219d96b25135c1f97a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2124829536" 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d" -o ${OBJECTDIR}/_ext/2124829536/picoRTOS.o ../../../picoRTOS.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/startup_samd51g19a.o: startup_samd51g19a.c  .generated_files/flags/default/72e40023b4498bd83b56b6d8787e3241ae6daa53 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/startup_samd51g19a.o.d 
	@${RM} ${OBJECTDIR}/startup_samd51g19a.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/startup_samd51g19a.o.d" -o ${OBJECTDIR}/startup_samd51g19a.o startup_samd51g19a.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/system_samd51g19a.o: system_samd51g19a.c  .generated_files/flags/default/3a48d288382a64ae31294051daae9baa8e1956a4 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/system_samd51g19a.o.d 
	@${RM} ${OBJECTDIR}/system_samd51g19a.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/system_samd51g19a.o.d" -o ${OBJECTDIR}/system_samd51g19a.o system_samd51g19a.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1462846893/adc-same5x.o: ../../../drivers/adc/adc-same5x.c  .generated_files/flags/default/175441902f0d691b3ad07b13c7c8823d4bb5ece3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462846893" 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-same5x.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-same5x.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1462846893/adc-same5x.o.d" -o ${OBJECTDIR}/_ext/1462846893/adc-same5x.o ../../../drivers/adc/adc-same5x.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/adafruit-itsybitsy-m4.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    samd51g19a_flash.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=cortex-m4   -gdwarf-2  -D__$(MP_PROCESSOR_OPTION)__   -T"samd51g19a_flash.ld"  -mthumb --specs=nosys.specs -Wl,-Map="${DISTDIR}/adafruit-itsybitsy-m4.X.${IMAGE_TYPE}.map"  -o ${DISTDIR}/adafruit-itsybitsy-m4.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}      -DXPRJ_default=$(CND_CONF)  -mfpu=fpv4-sp-d16 -g -O0 $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1 -Wl,--gc-sections  
	
	
	
	
	
	
else
${DISTDIR}/adafruit-itsybitsy-m4.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   samd51g19a_flash.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=cortex-m4  -D__$(MP_PROCESSOR_OPTION)__   -T"samd51g19a_flash.ld"  -mthumb --specs=nosys.specs -Wl,-Map="${DISTDIR}/adafruit-itsybitsy-m4.X.${IMAGE_TYPE}.map"  -o ${DISTDIR}/adafruit-itsybitsy-m4.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}      -DXPRJ_default=$(CND_CONF)  -mfpu=fpv4-sp-d16 -g -O0 $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION) -Wl,--gc-sections  
	
	${MP_CC_DIR}/arm-none-eabi-objcopy -O ihex -R .eeprom -R .fuse -R .lock -R .signature "${DISTDIR}/adafruit-itsybitsy-m4.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "${DISTDIR}/adafruit-itsybitsy-m4.X.${IMAGE_TYPE}.hex"
	
	
	
	
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
