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
SOURCEFILES_QUOTED_IF_SPACED=../adafruit-itsybitsy-m4.c ../main.c ../../../drivers/clock/clock-same5x.c ../../../drivers/mux/mux-same5x.c ../../../drivers/gpio/gpio-same5x.c ../../../drivers/spi/spi-sercom.c ../../../drivers/pwm/pwm-same5x_tc.c ../../../drivers/uart/uart-sercom.c ../../../drivers/adc/adc-same5x.c ../../../drivers/wd/wd-same5x.c ../../../arch/arm/cm4f/picoRTOS_port.c ../../../arch/arm/cm4f/picoRTOS_portasm.S ../../../devices/atmel/atsame5x/startup.S ../../../picoRTOS.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1472/adafruit-itsybitsy-m4.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1343658459/clock-same5x.o ${OBJECTDIR}/_ext/1462858973/mux-same5x.o ${OBJECTDIR}/_ext/1896195998/gpio-same5x.o ${OBJECTDIR}/_ext/1462864569/spi-sercom.o ${OBJECTDIR}/_ext/1462861907/pwm-same5x_tc.o ${OBJECTDIR}/_ext/1895793055/uart-sercom.o ${OBJECTDIR}/_ext/1462846893/adc-same5x.o ${OBJECTDIR}/_ext/229905376/wd-same5x.o ${OBJECTDIR}/_ext/1328606823/picoRTOS_port.o ${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o ${OBJECTDIR}/_ext/1781163083/startup.o ${OBJECTDIR}/_ext/2124829536/picoRTOS.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1472/adafruit-itsybitsy-m4.o.d ${OBJECTDIR}/_ext/1472/main.o.d ${OBJECTDIR}/_ext/1343658459/clock-same5x.o.d ${OBJECTDIR}/_ext/1462858973/mux-same5x.o.d ${OBJECTDIR}/_ext/1896195998/gpio-same5x.o.d ${OBJECTDIR}/_ext/1462864569/spi-sercom.o.d ${OBJECTDIR}/_ext/1462861907/pwm-same5x_tc.o.d ${OBJECTDIR}/_ext/1895793055/uart-sercom.o.d ${OBJECTDIR}/_ext/1462846893/adc-same5x.o.d ${OBJECTDIR}/_ext/229905376/wd-same5x.o.d ${OBJECTDIR}/_ext/1328606823/picoRTOS_port.o.d ${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o.d ${OBJECTDIR}/_ext/1781163083/startup.o.d ${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1472/adafruit-itsybitsy-m4.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1343658459/clock-same5x.o ${OBJECTDIR}/_ext/1462858973/mux-same5x.o ${OBJECTDIR}/_ext/1896195998/gpio-same5x.o ${OBJECTDIR}/_ext/1462864569/spi-sercom.o ${OBJECTDIR}/_ext/1462861907/pwm-same5x_tc.o ${OBJECTDIR}/_ext/1895793055/uart-sercom.o ${OBJECTDIR}/_ext/1462846893/adc-same5x.o ${OBJECTDIR}/_ext/229905376/wd-same5x.o ${OBJECTDIR}/_ext/1328606823/picoRTOS_port.o ${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o ${OBJECTDIR}/_ext/1781163083/startup.o ${OBJECTDIR}/_ext/2124829536/picoRTOS.o

# Source Files
SOURCEFILES=../adafruit-itsybitsy-m4.c ../main.c ../../../drivers/clock/clock-same5x.c ../../../drivers/mux/mux-same5x.c ../../../drivers/gpio/gpio-same5x.c ../../../drivers/spi/spi-sercom.c ../../../drivers/pwm/pwm-same5x_tc.c ../../../drivers/uart/uart-sercom.c ../../../drivers/adc/adc-same5x.c ../../../drivers/wd/wd-same5x.c ../../../arch/arm/cm4f/picoRTOS_port.c ../../../arch/arm/cm4f/picoRTOS_portasm.S ../../../devices/atmel/atsame5x/startup.S ../../../picoRTOS.c

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
${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o: ../../../arch/arm/cm4f/picoRTOS_portasm.S  .generated_files/flags/default/48a9f096d9074f87332240cc1afc47545df76d98 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1328606823" 
	@${RM} ${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o 
	@${RM} ${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o.ok ${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o.err 
	${MP_CC} $(MP_EXTRA_AS_PRE) -mcpu=cortex-m4  -D__DEBUG  -gdwarf-2  -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" -I "../../../devices" -MP -MMD -MF "${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o.d"  -o ${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o ../../../arch/arm/cm4f/picoRTOS_portasm.S  -DXPRJ_default=$(CND_CONF)  -mfpu=fpv4-sp-d16 -g -O0 -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o.asm.d",--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1 ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1781163083/startup.o: ../../../devices/atmel/atsame5x/startup.S  .generated_files/flags/default/16bdddd89367c5bb66f6002933624d28f894cf03 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1781163083" 
	@${RM} ${OBJECTDIR}/_ext/1781163083/startup.o.d 
	@${RM} ${OBJECTDIR}/_ext/1781163083/startup.o 
	@${RM} ${OBJECTDIR}/_ext/1781163083/startup.o.ok ${OBJECTDIR}/_ext/1781163083/startup.o.err 
	${MP_CC} $(MP_EXTRA_AS_PRE) -mcpu=cortex-m4  -D__DEBUG  -gdwarf-2  -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" -I "../../../devices" -MP -MMD -MF "${OBJECTDIR}/_ext/1781163083/startup.o.d"  -o ${OBJECTDIR}/_ext/1781163083/startup.o ../../../devices/atmel/atsame5x/startup.S  -DXPRJ_default=$(CND_CONF)  -mfpu=fpv4-sp-d16 -g -O0 -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/_ext/1781163083/startup.o.asm.d",--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1 ${PACK_COMMON_OPTIONS} 
	
else
${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o: ../../../arch/arm/cm4f/picoRTOS_portasm.S  .generated_files/flags/default/699a70317162ce029c425cc7be95f392bbc3f475 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1328606823" 
	@${RM} ${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o 
	@${RM} ${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o.ok ${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o.err 
	${MP_CC} $(MP_EXTRA_AS_PRE) -mcpu=cortex-m4  -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" -I "../../../devices" -MP -MMD -MF "${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o.d"  -o ${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o ../../../arch/arm/cm4f/picoRTOS_portasm.S  -DXPRJ_default=$(CND_CONF)  -mfpu=fpv4-sp-d16 -g -O0 -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/_ext/1328606823/picoRTOS_portasm.o.asm.d" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1781163083/startup.o: ../../../devices/atmel/atsame5x/startup.S  .generated_files/flags/default/3d9c1530ed24d2335c72b41be1c435c684452bdb .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1781163083" 
	@${RM} ${OBJECTDIR}/_ext/1781163083/startup.o.d 
	@${RM} ${OBJECTDIR}/_ext/1781163083/startup.o 
	@${RM} ${OBJECTDIR}/_ext/1781163083/startup.o.ok ${OBJECTDIR}/_ext/1781163083/startup.o.err 
	${MP_CC} $(MP_EXTRA_AS_PRE) -mcpu=cortex-m4  -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" -I "../../../devices" -MP -MMD -MF "${OBJECTDIR}/_ext/1781163083/startup.o.d"  -o ${OBJECTDIR}/_ext/1781163083/startup.o ../../../devices/atmel/atsame5x/startup.S  -DXPRJ_default=$(CND_CONF)  -mfpu=fpv4-sp-d16 -g -O0 -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/_ext/1781163083/startup.o.asm.d" ${PACK_COMMON_OPTIONS} 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/adafruit-itsybitsy-m4.o: ../adafruit-itsybitsy-m4.c  .generated_files/flags/default/16598ff3b583871c3ff23fe3434d6c5fa2f3f63c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/adafruit-itsybitsy-m4.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/adafruit-itsybitsy-m4.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" -I "../../../devices" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1472/adafruit-itsybitsy-m4.o.d" -o ${OBJECTDIR}/_ext/1472/adafruit-itsybitsy-m4.o ../adafruit-itsybitsy-m4.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  .generated_files/flags/default/15df86b296ffae93be379691a8aeadcae9663947 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" -I "../../../devices" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d" -o ${OBJECTDIR}/_ext/1472/main.o ../main.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1343658459/clock-same5x.o: ../../../drivers/clock/clock-same5x.c  .generated_files/flags/default/bcbbb54bc7573028b4e384774e09ddf1843cb73 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1343658459" 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-same5x.o.d 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-same5x.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" -I "../../../devices" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1343658459/clock-same5x.o.d" -o ${OBJECTDIR}/_ext/1343658459/clock-same5x.o ../../../drivers/clock/clock-same5x.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1462858973/mux-same5x.o: ../../../drivers/mux/mux-same5x.c  .generated_files/flags/default/d84a8e306ebfef73ca8fcf7bb5487ec1424442ce .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462858973" 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-same5x.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-same5x.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" -I "../../../devices" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1462858973/mux-same5x.o.d" -o ${OBJECTDIR}/_ext/1462858973/mux-same5x.o ../../../drivers/mux/mux-same5x.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1896195998/gpio-same5x.o: ../../../drivers/gpio/gpio-same5x.c  .generated_files/flags/default/2079f1cb57ccc52748df7a4c8888ac2cbfbc3539 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1896195998" 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-same5x.o.d 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-same5x.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" -I "../../../devices" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1896195998/gpio-same5x.o.d" -o ${OBJECTDIR}/_ext/1896195998/gpio-same5x.o ../../../drivers/gpio/gpio-same5x.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1462864569/spi-sercom.o: ../../../drivers/spi/spi-sercom.c  .generated_files/flags/default/de9147d00342ed29969a40f940387c881011c31f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462864569" 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-sercom.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-sercom.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" -I "../../../devices" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1462864569/spi-sercom.o.d" -o ${OBJECTDIR}/_ext/1462864569/spi-sercom.o ../../../drivers/spi/spi-sercom.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1462861907/pwm-same5x_tc.o: ../../../drivers/pwm/pwm-same5x_tc.c  .generated_files/flags/default/f0aa48060c3bf9d52036a2a187d8ed087b53977a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462861907" 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-same5x_tc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-same5x_tc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" -I "../../../devices" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1462861907/pwm-same5x_tc.o.d" -o ${OBJECTDIR}/_ext/1462861907/pwm-same5x_tc.o ../../../drivers/pwm/pwm-same5x_tc.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1895793055/uart-sercom.o: ../../../drivers/uart/uart-sercom.c  .generated_files/flags/default/fb454725e9b7536dadf8752893579c189d554aa1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1895793055" 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-sercom.o.d 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-sercom.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" -I "../../../devices" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1895793055/uart-sercom.o.d" -o ${OBJECTDIR}/_ext/1895793055/uart-sercom.o ../../../drivers/uart/uart-sercom.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1462846893/adc-same5x.o: ../../../drivers/adc/adc-same5x.c  .generated_files/flags/default/5787d4fa49b3f712154922cea04aa930fd339f1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462846893" 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-same5x.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-same5x.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" -I "../../../devices" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1462846893/adc-same5x.o.d" -o ${OBJECTDIR}/_ext/1462846893/adc-same5x.o ../../../drivers/adc/adc-same5x.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/229905376/wd-same5x.o: ../../../drivers/wd/wd-same5x.c  .generated_files/flags/default/52a167b3b343d8bcf13dbcdd9757cbc5aba59347 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/229905376" 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-same5x.o.d 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-same5x.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" -I "../../../devices" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/229905376/wd-same5x.o.d" -o ${OBJECTDIR}/_ext/229905376/wd-same5x.o ../../../drivers/wd/wd-same5x.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1328606823/picoRTOS_port.o: ../../../arch/arm/cm4f/picoRTOS_port.c  .generated_files/flags/default/2dd4c2dc695153a0bb2b3d23c0dd7750fb8156bd .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1328606823" 
	@${RM} ${OBJECTDIR}/_ext/1328606823/picoRTOS_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1328606823/picoRTOS_port.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" -I "../../../devices" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1328606823/picoRTOS_port.o.d" -o ${OBJECTDIR}/_ext/1328606823/picoRTOS_port.o ../../../arch/arm/cm4f/picoRTOS_port.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/2124829536/picoRTOS.o: ../../../picoRTOS.c  .generated_files/flags/default/b92f4028df6b1f4d5162e93a6e0e187f19955ff9 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2124829536" 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" -I "../../../devices" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d" -o ${OBJECTDIR}/_ext/2124829536/picoRTOS.o ../../../picoRTOS.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
else
${OBJECTDIR}/_ext/1472/adafruit-itsybitsy-m4.o: ../adafruit-itsybitsy-m4.c  .generated_files/flags/default/d54c273020f2ff95ff02f16e812c3ae6e55950f1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/adafruit-itsybitsy-m4.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/adafruit-itsybitsy-m4.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" -I "../../../devices" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1472/adafruit-itsybitsy-m4.o.d" -o ${OBJECTDIR}/_ext/1472/adafruit-itsybitsy-m4.o ../adafruit-itsybitsy-m4.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  .generated_files/flags/default/e420bb1b5c77c6dc38ccdfe250cabd0d6320bb32 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" -I "../../../devices" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d" -o ${OBJECTDIR}/_ext/1472/main.o ../main.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1343658459/clock-same5x.o: ../../../drivers/clock/clock-same5x.c  .generated_files/flags/default/e87d24f3dbb36a1dccaa7cad4ad5e6301ce1ace2 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1343658459" 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-same5x.o.d 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-same5x.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" -I "../../../devices" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1343658459/clock-same5x.o.d" -o ${OBJECTDIR}/_ext/1343658459/clock-same5x.o ../../../drivers/clock/clock-same5x.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1462858973/mux-same5x.o: ../../../drivers/mux/mux-same5x.c  .generated_files/flags/default/82b6863088cffea1b39437ee3e640d43606f2041 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462858973" 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-same5x.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-same5x.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" -I "../../../devices" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1462858973/mux-same5x.o.d" -o ${OBJECTDIR}/_ext/1462858973/mux-same5x.o ../../../drivers/mux/mux-same5x.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1896195998/gpio-same5x.o: ../../../drivers/gpio/gpio-same5x.c  .generated_files/flags/default/f57014b5000f32c36281d5a36d4500882b4c1c91 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1896195998" 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-same5x.o.d 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-same5x.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" -I "../../../devices" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1896195998/gpio-same5x.o.d" -o ${OBJECTDIR}/_ext/1896195998/gpio-same5x.o ../../../drivers/gpio/gpio-same5x.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1462864569/spi-sercom.o: ../../../drivers/spi/spi-sercom.c  .generated_files/flags/default/e30d84d5d3d07a587234af120dbea2b714de9fc9 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462864569" 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-sercom.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-sercom.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" -I "../../../devices" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1462864569/spi-sercom.o.d" -o ${OBJECTDIR}/_ext/1462864569/spi-sercom.o ../../../drivers/spi/spi-sercom.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1462861907/pwm-same5x_tc.o: ../../../drivers/pwm/pwm-same5x_tc.c  .generated_files/flags/default/61c122287deb844997b654157da66ab033137275 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462861907" 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-same5x_tc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-same5x_tc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" -I "../../../devices" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1462861907/pwm-same5x_tc.o.d" -o ${OBJECTDIR}/_ext/1462861907/pwm-same5x_tc.o ../../../drivers/pwm/pwm-same5x_tc.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1895793055/uart-sercom.o: ../../../drivers/uart/uart-sercom.c  .generated_files/flags/default/7250740b8749b74d5a58f09a6fc98d76d2a9498 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1895793055" 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-sercom.o.d 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-sercom.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" -I "../../../devices" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1895793055/uart-sercom.o.d" -o ${OBJECTDIR}/_ext/1895793055/uart-sercom.o ../../../drivers/uart/uart-sercom.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1462846893/adc-same5x.o: ../../../drivers/adc/adc-same5x.c  .generated_files/flags/default/ae13d68b83458a861cd017231656b4b9330df7d7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462846893" 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-same5x.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-same5x.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" -I "../../../devices" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1462846893/adc-same5x.o.d" -o ${OBJECTDIR}/_ext/1462846893/adc-same5x.o ../../../drivers/adc/adc-same5x.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/229905376/wd-same5x.o: ../../../drivers/wd/wd-same5x.c  .generated_files/flags/default/ef4893385cec282f8c612aab401d0b9687daaaa .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/229905376" 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-same5x.o.d 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-same5x.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" -I "../../../devices" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/229905376/wd-same5x.o.d" -o ${OBJECTDIR}/_ext/229905376/wd-same5x.o ../../../drivers/wd/wd-same5x.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/1328606823/picoRTOS_port.o: ../../../arch/arm/cm4f/picoRTOS_port.c  .generated_files/flags/default/fb9d8aa4972638bf1529532b443923d97d4030be .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1328606823" 
	@${RM} ${OBJECTDIR}/_ext/1328606823/picoRTOS_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1328606823/picoRTOS_port.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" -I "../../../devices" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1328606823/picoRTOS_port.o.d" -o ${OBJECTDIR}/_ext/1328606823/picoRTOS_port.o ../../../arch/arm/cm4f/picoRTOS_port.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
${OBJECTDIR}/_ext/2124829536/picoRTOS.o: ../../../picoRTOS.c  .generated_files/flags/default/9d32642cfe98b27667f87c0e3383bba3a27599a1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2124829536" 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "../" -I "../../.." -I "../../../arch/include" -I "../../../arch/arm/cm4f" -I "../../../drivers/include" -I "../../../drivers/clock" -I "../../../devices/atmel/atsame5x" -I "../../../drivers/mux" -I "../../../drivers/gpio" -I "../../../drivers/spi" -I "../../../drivers/uart" -I "../../../drivers/pwm" -I "../../../drivers/usart" -I "../../../drivers/adc" -I "../../../drivers/wd" -I "../../../devices" ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d" -o ${OBJECTDIR}/_ext/2124829536/picoRTOS.o ../../../picoRTOS.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -g -O0
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/adafruit-itsybitsy-m4.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    ../../../devices/atmel/atsame5x/gcc/atsamd51g_ram.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=cortex-m4   -gdwarf-2  -D__$(MP_PROCESSOR_OPTION)__   -T"../../../devices/atmel/atsame5x/gcc/atsamd51g_ram.ld"  -mthumb -nostartfiles -nodefaultlibs -nostdlib -static -Wl,-Map="${DISTDIR}/adafruit-itsybitsy-m4.X.${IMAGE_TYPE}.map"  -o ${DISTDIR}/adafruit-itsybitsy-m4.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}      -DXPRJ_default=$(CND_CONF)  -mfpu=fpv4-sp-d16 -g -O0 $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1 -Wl,--gc-sections  
	
	
	
	
	
	
else
${DISTDIR}/adafruit-itsybitsy-m4.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   ../../../devices/atmel/atsame5x/gcc/atsamd51g_ram.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=cortex-m4  -D__$(MP_PROCESSOR_OPTION)__   -T"../../../devices/atmel/atsame5x/gcc/atsamd51g_ram.ld"  -mthumb -nostartfiles -nodefaultlibs -nostdlib -static -Wl,-Map="${DISTDIR}/adafruit-itsybitsy-m4.X.${IMAGE_TYPE}.map"  -o ${DISTDIR}/adafruit-itsybitsy-m4.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}      -DXPRJ_default=$(CND_CONF)  -mfpu=fpv4-sp-d16 -g -O0 $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION) -Wl,--gc-sections  
	
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
