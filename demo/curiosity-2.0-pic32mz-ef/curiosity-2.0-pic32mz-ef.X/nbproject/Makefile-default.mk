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
SOURCEFILES_QUOTED_IF_SPACED=../curiosity-2.0-pic32mz-ef.c ../main.c ../config.S ../../../drivers/clock/clock-pic32mx.c ../../../drivers/mux/mux-pic32mx.c ../../../drivers/gpio/gpio-pic32mx.c ../../../drivers/uart/uart-pic32mx.c ../../../drivers/misc/misc-pic32mx.c ../../../drivers/spi/spi-pic32mx.c ../../../drivers/wd/wd-pic32mx.c ../../../drivers/pwm/pwm-pic32mx.c ../../../drivers/twi/twi-pic32mx.c ../../../drivers/adc/adc-pic32mx.c ../../../drivers/can/can-pic32mx.c ../../../arch/mips32/m51xx/picoRTOS_port.c ../../../arch/mips32/m51xx/picoRTOS_portasm.S ../../../ipc/picoRTOS_cond.c ../../../ipc/picoRTOS_mutex.c ../../../picoRTOS.c ../../../devices/microchip/pic32mz-ef/startup.S

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1472/config.o ${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o ${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o ${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o ${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o ${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o ${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o ${OBJECTDIR}/_ext/229905376/wd-pic32mx.o ${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o ${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o ${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o ${OBJECTDIR}/_ext/1462848733/can-pic32mx.o ${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o ${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o ${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o ${OBJECTDIR}/_ext/2124829536/picoRTOS.o ${OBJECTDIR}/_ext/441138584/startup.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o.d ${OBJECTDIR}/_ext/1472/main.o.d ${OBJECTDIR}/_ext/1472/config.o.d ${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o.d ${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o.d ${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o.d ${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o.d ${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o.d ${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o.d ${OBJECTDIR}/_ext/229905376/wd-pic32mx.o.d ${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o.d ${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o.d ${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o.d ${OBJECTDIR}/_ext/1462848733/can-pic32mx.o.d ${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o.d ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.d ${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o.d ${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o.d ${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d ${OBJECTDIR}/_ext/441138584/startup.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1472/config.o ${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o ${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o ${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o ${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o ${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o ${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o ${OBJECTDIR}/_ext/229905376/wd-pic32mx.o ${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o ${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o ${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o ${OBJECTDIR}/_ext/1462848733/can-pic32mx.o ${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o ${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o ${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o ${OBJECTDIR}/_ext/2124829536/picoRTOS.o ${OBJECTDIR}/_ext/441138584/startup.o

# Source Files
SOURCEFILES=../curiosity-2.0-pic32mz-ef.c ../main.c ../config.S ../../../drivers/clock/clock-pic32mx.c ../../../drivers/mux/mux-pic32mx.c ../../../drivers/gpio/gpio-pic32mx.c ../../../drivers/uart/uart-pic32mx.c ../../../drivers/misc/misc-pic32mx.c ../../../drivers/spi/spi-pic32mx.c ../../../drivers/wd/wd-pic32mx.c ../../../drivers/pwm/pwm-pic32mx.c ../../../drivers/twi/twi-pic32mx.c ../../../drivers/adc/adc-pic32mx.c ../../../drivers/can/can-pic32mx.c ../../../arch/mips32/m51xx/picoRTOS_port.c ../../../arch/mips32/m51xx/picoRTOS_portasm.S ../../../ipc/picoRTOS_cond.c ../../../ipc/picoRTOS_mutex.c ../../../picoRTOS.c ../../../devices/microchip/pic32mz-ef/startup.S



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
MP_LINKER_FILE_OPTION=,--script="../../../devices/microchip/pic32mz-ef/gcc/pic32mz-efm_2048_ram.ld"
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/config.o: ../config.S  .generated_files/flags/default/45f9cf7e25772fe6b5bda5ed0844dd0f106d88d2 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/config.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/config.o 
	@${RM} ${OBJECTDIR}/_ext/1472/config.o.ok ${OBJECTDIR}/_ext/1472/config.o.err 
	${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/config.o.d"  -o ${OBJECTDIR}/_ext/1472/config.o ../config.S  -DXPRJ_default=$(CND_CONF)    -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/_ext/1472/config.o.asm.d",--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1,-I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/config.o.d" "${OBJECTDIR}/_ext/1472/config.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o: ../../../arch/mips32/m51xx/picoRTOS_portasm.S  .generated_files/flags/default/d9c762de4f3b801946b1ba1358a6f524fefcd7e7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1430854306" 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.ok ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.err 
	${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.d"  -o ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o ../../../arch/mips32/m51xx/picoRTOS_portasm.S  -DXPRJ_default=$(CND_CONF)    -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.asm.d",--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1,-I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.d" "${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/441138584/startup.o: ../../../devices/microchip/pic32mz-ef/startup.S  .generated_files/flags/default/62468fb0a29ab5bd8b29febf1472047c4ef35784 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/441138584" 
	@${RM} ${OBJECTDIR}/_ext/441138584/startup.o.d 
	@${RM} ${OBJECTDIR}/_ext/441138584/startup.o 
	@${RM} ${OBJECTDIR}/_ext/441138584/startup.o.ok ${OBJECTDIR}/_ext/441138584/startup.o.err 
	${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/441138584/startup.o.d"  -o ${OBJECTDIR}/_ext/441138584/startup.o ../../../devices/microchip/pic32mz-ef/startup.S  -DXPRJ_default=$(CND_CONF)    -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/_ext/441138584/startup.o.asm.d",--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1,-I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" 
	@${FIXDEPS} "${OBJECTDIR}/_ext/441138584/startup.o.d" "${OBJECTDIR}/_ext/441138584/startup.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/_ext/1472/config.o: ../config.S  .generated_files/flags/default/bb8b65bb73fc17255d3f6ec803e0038e662df0dc .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/config.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/config.o 
	@${RM} ${OBJECTDIR}/_ext/1472/config.o.ok ${OBJECTDIR}/_ext/1472/config.o.err 
	${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/config.o.d"  -o ${OBJECTDIR}/_ext/1472/config.o ../config.S  -DXPRJ_default=$(CND_CONF)    -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/_ext/1472/config.o.asm.d",--gdwarf-2,-I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/config.o.d" "${OBJECTDIR}/_ext/1472/config.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o: ../../../arch/mips32/m51xx/picoRTOS_portasm.S  .generated_files/flags/default/92221eebe5831e1a775a3af9124a99f2d7424549 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1430854306" 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.ok ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.err 
	${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.d"  -o ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o ../../../arch/mips32/m51xx/picoRTOS_portasm.S  -DXPRJ_default=$(CND_CONF)    -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.asm.d",--gdwarf-2,-I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.d" "${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/441138584/startup.o: ../../../devices/microchip/pic32mz-ef/startup.S  .generated_files/flags/default/6dee348be72d36ca4446b7d4a8f397d650d1156c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/441138584" 
	@${RM} ${OBJECTDIR}/_ext/441138584/startup.o.d 
	@${RM} ${OBJECTDIR}/_ext/441138584/startup.o 
	@${RM} ${OBJECTDIR}/_ext/441138584/startup.o.ok ${OBJECTDIR}/_ext/441138584/startup.o.err 
	${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/441138584/startup.o.d"  -o ${OBJECTDIR}/_ext/441138584/startup.o ../../../devices/microchip/pic32mz-ef/startup.S  -DXPRJ_default=$(CND_CONF)    -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/_ext/441138584/startup.o.asm.d",--gdwarf-2,-I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" 
	@${FIXDEPS} "${OBJECTDIR}/_ext/441138584/startup.o.d" "${OBJECTDIR}/_ext/441138584/startup.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o: ../curiosity-2.0-pic32mz-ef.c  .generated_files/flags/default/906cc74970b893dc5acfa45e321dc58b28a611c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o.d" -o ${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o ../curiosity-2.0-pic32mz-ef.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  .generated_files/flags/default/7d086f81c4d04a8b11d2609127c9db132d56b2eb .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d" -o ${OBJECTDIR}/_ext/1472/main.o ../main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o: ../../../drivers/clock/clock-pic32mx.c  .generated_files/flags/default/510ccb15d673c9765d43e3b8837cf5b8d7a1772e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1343658459" 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o ../../../drivers/clock/clock-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o: ../../../drivers/mux/mux-pic32mx.c  .generated_files/flags/default/a216bd4a3645f5e6ec5eab2046e59da1b211e8b4 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462858973" 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o ../../../drivers/mux/mux-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o: ../../../drivers/gpio/gpio-pic32mx.c  .generated_files/flags/default/8869abd306b100fb24e80888b5bc41edb998bc59 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1896195998" 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o ../../../drivers/gpio/gpio-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o: ../../../drivers/uart/uart-pic32mx.c  .generated_files/flags/default/2252e6ac5a46667876de2db2006bae2cf3f7626b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1895793055" 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o ../../../drivers/uart/uart-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o: ../../../drivers/misc/misc-pic32mx.c  .generated_files/flags/default/8483a89ccf01e6a570c7783b9c016fcdbdfdc8a4 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1896023681" 
	@${RM} ${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o ../../../drivers/misc/misc-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o: ../../../drivers/spi/spi-pic32mx.c  .generated_files/flags/default/87c24b0428f097bfc9daad5e902e3b6717bc5dec .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462864569" 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o ../../../drivers/spi/spi-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/229905376/wd-pic32mx.o: ../../../drivers/wd/wd-pic32mx.c  .generated_files/flags/default/9c08c035d700123c5516fb46c88400f71d77a631 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/229905376" 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/229905376/wd-pic32mx.o.d" -o ${OBJECTDIR}/_ext/229905376/wd-pic32mx.o ../../../drivers/wd/wd-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o: ../../../drivers/pwm/pwm-pic32mx.c  .generated_files/flags/default/c81d441d5ae9f995eb827e9da91069152ba084ee .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462861907" 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o ../../../drivers/pwm/pwm-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o: ../../../drivers/twi/twi-pic32mx.c  .generated_files/flags/default/2497e524ff1190c97a8ee161d123ea279826308a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462865747" 
	@${RM} ${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o ../../../drivers/twi/twi-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o: ../../../drivers/adc/adc-pic32mx.c  .generated_files/flags/default/275ebd6e8e93757333b293b501866eb6454a94b8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462846893" 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o ../../../drivers/adc/adc-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1462848733/can-pic32mx.o: ../../../drivers/can/can-pic32mx.c  .generated_files/flags/default/984333c1fcf085180772bb61e4a845c8b6742be2 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462848733" 
	@${RM} ${OBJECTDIR}/_ext/1462848733/can-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462848733/can-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462848733/can-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462848733/can-pic32mx.o ../../../drivers/can/can-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o: ../../../arch/mips32/m51xx/picoRTOS_port.c  .generated_files/flags/default/f1d699afcde8bf841aa7bec70cac4b93c8d99a6d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1430854306" 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o.d" -o ${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o ../../../arch/mips32/m51xx/picoRTOS_port.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o: ../../../ipc/picoRTOS_cond.c  .generated_files/flags/default/19a9151126f0cba95be927a8c9b27ed2bb54d6bb .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1386518765" 
	@${RM} ${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o.d" -o ${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o ../../../ipc/picoRTOS_cond.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o: ../../../ipc/picoRTOS_mutex.c  .generated_files/flags/default/feab0a63f749e228f31384c15df0ce5124cc75ab .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1386518765" 
	@${RM} ${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o.d" -o ${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o ../../../ipc/picoRTOS_mutex.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/2124829536/picoRTOS.o: ../../../picoRTOS.c  .generated_files/flags/default/ed23edfb83f8b0a4724a33ad3c71da5754b8eb08 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2124829536" 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d" -o ${OBJECTDIR}/_ext/2124829536/picoRTOS.o ../../../picoRTOS.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
else
${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o: ../curiosity-2.0-pic32mz-ef.c  .generated_files/flags/default/554d744a19a403bb83b475ab0c6b5e08ca60fcfa .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o.d" -o ${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o ../curiosity-2.0-pic32mz-ef.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  .generated_files/flags/default/c5d497582f5fc63dd1f80dfcee32ddc18fb6539 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d" -o ${OBJECTDIR}/_ext/1472/main.o ../main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o: ../../../drivers/clock/clock-pic32mx.c  .generated_files/flags/default/d90f6b4b9f64d962aeb6491e43483d0fb50a7bff .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1343658459" 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o ../../../drivers/clock/clock-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o: ../../../drivers/mux/mux-pic32mx.c  .generated_files/flags/default/63cd4f5e9ac6fbe4ea9687a35c34011a703cc538 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462858973" 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o ../../../drivers/mux/mux-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o: ../../../drivers/gpio/gpio-pic32mx.c  .generated_files/flags/default/c2388b9d0fa1a11209199b12a0b11fbc085535fe .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1896195998" 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o ../../../drivers/gpio/gpio-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o: ../../../drivers/uart/uart-pic32mx.c  .generated_files/flags/default/c0aeac5a7929453bab39a8c2e0420b538e4ad17e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1895793055" 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o ../../../drivers/uart/uart-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o: ../../../drivers/misc/misc-pic32mx.c  .generated_files/flags/default/956f495dddcec8d20c837a139b967ddb9fb0e58 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1896023681" 
	@${RM} ${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o ../../../drivers/misc/misc-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o: ../../../drivers/spi/spi-pic32mx.c  .generated_files/flags/default/60f8ae3c32f7e9a7af4a7669082e00bcaf450568 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462864569" 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o ../../../drivers/spi/spi-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/229905376/wd-pic32mx.o: ../../../drivers/wd/wd-pic32mx.c  .generated_files/flags/default/afdda79838f0d12328e34a5baf552d4372239fa1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/229905376" 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/229905376/wd-pic32mx.o.d" -o ${OBJECTDIR}/_ext/229905376/wd-pic32mx.o ../../../drivers/wd/wd-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o: ../../../drivers/pwm/pwm-pic32mx.c  .generated_files/flags/default/1a5a2726dd2ab98f4153e5c060b7cbd151304a92 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462861907" 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o ../../../drivers/pwm/pwm-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o: ../../../drivers/twi/twi-pic32mx.c  .generated_files/flags/default/c8ee6b403ae832661a88eceafe7ef04cf0e10079 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462865747" 
	@${RM} ${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o ../../../drivers/twi/twi-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o: ../../../drivers/adc/adc-pic32mx.c  .generated_files/flags/default/a6d26f32403861a91c2e527a5432ed818de22438 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462846893" 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o ../../../drivers/adc/adc-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1462848733/can-pic32mx.o: ../../../drivers/can/can-pic32mx.c  .generated_files/flags/default/8ff2225aa1ca68e16fc138fb112d9bcc00331dc9 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462848733" 
	@${RM} ${OBJECTDIR}/_ext/1462848733/can-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462848733/can-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462848733/can-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462848733/can-pic32mx.o ../../../drivers/can/can-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o: ../../../arch/mips32/m51xx/picoRTOS_port.c  .generated_files/flags/default/f01812495d325c5ecf949af3d7f54d2b486cfced .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1430854306" 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o.d" -o ${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o ../../../arch/mips32/m51xx/picoRTOS_port.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o: ../../../ipc/picoRTOS_cond.c  .generated_files/flags/default/24704d9e3a54f0dceed7323b516d46a6827b98b8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1386518765" 
	@${RM} ${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o.d" -o ${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o ../../../ipc/picoRTOS_cond.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o: ../../../ipc/picoRTOS_mutex.c  .generated_files/flags/default/812eb0bf11bcd4ab99db2936c194e74569f6279e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1386518765" 
	@${RM} ${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o.d" -o ${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o ../../../ipc/picoRTOS_mutex.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/2124829536/picoRTOS.o: ../../../picoRTOS.c  .generated_files/flags/default/d00043840801c1480aaf2a90343e0227b96fabd8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2124829536" 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d" -o ${OBJECTDIR}/_ext/2124829536/picoRTOS.o ../../../picoRTOS.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/curiosity-2.0-pic32mz-ef.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    ../../../devices/microchip/pic32mz-ef/gcc/pic32mz-efm_2048_ram.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g   -mprocessor=$(MP_PROCESSOR_OPTION) -nostdlib -nostartfiles -o ${DISTDIR}/curiosity-2.0-pic32mz-ef.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)   -mreserve=data@0x0:0x37F   -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml 
	
else
${DISTDIR}/curiosity-2.0-pic32mz-ef.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   ../../../devices/microchip/pic32mz-ef/gcc/pic32mz-efm_2048_ram.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION) -nostdlib -nostartfiles -o ${DISTDIR}/curiosity-2.0-pic32mz-ef.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml 
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
