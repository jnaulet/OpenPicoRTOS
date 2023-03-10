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
SOURCEFILES_QUOTED_IF_SPACED=../../../arch/arm/cm4f/picoRTOS_port.c ../../../arch/arm/cm4f/picoRTOS_portasm.S ../adafruit-itsybitsy-m4.c ../main.c ../../../drivers/clock/clock-same5x.c ../../../drivers/mux/mux-same5x.c ../../../drivers/gpio/gpio-same5x.c ../../../drivers/spi/spi-sercom.c ../../../drivers/pwm/pwm-same5x_tc.c ../../../drivers/uart/uart-sercom.c ../../../drivers/adc/adc-same5x.c ../../../picoRTOS.c startup_samd51g19a.c system_samd51g19a.c ../../../drivers/wd/wd-same5x.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1328606823/picoRTOS_port.o ${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o ${OBJECTDIR}/_ext/1472/adafruit-itsybitsy-m4.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1343658459/clock-same5x.o ${OBJECTDIR}/_ext/1462858973/mux-same5x.o ${OBJECTDIR}/_ext/1896195998/gpio-same5x.o ${OBJECTDIR}/_ext/1462864569/spi-sercom.o ${OBJECTDIR}/_ext/1462861907/pwm-same5x_tc.o ${OBJECTDIR}/_ext/1895793055/uart-sercom.o ${OBJECTDIR}/_ext/1462846893/adc-same5x.o ${OBJECTDIR}/_ext/2124829536/picoRTOS.o ${OBJECTDIR}/startup_samd51g19a.o ${OBJECTDIR}/system_samd51g19a.o ${OBJECTDIR}/_ext/229905376/wd-same5x.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1328606823/picoRTOS_port.o.d ${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o.d ${OBJECTDIR}/_ext/1472/adafruit-itsybitsy-m4.o.d ${OBJECTDIR}/_ext/1472/main.o.d ${OBJECTDIR}/_ext/1343658459/clock-same5x.o.d ${OBJECTDIR}/_ext/1462858973/mux-same5x.o.d ${OBJECTDIR}/_ext/1896195998/gpio-same5x.o.d ${OBJECTDIR}/_ext/1462864569/spi-sercom.o.d ${OBJECTDIR}/_ext/1462861907/pwm-same5x_tc.o.d ${OBJECTDIR}/_ext/1895793055/uart-sercom.o.d ${OBJECTDIR}/_ext/1462846893/adc-same5x.o.d ${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d ${OBJECTDIR}/startup_samd51g19a.o.d ${OBJECTDIR}/system_samd51g19a.o.d ${OBJECTDIR}/_ext/229905376/wd-same5x.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1328606823/picoRTOS_port.o ${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o ${OBJECTDIR}/_ext/1472/adafruit-itsybitsy-m4.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1343658459/clock-same5x.o ${OBJECTDIR}/_ext/1462858973/mux-same5x.o ${OBJECTDIR}/_ext/1896195998/gpio-same5x.o ${OBJECTDIR}/_ext/1462864569/spi-sercom.o ${OBJECTDIR}/_ext/1462861907/pwm-same5x_tc.o ${OBJECTDIR}/_ext/1895793055/uart-sercom.o ${OBJECTDIR}/_ext/1462846893/adc-same5x.o ${OBJECTDIR}/_ext/2124829536/picoRTOS.o ${OBJECTDIR}/startup_samd51g19a.o ${OBJECTDIR}/system_samd51g19a.o ${OBJECTDIR}/_ext/229905376/wd-same5x.o

# Source Files
SOURCEFILES=../../../arch/arm/cm4f/picoRTOS_port.c ../../../arch/arm/cm4f/picoRTOS_portasm.S ../adafruit-itsybitsy-m4.c ../main.c ../../../drivers/clock/clock-same5x.c ../../../drivers/mux/mux-same5x.c ../../../drivers/gpio/gpio-same5x.c ../../../drivers/spi/spi-sercom.c ../../../drivers/pwm/pwm-same5x_tc.c ../../../drivers/uart/uart-sercom.c ../../../drivers/adc/adc-same5x.c ../../../picoRTOS.c startup_samd51g19a.c system_samd51g19a.c ../../../drivers/wd/wd-same5x.c

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
${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o: ../../../arch/arm/cm4f/picoRTOS_portasm.S  .generated_files/flags/default/66e3874c19293ba133f3d9ead4f37f03798a1604 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1328606823" 
	@${RM} ${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o 
	@${RM} ${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o.ok ${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o.err 
	${MP_CC} $(MP_EXTRA_AS_PRE) -mcpu=cortex-m4  -D__DEBUG  -gdwarf-2  -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" -MP -MMD -MF "${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o.d"  -o ${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o ../../../arch/arm/cm4f/picoRTOS_portasm.S  -DXPRJ_default=$(CND_CONF)  -mfpu=fpv4-sp-d16 -g -O0 -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o.asm.d",--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1 ${PACK_COMMON_OPTIONS} 
	
else
${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o: ../../../arch/arm/cm4f/picoRTOS_portasm.S  .generated_files/flags/default/8295e68532151f749a223f1f2cd6a3aa65165952 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1328606823" 
	@${RM} ${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o 
	@${RM} ${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o.ok ${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o.err 
	${MP_CC} $(MP_EXTRA_AS_PRE) -mcpu=cortex-m4  -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" -MP -MMD -MF "${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o.d"  -o ${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o ../../../arch/arm/cm4f/picoRTOS_portasm.S  -DXPRJ_default=$(CND_CONF)  -mfpu=fpv4-sp-d16 -g -O0 -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o.asm.d" ${PACK_COMMON_OPTIONS} 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1328606823/picoRTOS_port.o: ../../../arch/arm/cm4f/picoRTOS_port.c  .generated_files/flags/default/d8ccefa926daf74bee652811c71143b5714fd881 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1328606823" 
	@${RM} ${OBJECTDIR}/_ext/1328606823/picoRTOS_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1328606823/picoRTOS_port.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1328606823/picoRTOS_port.o.d" -o ${OBJECTDIR}/_ext/1328606823/picoRTOS_port.o ../../../arch/arm/cm4f/picoRTOS_port.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1472/adafruit-itsybitsy-m4.o: ../adafruit-itsybitsy-m4.c  .generated_files/flags/default/16a0b70e9375df17bb275a145e1b64d42d527e91 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/adafruit-itsybitsy-m4.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/adafruit-itsybitsy-m4.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1472/adafruit-itsybitsy-m4.o.d" -o ${OBJECTDIR}/_ext/1472/adafruit-itsybitsy-m4.o ../adafruit-itsybitsy-m4.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  .generated_files/flags/default/323fabdbdc27d49df82bdad06528e62c6e5c0172 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d" -o ${OBJECTDIR}/_ext/1472/main.o ../main.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1343658459/clock-same5x.o: ../../../drivers/clock/clock-same5x.c  .generated_files/flags/default/f6b0042a6f5615b0d727d57a54b0f4cd3c6cacde .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1343658459" 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-same5x.o.d 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-same5x.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1343658459/clock-same5x.o.d" -o ${OBJECTDIR}/_ext/1343658459/clock-same5x.o ../../../drivers/clock/clock-same5x.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1462858973/mux-same5x.o: ../../../drivers/mux/mux-same5x.c  .generated_files/flags/default/46bd650ed071762a0b5bd6002d866625003a4d58 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462858973" 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-same5x.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-same5x.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1462858973/mux-same5x.o.d" -o ${OBJECTDIR}/_ext/1462858973/mux-same5x.o ../../../drivers/mux/mux-same5x.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1896195998/gpio-same5x.o: ../../../drivers/gpio/gpio-same5x.c  .generated_files/flags/default/5df00cd2c104be0c0353b50bf047cc796ae8c13c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1896195998" 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-same5x.o.d 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-same5x.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1896195998/gpio-same5x.o.d" -o ${OBJECTDIR}/_ext/1896195998/gpio-same5x.o ../../../drivers/gpio/gpio-same5x.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1462864569/spi-sercom.o: ../../../drivers/spi/spi-sercom.c  .generated_files/flags/default/b1edb3a1c82370f8200392ff23b63b4961a03823 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462864569" 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-sercom.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-sercom.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1462864569/spi-sercom.o.d" -o ${OBJECTDIR}/_ext/1462864569/spi-sercom.o ../../../drivers/spi/spi-sercom.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1462861907/pwm-same5x_tc.o: ../../../drivers/pwm/pwm-same5x_tc.c  .generated_files/flags/default/6e8f32542c9a4d6f7d05abf905d6a647bf9f8aa5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462861907" 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-same5x_tc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-same5x_tc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1462861907/pwm-same5x_tc.o.d" -o ${OBJECTDIR}/_ext/1462861907/pwm-same5x_tc.o ../../../drivers/pwm/pwm-same5x_tc.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1895793055/uart-sercom.o: ../../../drivers/uart/uart-sercom.c  .generated_files/flags/default/b14d34e3c9ac644ca2eb2cd10eec94d35098a8bb .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1895793055" 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-sercom.o.d 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-sercom.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1895793055/uart-sercom.o.d" -o ${OBJECTDIR}/_ext/1895793055/uart-sercom.o ../../../drivers/uart/uart-sercom.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1462846893/adc-same5x.o: ../../../drivers/adc/adc-same5x.c  .generated_files/flags/default/9793bc828b527826b62037239c45c8e3c19f23b3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462846893" 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-same5x.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-same5x.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1462846893/adc-same5x.o.d" -o ${OBJECTDIR}/_ext/1462846893/adc-same5x.o ../../../drivers/adc/adc-same5x.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/2124829536/picoRTOS.o: ../../../picoRTOS.c  .generated_files/flags/default/ef0699fa1ff5dfa5680cd273321b233eef830d86 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2124829536" 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d" -o ${OBJECTDIR}/_ext/2124829536/picoRTOS.o ../../../picoRTOS.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/startup_samd51g19a.o: startup_samd51g19a.c  .generated_files/flags/default/50d9eb65f10a0e5ad1101694d369350c8641aa44 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/startup_samd51g19a.o.d 
	@${RM} ${OBJECTDIR}/startup_samd51g19a.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/startup_samd51g19a.o.d" -o ${OBJECTDIR}/startup_samd51g19a.o startup_samd51g19a.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/system_samd51g19a.o: system_samd51g19a.c  .generated_files/flags/default/5152fe07a555e94f2b5cf6aebbf3572144198ff2 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/system_samd51g19a.o.d 
	@${RM} ${OBJECTDIR}/system_samd51g19a.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/system_samd51g19a.o.d" -o ${OBJECTDIR}/system_samd51g19a.o system_samd51g19a.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/229905376/wd-same5x.o: ../../../drivers/wd/wd-same5x.c  .generated_files/flags/default/257694f929641d2b7bb54076f7c50acf3710a9f9 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/229905376" 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-same5x.o.d 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-same5x.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/229905376/wd-same5x.o.d" -o ${OBJECTDIR}/_ext/229905376/wd-same5x.o ../../../drivers/wd/wd-same5x.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
else
${OBJECTDIR}/_ext/1328606823/picoRTOS_port.o: ../../../arch/arm/cm4f/picoRTOS_port.c  .generated_files/flags/default/c7447861a02b069876e35a3b0e2da57f91e68a28 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1328606823" 
	@${RM} ${OBJECTDIR}/_ext/1328606823/picoRTOS_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1328606823/picoRTOS_port.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1328606823/picoRTOS_port.o.d" -o ${OBJECTDIR}/_ext/1328606823/picoRTOS_port.o ../../../arch/arm/cm4f/picoRTOS_port.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1472/adafruit-itsybitsy-m4.o: ../adafruit-itsybitsy-m4.c  .generated_files/flags/default/e1416a5b7283a691b0a5ebd1cd9aaf29e0494f32 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/adafruit-itsybitsy-m4.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/adafruit-itsybitsy-m4.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1472/adafruit-itsybitsy-m4.o.d" -o ${OBJECTDIR}/_ext/1472/adafruit-itsybitsy-m4.o ../adafruit-itsybitsy-m4.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  .generated_files/flags/default/3a402177aa5d2f28d3954be7ab1e6e074fb255b6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d" -o ${OBJECTDIR}/_ext/1472/main.o ../main.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1343658459/clock-same5x.o: ../../../drivers/clock/clock-same5x.c  .generated_files/flags/default/714c5d121d8108c0f6bb37934d777186ec0b1e3c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1343658459" 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-same5x.o.d 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-same5x.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1343658459/clock-same5x.o.d" -o ${OBJECTDIR}/_ext/1343658459/clock-same5x.o ../../../drivers/clock/clock-same5x.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1462858973/mux-same5x.o: ../../../drivers/mux/mux-same5x.c  .generated_files/flags/default/2842330f60ad98317fcc7e2415c74a0f6b7c4555 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462858973" 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-same5x.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-same5x.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1462858973/mux-same5x.o.d" -o ${OBJECTDIR}/_ext/1462858973/mux-same5x.o ../../../drivers/mux/mux-same5x.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1896195998/gpio-same5x.o: ../../../drivers/gpio/gpio-same5x.c  .generated_files/flags/default/2f218ca42a632f1ddcd098ddf8b019092fd68476 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1896195998" 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-same5x.o.d 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-same5x.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1896195998/gpio-same5x.o.d" -o ${OBJECTDIR}/_ext/1896195998/gpio-same5x.o ../../../drivers/gpio/gpio-same5x.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1462864569/spi-sercom.o: ../../../drivers/spi/spi-sercom.c  .generated_files/flags/default/502436ad9a9f2c3f18d18c4278a9d28a546109c2 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462864569" 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-sercom.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-sercom.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1462864569/spi-sercom.o.d" -o ${OBJECTDIR}/_ext/1462864569/spi-sercom.o ../../../drivers/spi/spi-sercom.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1462861907/pwm-same5x_tc.o: ../../../drivers/pwm/pwm-same5x_tc.c  .generated_files/flags/default/eaf5df4d6252f38f9af1f66519d7410d5e306109 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462861907" 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-same5x_tc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-same5x_tc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1462861907/pwm-same5x_tc.o.d" -o ${OBJECTDIR}/_ext/1462861907/pwm-same5x_tc.o ../../../drivers/pwm/pwm-same5x_tc.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1895793055/uart-sercom.o: ../../../drivers/uart/uart-sercom.c  .generated_files/flags/default/ca33ac0c1c3e64ec68d9af9efdef039b97db51d8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1895793055" 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-sercom.o.d 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-sercom.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1895793055/uart-sercom.o.d" -o ${OBJECTDIR}/_ext/1895793055/uart-sercom.o ../../../drivers/uart/uart-sercom.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1462846893/adc-same5x.o: ../../../drivers/adc/adc-same5x.c  .generated_files/flags/default/35e83dd0696677aa6717510af125e4d4bc2158ae .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462846893" 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-same5x.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-same5x.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1462846893/adc-same5x.o.d" -o ${OBJECTDIR}/_ext/1462846893/adc-same5x.o ../../../drivers/adc/adc-same5x.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/2124829536/picoRTOS.o: ../../../picoRTOS.c  .generated_files/flags/default/28c0f6d147a0a24da79bd3b1ea33407b39259268 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2124829536" 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d" -o ${OBJECTDIR}/_ext/2124829536/picoRTOS.o ../../../picoRTOS.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/startup_samd51g19a.o: startup_samd51g19a.c  .generated_files/flags/default/9cd098e759ed92001f1554db0db48193cb963ee0 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/startup_samd51g19a.o.d 
	@${RM} ${OBJECTDIR}/startup_samd51g19a.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/startup_samd51g19a.o.d" -o ${OBJECTDIR}/startup_samd51g19a.o startup_samd51g19a.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/system_samd51g19a.o: system_samd51g19a.c  .generated_files/flags/default/69a8e2d11fb1565709a5123935b97cd0f9aa16a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/system_samd51g19a.o.d 
	@${RM} ${OBJECTDIR}/system_samd51g19a.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/system_samd51g19a.o.d" -o ${OBJECTDIR}/system_samd51g19a.o system_samd51g19a.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/229905376/wd-same5x.o: ../../../drivers/wd/wd-same5x.c  .generated_files/flags/default/86d8f16abc79f8f4e6dec24a96f94e54696141 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/229905376" 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-same5x.o.d 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-same5x.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/229905376/wd-same5x.o.d" -o ${OBJECTDIR}/_ext/229905376/wd-same5x.o ../../../drivers/wd/wd-same5x.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
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
