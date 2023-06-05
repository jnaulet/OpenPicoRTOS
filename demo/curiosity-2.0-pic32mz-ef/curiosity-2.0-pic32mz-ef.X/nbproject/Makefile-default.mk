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
SOURCEFILES_QUOTED_IF_SPACED=../curiosity-2.0-pic32mz-ef.c ../main.c ../config.S ../../../drivers/clock/clock-pic32mx.c ../../../drivers/mux/mux-pic32mx.c ../../../drivers/gpio/gpio-pic32mx.c ../../../drivers/uart/uart-pic32mx.c ../../../drivers/misc/misc-pic32mx.c ../../../drivers/spi/spi-pic32mx.c ../../../drivers/wd/wd-pic32mx.c ../../../drivers/pwm/pwm-pic32mx.c ../../../drivers/twi/twi-pic32mx.c ../../../drivers/adc/adc-pic32mx.c ../../../staging/drivers/can/can-pic32mx.c ../../../drivers/flash/flash-pic32mx.c ../../../arch/mips32/m51xx/picoRTOS_port.c ../../../arch/mips32/m51xx/picoRTOS_portasm.S ../../../arch/mips32/common/intc-pic32mx.c ../../../ipc/picoRTOS_cond.c ../../../ipc/picoRTOS_mutex.c ../../../picoRTOS.c ../../../devices/microchip/pic32mz-ef/startup.S

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1472/config.o ${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o ${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o ${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o ${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o ${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o ${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o ${OBJECTDIR}/_ext/229905376/wd-pic32mx.o ${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o ${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o ${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o ${OBJECTDIR}/_ext/481107959/can-pic32mx.o ${OBJECTDIR}/_ext/1346416061/flash-pic32mx.o ${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o ${OBJECTDIR}/_ext/1175859954/intc-pic32mx.o ${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o ${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o ${OBJECTDIR}/_ext/2124829536/picoRTOS.o ${OBJECTDIR}/_ext/441138584/startup.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o.d ${OBJECTDIR}/_ext/1472/main.o.d ${OBJECTDIR}/_ext/1472/config.o.d ${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o.d ${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o.d ${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o.d ${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o.d ${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o.d ${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o.d ${OBJECTDIR}/_ext/229905376/wd-pic32mx.o.d ${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o.d ${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o.d ${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o.d ${OBJECTDIR}/_ext/481107959/can-pic32mx.o.d ${OBJECTDIR}/_ext/1346416061/flash-pic32mx.o.d ${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o.d ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.d ${OBJECTDIR}/_ext/1175859954/intc-pic32mx.o.d ${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o.d ${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o.d ${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d ${OBJECTDIR}/_ext/441138584/startup.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1472/config.o ${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o ${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o ${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o ${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o ${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o ${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o ${OBJECTDIR}/_ext/229905376/wd-pic32mx.o ${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o ${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o ${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o ${OBJECTDIR}/_ext/481107959/can-pic32mx.o ${OBJECTDIR}/_ext/1346416061/flash-pic32mx.o ${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o ${OBJECTDIR}/_ext/1175859954/intc-pic32mx.o ${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o ${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o ${OBJECTDIR}/_ext/2124829536/picoRTOS.o ${OBJECTDIR}/_ext/441138584/startup.o

# Source Files
SOURCEFILES=../curiosity-2.0-pic32mz-ef.c ../main.c ../config.S ../../../drivers/clock/clock-pic32mx.c ../../../drivers/mux/mux-pic32mx.c ../../../drivers/gpio/gpio-pic32mx.c ../../../drivers/uart/uart-pic32mx.c ../../../drivers/misc/misc-pic32mx.c ../../../drivers/spi/spi-pic32mx.c ../../../drivers/wd/wd-pic32mx.c ../../../drivers/pwm/pwm-pic32mx.c ../../../drivers/twi/twi-pic32mx.c ../../../drivers/adc/adc-pic32mx.c ../../../staging/drivers/can/can-pic32mx.c ../../../drivers/flash/flash-pic32mx.c ../../../arch/mips32/m51xx/picoRTOS_port.c ../../../arch/mips32/m51xx/picoRTOS_portasm.S ../../../arch/mips32/common/intc-pic32mx.c ../../../ipc/picoRTOS_cond.c ../../../ipc/picoRTOS_mutex.c ../../../picoRTOS.c ../../../devices/microchip/pic32mz-ef/startup.S



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
${OBJECTDIR}/_ext/1472/config.o: ../config.S  .generated_files/flags/default/23b9a1c7770b590d702f5d7d6fed780410b2ac1d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/config.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/config.o 
	@${RM} ${OBJECTDIR}/_ext/1472/config.o.ok ${OBJECTDIR}/_ext/1472/config.o.err 
	${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/config.o.d"  -o ${OBJECTDIR}/_ext/1472/config.o ../config.S  -DXPRJ_default=$(CND_CONF)    -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/_ext/1472/config.o.asm.d",--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1,-I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -I"../../../drivers/flash" 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/config.o.d" "${OBJECTDIR}/_ext/1472/config.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o: ../../../arch/mips32/m51xx/picoRTOS_portasm.S  .generated_files/flags/default/c9a0903d6159fb77f998d6820eff2139a2a994a8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1430854306" 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.ok ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.err 
	${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.d"  -o ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o ../../../arch/mips32/m51xx/picoRTOS_portasm.S  -DXPRJ_default=$(CND_CONF)    -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.asm.d",--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1,-I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -I"../../../drivers/flash" 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.d" "${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/441138584/startup.o: ../../../devices/microchip/pic32mz-ef/startup.S  .generated_files/flags/default/12904bfe36f7fd02a5c24dc6e30847b6d94b7b51 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/441138584" 
	@${RM} ${OBJECTDIR}/_ext/441138584/startup.o.d 
	@${RM} ${OBJECTDIR}/_ext/441138584/startup.o 
	@${RM} ${OBJECTDIR}/_ext/441138584/startup.o.ok ${OBJECTDIR}/_ext/441138584/startup.o.err 
	${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/441138584/startup.o.d"  -o ${OBJECTDIR}/_ext/441138584/startup.o ../../../devices/microchip/pic32mz-ef/startup.S  -DXPRJ_default=$(CND_CONF)    -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/_ext/441138584/startup.o.asm.d",--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1,-I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -I"../../../drivers/flash" 
	@${FIXDEPS} "${OBJECTDIR}/_ext/441138584/startup.o.d" "${OBJECTDIR}/_ext/441138584/startup.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/_ext/1472/config.o: ../config.S  .generated_files/flags/default/41c7439e02a12a4887126adf825a0205204e2c01 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/config.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/config.o 
	@${RM} ${OBJECTDIR}/_ext/1472/config.o.ok ${OBJECTDIR}/_ext/1472/config.o.err 
	${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/config.o.d"  -o ${OBJECTDIR}/_ext/1472/config.o ../config.S  -DXPRJ_default=$(CND_CONF)    -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/_ext/1472/config.o.asm.d",--gdwarf-2,-I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -I"../../../drivers/flash" 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/config.o.d" "${OBJECTDIR}/_ext/1472/config.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o: ../../../arch/mips32/m51xx/picoRTOS_portasm.S  .generated_files/flags/default/50be908f8a77679898d7dca513048f02e8fd33b2 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1430854306" 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.ok ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.err 
	${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.d"  -o ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o ../../../arch/mips32/m51xx/picoRTOS_portasm.S  -DXPRJ_default=$(CND_CONF)    -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.asm.d",--gdwarf-2,-I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -I"../../../drivers/flash" 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.d" "${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/441138584/startup.o: ../../../devices/microchip/pic32mz-ef/startup.S  .generated_files/flags/default/50dca33a87615d3a99c66b0b09a785d498a32412 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/441138584" 
	@${RM} ${OBJECTDIR}/_ext/441138584/startup.o.d 
	@${RM} ${OBJECTDIR}/_ext/441138584/startup.o 
	@${RM} ${OBJECTDIR}/_ext/441138584/startup.o.ok ${OBJECTDIR}/_ext/441138584/startup.o.err 
	${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/441138584/startup.o.d"  -o ${OBJECTDIR}/_ext/441138584/startup.o ../../../devices/microchip/pic32mz-ef/startup.S  -DXPRJ_default=$(CND_CONF)    -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/_ext/441138584/startup.o.asm.d",--gdwarf-2,-I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -I"../../../drivers/flash" 
	@${FIXDEPS} "${OBJECTDIR}/_ext/441138584/startup.o.d" "${OBJECTDIR}/_ext/441138584/startup.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o: ../curiosity-2.0-pic32mz-ef.c  .generated_files/flags/default/14473445c36afbb991603789c81558178025ebda .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -I"../../../drivers/flash" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o.d" -o ${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o ../curiosity-2.0-pic32mz-ef.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  .generated_files/flags/default/80c739fe8e5003b6a9dc7dbbf81f9401271c2d9e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -I"../../../drivers/flash" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d" -o ${OBJECTDIR}/_ext/1472/main.o ../main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o: ../../../drivers/clock/clock-pic32mx.c  .generated_files/flags/default/8f6bf3b30eb53e9f43bfcd365e95f4a9a5ad2ad8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1343658459" 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -I"../../../drivers/flash" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o ../../../drivers/clock/clock-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o: ../../../drivers/mux/mux-pic32mx.c  .generated_files/flags/default/59f21e1b5978403f60149ad920f2d92d41c8cc8d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462858973" 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -I"../../../drivers/flash" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o ../../../drivers/mux/mux-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o: ../../../drivers/gpio/gpio-pic32mx.c  .generated_files/flags/default/8a2458eb05049390c8b9bea1293a26ee876e9d47 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1896195998" 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -I"../../../drivers/flash" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o ../../../drivers/gpio/gpio-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o: ../../../drivers/uart/uart-pic32mx.c  .generated_files/flags/default/7307aff92b94f953de3cee4dfffd5010497de516 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1895793055" 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -I"../../../drivers/flash" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o ../../../drivers/uart/uart-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o: ../../../drivers/misc/misc-pic32mx.c  .generated_files/flags/default/9cade245dc35022ede5d52f7a14cd9f6f76cf120 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1896023681" 
	@${RM} ${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -I"../../../drivers/flash" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o ../../../drivers/misc/misc-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o: ../../../drivers/spi/spi-pic32mx.c  .generated_files/flags/default/6c15cef9b750c7937e1c94b81ba001d612024241 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462864569" 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -I"../../../drivers/flash" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o ../../../drivers/spi/spi-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/229905376/wd-pic32mx.o: ../../../drivers/wd/wd-pic32mx.c  .generated_files/flags/default/dc4c1577f9b1d9f71724b18dcdb34f5b127ee772 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/229905376" 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -I"../../../drivers/flash" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/229905376/wd-pic32mx.o.d" -o ${OBJECTDIR}/_ext/229905376/wd-pic32mx.o ../../../drivers/wd/wd-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o: ../../../drivers/pwm/pwm-pic32mx.c  .generated_files/flags/default/2a3a6689bd9d1291ca2b357642f9432afd135432 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462861907" 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -I"../../../drivers/flash" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o ../../../drivers/pwm/pwm-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o: ../../../drivers/twi/twi-pic32mx.c  .generated_files/flags/default/30b5d551e4ae3d78622ffbad609ea44652faab2 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462865747" 
	@${RM} ${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -I"../../../drivers/flash" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o ../../../drivers/twi/twi-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o: ../../../drivers/adc/adc-pic32mx.c  .generated_files/flags/default/157fa1a7ab64d55f548c0b6fd321cfd6ca0c1de7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462846893" 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -I"../../../drivers/flash" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o ../../../drivers/adc/adc-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/481107959/can-pic32mx.o: ../../../staging/drivers/can/can-pic32mx.c  .generated_files/flags/default/a0263d70cbe0c1b1d27dc1bd75eaf26ac66966ed .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/481107959" 
	@${RM} ${OBJECTDIR}/_ext/481107959/can-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/481107959/can-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -I"../../../drivers/flash" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/481107959/can-pic32mx.o.d" -o ${OBJECTDIR}/_ext/481107959/can-pic32mx.o ../../../staging/drivers/can/can-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1346416061/flash-pic32mx.o: ../../../drivers/flash/flash-pic32mx.c  .generated_files/flags/default/9124d8e0888bbcba244c4dc5c052a20cc102f0fe .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1346416061" 
	@${RM} ${OBJECTDIR}/_ext/1346416061/flash-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1346416061/flash-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -I"../../../drivers/flash" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1346416061/flash-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1346416061/flash-pic32mx.o ../../../drivers/flash/flash-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o: ../../../arch/mips32/m51xx/picoRTOS_port.c  .generated_files/flags/default/701d993fdac7eba4fb90a04887af5efc3ac000 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1430854306" 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -I"../../../drivers/flash" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o.d" -o ${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o ../../../arch/mips32/m51xx/picoRTOS_port.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1175859954/intc-pic32mx.o: ../../../arch/mips32/common/intc-pic32mx.c  .generated_files/flags/default/e84753426197ca6eef0106d7952edcd7447616e8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1175859954" 
	@${RM} ${OBJECTDIR}/_ext/1175859954/intc-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1175859954/intc-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -I"../../../drivers/flash" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1175859954/intc-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1175859954/intc-pic32mx.o ../../../arch/mips32/common/intc-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o: ../../../ipc/picoRTOS_cond.c  .generated_files/flags/default/92d52a8a639813b9d0c2bfa8e8435a9a7c7b33ec .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1386518765" 
	@${RM} ${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -I"../../../drivers/flash" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o.d" -o ${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o ../../../ipc/picoRTOS_cond.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o: ../../../ipc/picoRTOS_mutex.c  .generated_files/flags/default/b22456929c95b6c41160e8d8abc923628a85a05a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1386518765" 
	@${RM} ${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -I"../../../drivers/flash" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o.d" -o ${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o ../../../ipc/picoRTOS_mutex.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/2124829536/picoRTOS.o: ../../../picoRTOS.c  .generated_files/flags/default/6ab3838a0643cafb3d13e34d5aa06f28495d07bc .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2124829536" 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -I"../../../drivers/flash" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d" -o ${OBJECTDIR}/_ext/2124829536/picoRTOS.o ../../../picoRTOS.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
else
${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o: ../curiosity-2.0-pic32mz-ef.c  .generated_files/flags/default/9a1b5c23bdabeaa6836b755b4eb003c9811727e7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -I"../../../drivers/flash" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o.d" -o ${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o ../curiosity-2.0-pic32mz-ef.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  .generated_files/flags/default/55ee4baaf8947c880e2a2199ac0f7bb696dba414 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -I"../../../drivers/flash" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d" -o ${OBJECTDIR}/_ext/1472/main.o ../main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o: ../../../drivers/clock/clock-pic32mx.c  .generated_files/flags/default/2d15970c2b8885bc47414e8855e8b0157f16629b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1343658459" 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -I"../../../drivers/flash" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o ../../../drivers/clock/clock-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o: ../../../drivers/mux/mux-pic32mx.c  .generated_files/flags/default/71e69bd00ee39761838d4f2f7bbdebefa530fc23 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462858973" 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -I"../../../drivers/flash" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o ../../../drivers/mux/mux-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o: ../../../drivers/gpio/gpio-pic32mx.c  .generated_files/flags/default/ec551a1533ff1197f32bfe43226c6294f9db34af .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1896195998" 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -I"../../../drivers/flash" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o ../../../drivers/gpio/gpio-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o: ../../../drivers/uart/uart-pic32mx.c  .generated_files/flags/default/23c5e12dce196f3361de2fbda3c5ff5b4cee5267 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1895793055" 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -I"../../../drivers/flash" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o ../../../drivers/uart/uart-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o: ../../../drivers/misc/misc-pic32mx.c  .generated_files/flags/default/c032e6777ce881bd08138da2517d5dfcba7d9558 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1896023681" 
	@${RM} ${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -I"../../../drivers/flash" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o ../../../drivers/misc/misc-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o: ../../../drivers/spi/spi-pic32mx.c  .generated_files/flags/default/3abb3d131284b636fd29b9d8d5df67c410a08e57 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462864569" 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -I"../../../drivers/flash" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o ../../../drivers/spi/spi-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/229905376/wd-pic32mx.o: ../../../drivers/wd/wd-pic32mx.c  .generated_files/flags/default/f8d31acb39199d8b41bc54eaee7bdea4a26b609b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/229905376" 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -I"../../../drivers/flash" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/229905376/wd-pic32mx.o.d" -o ${OBJECTDIR}/_ext/229905376/wd-pic32mx.o ../../../drivers/wd/wd-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o: ../../../drivers/pwm/pwm-pic32mx.c  .generated_files/flags/default/559183d0cc75f803039998eab1a417371a2a2829 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462861907" 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -I"../../../drivers/flash" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o ../../../drivers/pwm/pwm-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o: ../../../drivers/twi/twi-pic32mx.c  .generated_files/flags/default/84dbe482429406c66ecb52e31725ed58cc6ee4da .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462865747" 
	@${RM} ${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -I"../../../drivers/flash" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462865747/twi-pic32mx.o ../../../drivers/twi/twi-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o: ../../../drivers/adc/adc-pic32mx.c  .generated_files/flags/default/c935698a27b9c3280329c56a10e8dcf2bc7438b5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462846893" 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -I"../../../drivers/flash" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462846893/adc-pic32mx.o ../../../drivers/adc/adc-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/481107959/can-pic32mx.o: ../../../staging/drivers/can/can-pic32mx.c  .generated_files/flags/default/141ed7117917ce4a3908c7fcfec3466fbb1f0690 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/481107959" 
	@${RM} ${OBJECTDIR}/_ext/481107959/can-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/481107959/can-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -I"../../../drivers/flash" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/481107959/can-pic32mx.o.d" -o ${OBJECTDIR}/_ext/481107959/can-pic32mx.o ../../../staging/drivers/can/can-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1346416061/flash-pic32mx.o: ../../../drivers/flash/flash-pic32mx.c  .generated_files/flags/default/c4e091cd4194a24f876edb39882deeade72453a8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1346416061" 
	@${RM} ${OBJECTDIR}/_ext/1346416061/flash-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1346416061/flash-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -I"../../../drivers/flash" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1346416061/flash-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1346416061/flash-pic32mx.o ../../../drivers/flash/flash-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o: ../../../arch/mips32/m51xx/picoRTOS_port.c  .generated_files/flags/default/7bae9d468cc1a40f2f2dad09ed5ba1564db3696a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1430854306" 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -I"../../../drivers/flash" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o.d" -o ${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o ../../../arch/mips32/m51xx/picoRTOS_port.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1175859954/intc-pic32mx.o: ../../../arch/mips32/common/intc-pic32mx.c  .generated_files/flags/default/e9ddf469738cafac4052eb7cba199851db5cd300 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1175859954" 
	@${RM} ${OBJECTDIR}/_ext/1175859954/intc-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1175859954/intc-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -I"../../../drivers/flash" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1175859954/intc-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1175859954/intc-pic32mx.o ../../../arch/mips32/common/intc-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o: ../../../ipc/picoRTOS_cond.c  .generated_files/flags/default/daf4477fe2217dd7f47c58f3d1d29509ca82280 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1386518765" 
	@${RM} ${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -I"../../../drivers/flash" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o.d" -o ${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o ../../../ipc/picoRTOS_cond.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o: ../../../ipc/picoRTOS_mutex.c  .generated_files/flags/default/49d1a0aeace2c9bc5e6221a43118ed3f3de28df2 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1386518765" 
	@${RM} ${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -I"../../../drivers/flash" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o.d" -o ${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o ../../../ipc/picoRTOS_mutex.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/2124829536/picoRTOS.o: ../../../picoRTOS.c  .generated_files/flags/default/215f1c36bafc66239545c27677d3405f5b94d7b9 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2124829536" 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -I"../../../drivers/twi" -I"../../../drivers/adc" -I"../../../staging/drivers/can" -I"../../../drivers/flash" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d" -o ${OBJECTDIR}/_ext/2124829536/picoRTOS.o ../../../picoRTOS.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
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
