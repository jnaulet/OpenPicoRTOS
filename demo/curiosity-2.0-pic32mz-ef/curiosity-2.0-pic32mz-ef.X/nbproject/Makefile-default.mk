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
FINAL_IMAGE=${DISTDIR}/curiosity-2.0-pic32mz-ef.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/curiosity-2.0-pic32mz-ef.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
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
SOURCEFILES_QUOTED_IF_SPACED=../curiosity-2.0-pic32mz-ef.c ../main.c ../../../drivers/clock/clock-pic32mx.c ../../../drivers/mux/mux-pic32mx.c ../../../drivers/gpio/gpio-pic32mx.c ../../../drivers/uart/uart-pic32mx.c ../../../drivers/misc/misc-pic32mx.c ../../../drivers/spi/spi-pic32mx.c ../../../drivers/wd/wd-pic32mx.c ../../../drivers/pwm/pwm-pic32mx.c ../../../drivers/twi/twi-pic32mx.c ../../../drivers/adc/adc-pic32mx.c ../../../drivers/can/can-pic32mx.c ../../../arch/mips32/m51xx/picoRTOS_port.c ../../../arch/mips32/m51xx/picoRTOS_portasm.S ../../../ipc/picoRTOS_cond.c ../../../ipc/picoRTOS_mutex.c ../../../picoRTOS.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o ${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o ${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o ${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o ${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o ${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o ${OBJECTDIR}/_ext/229905376/wd-pic32mx.o ${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o ${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o ${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o ${OBJECTDIR}/_ext/1462848733/can-pic32mx.o ${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o ${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o ${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o ${OBJECTDIR}/_ext/2124829536/picoRTOS.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o.d ${OBJECTDIR}/_ext/1472/main.o.d ${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o.d ${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o.d ${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o.d ${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o.d ${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o.d ${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o.d ${OBJECTDIR}/_ext/229905376/wd-pic32mx.o.d ${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o.d ${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o.d ${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o.d ${OBJECTDIR}/_ext/1462848733/can-pic32mx.o.d ${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o.d ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.d ${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o.d ${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o.d ${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o ${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o ${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o ${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o ${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o ${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o ${OBJECTDIR}/_ext/229905376/wd-pic32mx.o ${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o ${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o ${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o ${OBJECTDIR}/_ext/1462848733/can-pic32mx.o ${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o ${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o ${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o ${OBJECTDIR}/_ext/2124829536/picoRTOS.o

# Source Files
SOURCEFILES=../curiosity-2.0-pic32mz-ef.c ../main.c ../../../drivers/clock/clock-pic32mx.c ../../../drivers/mux/mux-pic32mx.c ../../../drivers/gpio/gpio-pic32mx.c ../../../drivers/uart/uart-pic32mx.c ../../../drivers/misc/misc-pic32mx.c ../../../drivers/spi/spi-pic32mx.c ../../../drivers/wd/wd-pic32mx.c ../../../drivers/pwm/pwm-pic32mx.c ../../../drivers/twi/twi-pic32mx.c ../../../drivers/adc/adc-pic32mx.c ../../../drivers/can/can-pic32mx.c ../../../arch/mips32/m51xx/picoRTOS_port.c ../../../arch/mips32/m51xx/picoRTOS_portasm.S ../../../ipc/picoRTOS_cond.c ../../../ipc/picoRTOS_mutex.c ../../../picoRTOS.c



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
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/curiosity-2.0-pic32mz-ef.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MZ2048EFM144
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o: ../../../arch/mips32/m51xx/picoRTOS_portasm.S  .generated_files/flags/default/2254f52cb5166a156138f1376f323d709b105be .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1430854306" 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.ok ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.err 
	${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.d"  -o ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o ../../../arch/mips32/m51xx/picoRTOS_portasm.S  -DXPRJ_default=$(CND_CONF)    -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.asm.d",--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1,-I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -mdfp="${DFP_DIR}"
	@${FIXDEPS} "${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.d" "${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o: ../../../arch/mips32/m51xx/picoRTOS_portasm.S  .generated_files/flags/default/540e1c06ece7be081feddad10aabeb762cb20cf1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1430854306" 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.ok ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.err 
	${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.d"  -o ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o ../../../arch/mips32/m51xx/picoRTOS_portasm.S  -DXPRJ_default=$(CND_CONF)    -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.asm.d",--gdwarf-2,-I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -mdfp="${DFP_DIR}"
	@${FIXDEPS} "${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.d" "${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o: ../curiosity-2.0-pic32mz-ef.c  .generated_files/flags/default/9218f58bbaff273155e0e353e6f90776f0a0087f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o.d" -o ${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o ../curiosity-2.0-pic32mz-ef.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  .generated_files/flags/default/6a4d28f1f423a306751d0cd7233a5d15097ceb4a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d" -o ${OBJECTDIR}/_ext/1472/main.o ../main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o: ../../../drivers/clock/clock-pic32mx.c  .generated_files/flags/default/60af5285b356af9e9d489c87a1c0c96f3e72993e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1343658459" 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o ../../../drivers/clock/clock-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o: ../../../drivers/mux/mux-pic32mx.c  .generated_files/flags/default/d4bce2d59599aba9b91d7af513bf9c2fcf3f81cf .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462858973" 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o ../../../drivers/mux/mux-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o: ../../../drivers/gpio/gpio-pic32mx.c  .generated_files/flags/default/c4726e6830bc53ef972c5275fa0fa4a43f74b006 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1896195998" 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o ../../../drivers/gpio/gpio-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o: ../../../drivers/uart/uart-pic32mx.c  .generated_files/flags/default/e917099e683e0e16b8545f024f1135482bc709c9 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1895793055" 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o ../../../drivers/uart/uart-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o: ../../../drivers/misc/misc-pic32mx.c  .generated_files/flags/default/91d2dce956d97f0e8506700690d609f4aee521a8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1896023681" 
	@${RM} ${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o ../../../drivers/misc/misc-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o: ../../../drivers/spi/spi-pic32mx.c  .generated_files/flags/default/411245147920c0ca1e7547c23b996febb277ddd0 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462864569" 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o ../../../drivers/spi/spi-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/229905376/wd-pic32mx.o: ../../../drivers/wd/wd-pic32mx.c  .generated_files/flags/default/223621ee28e6910385dfff75332610c4d76d1edd .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/229905376" 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/229905376/wd-pic32mx.o.d" -o ${OBJECTDIR}/_ext/229905376/wd-pic32mx.o ../../../drivers/wd/wd-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o: ../../../drivers/pwm/pwm-pic32mx.c  .generated_files/flags/default/a346f847c8910095e317f09a459eea380989196e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462861907" 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o ../../../drivers/pwm/pwm-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o: ../../../drivers/twi/twi-pic32mx.c  .generated_files/flags/default/c6af07cb3d4ac78384f1c78bc772434d715cd325 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462865747" 
	@${RM} ${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o ../../../drivers/twi/twi-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o: ../../../drivers/adc/adc-pic32mx.c  .generated_files/flags/default/405d88efe3c0aea6d62f2d224d15721d05d3859c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462846893" 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o ../../../drivers/adc/adc-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1462848733/can-pic32mx.o: ../../../drivers/can/can-pic32mx.c  .generated_files/flags/default/f8c14150928f426c3383756b2f828822055c4b89 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462848733" 
	@${RM} ${OBJECTDIR}/_ext/1462848733/can-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462848733/can-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462848733/can-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462848733/can-pic32mx.o ../../../drivers/can/can-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o: ../../../arch/mips32/m51xx/picoRTOS_port.c  .generated_files/flags/default/5d5b76479b7de24e5ab17a7b099a9f85173d3091 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1430854306" 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o.d" -o ${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o ../../../arch/mips32/m51xx/picoRTOS_port.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o: ../../../ipc/picoRTOS_cond.c  .generated_files/flags/default/f644c1334a01df0d5083a1d055b3ba4a048679d5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1386518765" 
	@${RM} ${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o.d" -o ${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o ../../../ipc/picoRTOS_cond.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o: ../../../ipc/picoRTOS_mutex.c  .generated_files/flags/default/dd73fc573d8dc7daa43448c658d3b99ecc9119de .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1386518765" 
	@${RM} ${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o.d" -o ${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o ../../../ipc/picoRTOS_mutex.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/2124829536/picoRTOS.o: ../../../picoRTOS.c  .generated_files/flags/default/8d7a1dd30122ce198348296e74e2902e81c29826 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2124829536" 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d" -o ${OBJECTDIR}/_ext/2124829536/picoRTOS.o ../../../picoRTOS.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
else
${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o: ../curiosity-2.0-pic32mz-ef.c  .generated_files/flags/default/926a23c7ce623d9fff8eb5962e725ff5fd52728 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o.d" -o ${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o ../curiosity-2.0-pic32mz-ef.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  .generated_files/flags/default/f76693c4a70c8162c050c541cf45c60fd2c17629 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d" -o ${OBJECTDIR}/_ext/1472/main.o ../main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o: ../../../drivers/clock/clock-pic32mx.c  .generated_files/flags/default/5f94fac3c652523ed2300d57c347900984fe9753 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1343658459" 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o ../../../drivers/clock/clock-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o: ../../../drivers/mux/mux-pic32mx.c  .generated_files/flags/default/94be44b983b01c406e01a36dbc60a1e06a3202ca .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462858973" 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o ../../../drivers/mux/mux-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o: ../../../drivers/gpio/gpio-pic32mx.c  .generated_files/flags/default/ca144e0c76e0b702446dd09fe91775ad50ce7e93 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1896195998" 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o ../../../drivers/gpio/gpio-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o: ../../../drivers/uart/uart-pic32mx.c  .generated_files/flags/default/d19b3252beb1a03c12ebc792a8dc1d9ed3fdfe6c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1895793055" 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o ../../../drivers/uart/uart-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o: ../../../drivers/misc/misc-pic32mx.c  .generated_files/flags/default/52a9f53053a8e1da612792d0f281b0408bc917bf .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1896023681" 
	@${RM} ${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o ../../../drivers/misc/misc-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o: ../../../drivers/spi/spi-pic32mx.c  .generated_files/flags/default/eb3d38a7e5b732739be264812fa51101e0102b00 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462864569" 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o ../../../drivers/spi/spi-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/229905376/wd-pic32mx.o: ../../../drivers/wd/wd-pic32mx.c  .generated_files/flags/default/1fb8c65ae5f6a9cd9d935ae412cfc2d149628fab .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/229905376" 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/229905376/wd-pic32mx.o.d" -o ${OBJECTDIR}/_ext/229905376/wd-pic32mx.o ../../../drivers/wd/wd-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o: ../../../drivers/pwm/pwm-pic32mx.c  .generated_files/flags/default/6a62b4719ccfb046f5631be28a4d1278934bfb60 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462861907" 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o ../../../drivers/pwm/pwm-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o: ../../../drivers/twi/twi-pic32mx.c  .generated_files/flags/default/c4815637cc43993a20304591bf8c0ebd086fc42f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462865747" 
	@${RM} ${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o ../../../drivers/twi/twi-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o: ../../../drivers/adc/adc-pic32mx.c  .generated_files/flags/default/dd6c5e779f4ec317d389dad94090fae81b45841e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462846893" 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o ../../../drivers/adc/adc-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1462848733/can-pic32mx.o: ../../../drivers/can/can-pic32mx.c  .generated_files/flags/default/92c5607dd78c4760a5e66fddaea16acba65581f7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462848733" 
	@${RM} ${OBJECTDIR}/_ext/1462848733/can-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462848733/can-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462848733/can-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462848733/can-pic32mx.o ../../../drivers/can/can-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o: ../../../arch/mips32/m51xx/picoRTOS_port.c  .generated_files/flags/default/f8da5185bc7fc1ea73fde2711449cad03ffe84ec .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1430854306" 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o.d" -o ${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o ../../../arch/mips32/m51xx/picoRTOS_port.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o: ../../../ipc/picoRTOS_cond.c  .generated_files/flags/default/37a0825989ec02ce6ad7195c842c41b24d8c8b7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1386518765" 
	@${RM} ${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o.d" -o ${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o ../../../ipc/picoRTOS_cond.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o: ../../../ipc/picoRTOS_mutex.c  .generated_files/flags/default/4325551d8888974929adbee95f1bafdd460636ad .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1386518765" 
	@${RM} ${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o.d" -o ${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o ../../../ipc/picoRTOS_mutex.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/2124829536/picoRTOS.o: ../../../picoRTOS.c  .generated_files/flags/default/3d4a87cf2fdffbdc4505704633764c4417485670 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2124829536" 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d" -o ${OBJECTDIR}/_ext/2124829536/picoRTOS.o ../../../picoRTOS.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/curiosity-2.0-pic32mz-ef.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g   -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${DISTDIR}/curiosity-2.0-pic32mz-ef.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)   -mreserve=data@0x0:0x37F   -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,${DISTDIR}/memoryfile.xml -mdfp="${DFP_DIR}"
	
else
${DISTDIR}/curiosity-2.0-pic32mz-ef.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${DISTDIR}/curiosity-2.0-pic32mz-ef.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,${DISTDIR}/memoryfile.xml -mdfp="${DFP_DIR}"
	${MP_CC_DIR}/xc32-bin2hex ${DISTDIR}/curiosity-2.0-pic32mz-ef.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
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
