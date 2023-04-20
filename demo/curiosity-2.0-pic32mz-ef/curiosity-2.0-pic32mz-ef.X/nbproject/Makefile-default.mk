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
SOURCEFILES_QUOTED_IF_SPACED=../curiosity-2.0-pic32mz-ef.c ../main.c ../config.S ../../../drivers/clock/clock-pic32mx.c ../../../drivers/mux/mux-pic32mx.c ../../../drivers/gpio/gpio-pic32mx.c ../../../drivers/uart/uart-pic32mx.c ../../../drivers/misc/misc-pic32mx.c ../../../drivers/spi/spi-pic32mx.c ../../../drivers/wd/wd-pic32mx.c ../../../drivers/pwm/pwm-pic32mx.c ../../../drivers/twi/twi-pic32mx.c ../../../drivers/adc/adc-pic32mx.c ../../../arch/mips32/m51xx/picoRTOS_port.c ../../../arch/mips32/m51xx/picoRTOS_portasm.S ../../../ipc/picoRTOS_cond.c ../../../ipc/picoRTOS_mutex.c ../../../picoRTOS.c ../../../devices/microchip/pic32mz-ef/startup.S ../../../staging/drivers/can/can-pic32mx.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1472/config.o ${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o ${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o ${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o ${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o ${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o ${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o ${OBJECTDIR}/_ext/229905376/wd-pic32mx.o ${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o ${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o ${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o ${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o ${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o ${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o ${OBJECTDIR}/_ext/2124829536/picoRTOS.o ${OBJECTDIR}/_ext/441138584/startup.o ${OBJECTDIR}/_ext/481107959/can-pic32mx.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o.d ${OBJECTDIR}/_ext/1472/main.o.d ${OBJECTDIR}/_ext/1472/config.o.d ${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o.d ${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o.d ${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o.d ${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o.d ${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o.d ${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o.d ${OBJECTDIR}/_ext/229905376/wd-pic32mx.o.d ${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o.d ${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o.d ${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o.d ${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o.d ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.d ${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o.d ${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o.d ${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d ${OBJECTDIR}/_ext/441138584/startup.o.d ${OBJECTDIR}/_ext/481107959/can-pic32mx.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1472/config.o ${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o ${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o ${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o ${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o ${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o ${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o ${OBJECTDIR}/_ext/229905376/wd-pic32mx.o ${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o ${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o ${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o ${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o ${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o ${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o ${OBJECTDIR}/_ext/2124829536/picoRTOS.o ${OBJECTDIR}/_ext/441138584/startup.o ${OBJECTDIR}/_ext/481107959/can-pic32mx.o

# Source Files
SOURCEFILES=../curiosity-2.0-pic32mz-ef.c ../main.c ../config.S ../../../drivers/clock/clock-pic32mx.c ../../../drivers/mux/mux-pic32mx.c ../../../drivers/gpio/gpio-pic32mx.c ../../../drivers/uart/uart-pic32mx.c ../../../drivers/misc/misc-pic32mx.c ../../../drivers/spi/spi-pic32mx.c ../../../drivers/wd/wd-pic32mx.c ../../../drivers/pwm/pwm-pic32mx.c ../../../drivers/twi/twi-pic32mx.c ../../../drivers/adc/adc-pic32mx.c ../../../arch/mips32/m51xx/picoRTOS_port.c ../../../arch/mips32/m51xx/picoRTOS_portasm.S ../../../ipc/picoRTOS_cond.c ../../../ipc/picoRTOS_mutex.c ../../../picoRTOS.c ../../../devices/microchip/pic32mz-ef/startup.S ../../../staging/drivers/can/can-pic32mx.c



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
${OBJECTDIR}/_ext/1472/config.o: ../config.S  .generated_files/flags/default/52881bad187716eacdb7d5ed847833f163f3e8f3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/config.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/config.o 
	@${RM} ${OBJECTDIR}/_ext/1472/config.o.ok ${OBJECTDIR}/_ext/1472/config.o.err 
	${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/config.o.d"  -o ${OBJECTDIR}/_ext/1472/config.o ../config.S  -DXPRJ_default=$(CND_CONF)    -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/_ext/1472/config.o.asm.d",--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1,-I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/config.o.d" "${OBJECTDIR}/_ext/1472/config.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o: ../../../arch/mips32/m51xx/picoRTOS_portasm.S  .generated_files/flags/default/f47c073e70f8129a9cff133d1b2a12d4db80ecd5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1430854306" 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.ok ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.err 
	${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.d"  -o ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o ../../../arch/mips32/m51xx/picoRTOS_portasm.S  -DXPRJ_default=$(CND_CONF)    -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.asm.d",--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1,-I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.d" "${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/441138584/startup.o: ../../../devices/microchip/pic32mz-ef/startup.S  .generated_files/flags/default/ed33462ab06559cb2f60bc36bbd6e946bb67c38f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/441138584" 
	@${RM} ${OBJECTDIR}/_ext/441138584/startup.o.d 
	@${RM} ${OBJECTDIR}/_ext/441138584/startup.o 
	@${RM} ${OBJECTDIR}/_ext/441138584/startup.o.ok ${OBJECTDIR}/_ext/441138584/startup.o.err 
	${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/441138584/startup.o.d"  -o ${OBJECTDIR}/_ext/441138584/startup.o ../../../devices/microchip/pic32mz-ef/startup.S  -DXPRJ_default=$(CND_CONF)    -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/_ext/441138584/startup.o.asm.d",--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1,-I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" 
	@${FIXDEPS} "${OBJECTDIR}/_ext/441138584/startup.o.d" "${OBJECTDIR}/_ext/441138584/startup.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/_ext/1472/config.o: ../config.S  .generated_files/flags/default/87803c20e66dccd5dcd3ccd8e62b4c78421dff3a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/config.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/config.o 
	@${RM} ${OBJECTDIR}/_ext/1472/config.o.ok ${OBJECTDIR}/_ext/1472/config.o.err 
	${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/config.o.d"  -o ${OBJECTDIR}/_ext/1472/config.o ../config.S  -DXPRJ_default=$(CND_CONF)    -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/_ext/1472/config.o.asm.d",--gdwarf-2,-I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/config.o.d" "${OBJECTDIR}/_ext/1472/config.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o: ../../../arch/mips32/m51xx/picoRTOS_portasm.S  .generated_files/flags/default/de30df54bc77f8c3d87d827c18288b82e139b355 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1430854306" 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.ok ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.err 
	${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.d"  -o ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o ../../../arch/mips32/m51xx/picoRTOS_portasm.S  -DXPRJ_default=$(CND_CONF)    -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.asm.d",--gdwarf-2,-I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.d" "${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/441138584/startup.o: ../../../devices/microchip/pic32mz-ef/startup.S  .generated_files/flags/default/1104ae328d0ee64ec4e6a49629d673469b5f3018 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/441138584" 
	@${RM} ${OBJECTDIR}/_ext/441138584/startup.o.d 
	@${RM} ${OBJECTDIR}/_ext/441138584/startup.o 
	@${RM} ${OBJECTDIR}/_ext/441138584/startup.o.ok ${OBJECTDIR}/_ext/441138584/startup.o.err 
	${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/441138584/startup.o.d"  -o ${OBJECTDIR}/_ext/441138584/startup.o ../../../devices/microchip/pic32mz-ef/startup.S  -DXPRJ_default=$(CND_CONF)    -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/_ext/441138584/startup.o.asm.d",--gdwarf-2,-I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" 
	@${FIXDEPS} "${OBJECTDIR}/_ext/441138584/startup.o.d" "${OBJECTDIR}/_ext/441138584/startup.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o: ../curiosity-2.0-pic32mz-ef.c  .generated_files/flags/default/1c9e70da13ded8115b0c857de95634b408339b5c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o.d" -o ${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o ../curiosity-2.0-pic32mz-ef.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  .generated_files/flags/default/c303beec550f27828cb2990ff1996446b1d12f48 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d" -o ${OBJECTDIR}/_ext/1472/main.o ../main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o: ../../../drivers/clock/clock-pic32mx.c  .generated_files/flags/default/4bb0c3793d9d026cc91c6b73095073b28842318b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1343658459" 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o ../../../drivers/clock/clock-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o: ../../../drivers/mux/mux-pic32mx.c  .generated_files/flags/default/234b5d56bbc2b45a4d99638f024df44a3654f9cd .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462858973" 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o ../../../drivers/mux/mux-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o: ../../../drivers/gpio/gpio-pic32mx.c  .generated_files/flags/default/95c6a53325007010639b84e37fdf9b669a97bc03 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1896195998" 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o ../../../drivers/gpio/gpio-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o: ../../../drivers/uart/uart-pic32mx.c  .generated_files/flags/default/36ae0da3cff616892df7130abcb94f9ab41dcc8a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1895793055" 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o ../../../drivers/uart/uart-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o: ../../../drivers/misc/misc-pic32mx.c  .generated_files/flags/default/799408ccae18fa7161e9bdd48f69596cfeb9adf8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1896023681" 
	@${RM} ${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o ../../../drivers/misc/misc-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o: ../../../drivers/spi/spi-pic32mx.c  .generated_files/flags/default/a669306bfa72972b52b1f064940638d9ad159423 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462864569" 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o ../../../drivers/spi/spi-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/229905376/wd-pic32mx.o: ../../../drivers/wd/wd-pic32mx.c  .generated_files/flags/default/989e35e4a128ee530a699f75e1d07afbff723df7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/229905376" 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/229905376/wd-pic32mx.o.d" -o ${OBJECTDIR}/_ext/229905376/wd-pic32mx.o ../../../drivers/wd/wd-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o: ../../../drivers/pwm/pwm-pic32mx.c  .generated_files/flags/default/39e5b41ba8139b3a2c0f60a7924a623d259a281d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462861907" 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o ../../../drivers/pwm/pwm-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o: ../../../drivers/twi/twi-pic32mx.c  .generated_files/flags/default/7fa6de8a013886839071fb27d0e39892460740cf .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462865747" 
	@${RM} ${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o ../../../drivers/twi/twi-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o: ../../../drivers/adc/adc-pic32mx.c  .generated_files/flags/default/aebe8042c8ff14d55cf4adb44ea478585dbdf3c1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462846893" 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o ../../../drivers/adc/adc-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o: ../../../arch/mips32/m51xx/picoRTOS_port.c  .generated_files/flags/default/e60868642c27d6825e24f5c27b357ff02d09c13c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1430854306" 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o.d" -o ${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o ../../../arch/mips32/m51xx/picoRTOS_port.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o: ../../../ipc/picoRTOS_cond.c  .generated_files/flags/default/84958323ac3c6e487048e134b0d29fdfb149c1fd .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1386518765" 
	@${RM} ${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o.d" -o ${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o ../../../ipc/picoRTOS_cond.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o: ../../../ipc/picoRTOS_mutex.c  .generated_files/flags/default/10754c80b44593c341b5f59f42227a7dfa4ce86a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1386518765" 
	@${RM} ${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o.d" -o ${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o ../../../ipc/picoRTOS_mutex.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/2124829536/picoRTOS.o: ../../../picoRTOS.c  .generated_files/flags/default/66e68ddd804f22a0f911c149b1e350992f8e7136 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2124829536" 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d" -o ${OBJECTDIR}/_ext/2124829536/picoRTOS.o ../../../picoRTOS.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/481107959/can-pic32mx.o: ../../../staging/drivers/can/can-pic32mx.c  .generated_files/flags/default/f4168cfd80ce2e97291ebc6e0d97cffa25fb2254 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/481107959" 
	@${RM} ${OBJECTDIR}/_ext/481107959/can-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/481107959/can-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/481107959/can-pic32mx.o.d" -o ${OBJECTDIR}/_ext/481107959/can-pic32mx.o ../../../staging/drivers/can/can-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
else
${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o: ../curiosity-2.0-pic32mz-ef.c  .generated_files/flags/default/2d7da2ef76db31a411fe5ade9a54cddc55e7b67a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o.d" -o ${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o ../curiosity-2.0-pic32mz-ef.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  .generated_files/flags/default/a8f79fc0905cec3d8f7694e82d6f2ef2c4e0b935 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d" -o ${OBJECTDIR}/_ext/1472/main.o ../main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o: ../../../drivers/clock/clock-pic32mx.c  .generated_files/flags/default/2d675e7518b4d5ec95f0e03628b53f8df3eeaafa .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1343658459" 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o ../../../drivers/clock/clock-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o: ../../../drivers/mux/mux-pic32mx.c  .generated_files/flags/default/58ab0b20c2e90206269f9b8a85f259d677dea551 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462858973" 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o ../../../drivers/mux/mux-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o: ../../../drivers/gpio/gpio-pic32mx.c  .generated_files/flags/default/4f132894dac9fb4beb76c50674f4bd12640215d5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1896195998" 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o ../../../drivers/gpio/gpio-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o: ../../../drivers/uart/uart-pic32mx.c  .generated_files/flags/default/a172d4b2c2bbfb274df4fa15cd5b2b3c7aff5b08 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1895793055" 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o ../../../drivers/uart/uart-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o: ../../../drivers/misc/misc-pic32mx.c  .generated_files/flags/default/73cae9645a5a036296e4ba4dd4094f5a9f9f6f4f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1896023681" 
	@${RM} ${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o ../../../drivers/misc/misc-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o: ../../../drivers/spi/spi-pic32mx.c  .generated_files/flags/default/1c07e302d1aa27dfa7e1385214c62525b8fc588c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462864569" 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o ../../../drivers/spi/spi-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/229905376/wd-pic32mx.o: ../../../drivers/wd/wd-pic32mx.c  .generated_files/flags/default/ab2552f4e10e41202ecfa96168d4b55e7b02fc9a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/229905376" 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/229905376/wd-pic32mx.o.d" -o ${OBJECTDIR}/_ext/229905376/wd-pic32mx.o ../../../drivers/wd/wd-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o: ../../../drivers/pwm/pwm-pic32mx.c  .generated_files/flags/default/93aa4a8a31891e77ef9b48708d7a2169ac41ebf3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462861907" 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o ../../../drivers/pwm/pwm-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o: ../../../drivers/twi/twi-pic32mx.c  .generated_files/flags/default/fa76ff4a818339b0c8459654f54f4daadb203867 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462865747" 
	@${RM} ${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o ../../../drivers/twi/twi-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o: ../../../drivers/adc/adc-pic32mx.c  .generated_files/flags/default/80e0ac2b7b020327250c6f1309a26e2df0c69911 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462846893" 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o ../../../drivers/adc/adc-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o: ../../../arch/mips32/m51xx/picoRTOS_port.c  .generated_files/flags/default/ed4cf0a7d73db74df013f99497bd23c4a4fd146f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1430854306" 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o.d" -o ${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o ../../../arch/mips32/m51xx/picoRTOS_port.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o: ../../../ipc/picoRTOS_cond.c  .generated_files/flags/default/7bf3afdb923a4ba80a6c67605d3e0f2d394ad88c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1386518765" 
	@${RM} ${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o.d" -o ${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o ../../../ipc/picoRTOS_cond.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o: ../../../ipc/picoRTOS_mutex.c  .generated_files/flags/default/4d39df0a18951a58cc586864ca1506f6ca52e51c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1386518765" 
	@${RM} ${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o.d" -o ${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o ../../../ipc/picoRTOS_mutex.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/2124829536/picoRTOS.o: ../../../picoRTOS.c  .generated_files/flags/default/c2bb96581f69d7d7773bb0a0cfd9ca1712b365d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2124829536" 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d" -o ${OBJECTDIR}/_ext/2124829536/picoRTOS.o ../../../picoRTOS.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/481107959/can-pic32mx.o: ../../../staging/drivers/can/can-pic32mx.c  .generated_files/flags/default/7703b82f05eefe39cd899e6c744565c94961f20b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/481107959" 
	@${RM} ${OBJECTDIR}/_ext/481107959/can-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/481107959/can-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/481107959/can-pic32mx.o.d" -o ${OBJECTDIR}/_ext/481107959/can-pic32mx.o ../../../staging/drivers/can/can-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
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
	${MP_CC} $(MP_EXTRA_LD_PRE) -g   -mprocessor=$(MP_PROCESSOR_OPTION) -nostdlib -nostartfiles -o ${DISTDIR}/curiosity-2.0-pic32mz-ef.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)      -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml 
	
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
