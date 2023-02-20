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
SOURCEFILES_QUOTED_IF_SPACED=../curiosity-2.0-pic32mz-ef.c ../main.c ../../../drivers/clock/clock-pic32mx.c ../../../drivers/mux/mux-pic32mx.c ../../../drivers/gpio/gpio-pic32mx.c ../../../drivers/uart/uart-pic32mx.c ../../../drivers/misc/misc-pic32mx.c ../../../drivers/spi/spi-pic32mx.c ../../../drivers/wd/wd-pic32mx.c ../../../arch/mips32/m51xx/picoRTOS_port.c ../../../arch/mips32/m51xx/picoRTOS_portasm.S ../../../ipc/picoRTOS_cond.c ../../../ipc/picoRTOS_mutex.c ../../../picoRTOS.c ../../../drivers/pwm/pwm-pic32mx.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o ${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o ${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o ${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o ${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o ${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o ${OBJECTDIR}/_ext/229905376/wd-pic32mx.o ${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o ${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o ${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o ${OBJECTDIR}/_ext/2124829536/picoRTOS.o ${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o.d ${OBJECTDIR}/_ext/1472/main.o.d ${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o.d ${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o.d ${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o.d ${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o.d ${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o.d ${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o.d ${OBJECTDIR}/_ext/229905376/wd-pic32mx.o.d ${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o.d ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.d ${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o.d ${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o.d ${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d ${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o ${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o ${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o ${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o ${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o ${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o ${OBJECTDIR}/_ext/229905376/wd-pic32mx.o ${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o ${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o ${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o ${OBJECTDIR}/_ext/2124829536/picoRTOS.o ${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o

# Source Files
SOURCEFILES=../curiosity-2.0-pic32mz-ef.c ../main.c ../../../drivers/clock/clock-pic32mx.c ../../../drivers/mux/mux-pic32mx.c ../../../drivers/gpio/gpio-pic32mx.c ../../../drivers/uart/uart-pic32mx.c ../../../drivers/misc/misc-pic32mx.c ../../../drivers/spi/spi-pic32mx.c ../../../drivers/wd/wd-pic32mx.c ../../../arch/mips32/m51xx/picoRTOS_port.c ../../../arch/mips32/m51xx/picoRTOS_portasm.S ../../../ipc/picoRTOS_cond.c ../../../ipc/picoRTOS_mutex.c ../../../picoRTOS.c ../../../drivers/pwm/pwm-pic32mx.c



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
${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o: ../../../arch/mips32/m51xx/picoRTOS_portasm.S  .generated_files/flags/default/49911d997fa56a4d7893cce483d51c487d7d7206 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1430854306" 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.ok ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.err 
	${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.d"  -o ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o ../../../arch/mips32/m51xx/picoRTOS_portasm.S  -DXPRJ_default=$(CND_CONF)    -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.asm.d",--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1,-I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -mdfp="${DFP_DIR}"
	@${FIXDEPS} "${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.d" "${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o: ../../../arch/mips32/m51xx/picoRTOS_portasm.S  .generated_files/flags/default/cd283ca8323bab3ad2e9e2185ea0f11e6414b95a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1430854306" 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.ok ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.err 
	${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.d"  -o ${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o ../../../arch/mips32/m51xx/picoRTOS_portasm.S  -DXPRJ_default=$(CND_CONF)    -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.asm.d",--gdwarf-2,-I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -mdfp="${DFP_DIR}"
	@${FIXDEPS} "${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.d" "${OBJECTDIR}/_ext/1430854306/picoRTOS_portasm.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o: ../curiosity-2.0-pic32mz-ef.c  .generated_files/flags/default/5bb465fe49ef121fca1822845ce4bbcb9cefd40d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o.d" -o ${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o ../curiosity-2.0-pic32mz-ef.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  .generated_files/flags/default/4ae6e5b9786b85e1a6683d7c59c5fd15aec32d72 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d" -o ${OBJECTDIR}/_ext/1472/main.o ../main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o: ../../../drivers/clock/clock-pic32mx.c  .generated_files/flags/default/9690d09c1df57ca627381f16489f6cd1d46eefd9 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1343658459" 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o ../../../drivers/clock/clock-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o: ../../../drivers/mux/mux-pic32mx.c  .generated_files/flags/default/e88dba92354d1a9c45d957259eefc8d91c7912b3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462858973" 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o ../../../drivers/mux/mux-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o: ../../../drivers/gpio/gpio-pic32mx.c  .generated_files/flags/default/981bdb9e1abca6ff7f81cbbe0b910830f0f73373 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1896195998" 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o ../../../drivers/gpio/gpio-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o: ../../../drivers/uart/uart-pic32mx.c  .generated_files/flags/default/76b772a44a7a344bb29274436b61e21b3f79d265 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1895793055" 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o ../../../drivers/uart/uart-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o: ../../../drivers/misc/misc-pic32mx.c  .generated_files/flags/default/f67adefaf13797af9bd734a123cfbaed1355a6e3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1896023681" 
	@${RM} ${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o ../../../drivers/misc/misc-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o: ../../../drivers/spi/spi-pic32mx.c  .generated_files/flags/default/803f7260a915c2a61542e60b5cc67ba8caa344a9 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462864569" 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o ../../../drivers/spi/spi-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/229905376/wd-pic32mx.o: ../../../drivers/wd/wd-pic32mx.c  .generated_files/flags/default/1b83934eecaf9e530a70b7bb6641c95892538337 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/229905376" 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/229905376/wd-pic32mx.o.d" -o ${OBJECTDIR}/_ext/229905376/wd-pic32mx.o ../../../drivers/wd/wd-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o: ../../../arch/mips32/m51xx/picoRTOS_port.c  .generated_files/flags/default/4ef772842b09753ee1c8e0aa4d7c7f27526748ff .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1430854306" 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o.d" -o ${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o ../../../arch/mips32/m51xx/picoRTOS_port.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o: ../../../ipc/picoRTOS_cond.c  .generated_files/flags/default/5337f41ed33fd7b28b175a28dd18d23b41bb1b6f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1386518765" 
	@${RM} ${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o.d" -o ${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o ../../../ipc/picoRTOS_cond.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o: ../../../ipc/picoRTOS_mutex.c  .generated_files/flags/default/6a1f8a27db561808950b6c72f35c9622264433c1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1386518765" 
	@${RM} ${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o.d" -o ${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o ../../../ipc/picoRTOS_mutex.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/2124829536/picoRTOS.o: ../../../picoRTOS.c  .generated_files/flags/default/77be5c51486320e078282eadc1c7eeae821670ea .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2124829536" 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d" -o ${OBJECTDIR}/_ext/2124829536/picoRTOS.o ../../../picoRTOS.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o: ../../../drivers/pwm/pwm-pic32mx.c  .generated_files/flags/default/1b006c5912f561f02285e4ba2fc06b864cff49f7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462861907" 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o ../../../drivers/pwm/pwm-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
else
${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o: ../curiosity-2.0-pic32mz-ef.c  .generated_files/flags/default/7506d3d5de00f0bf9e03cd33b176397d04cafe8e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o.d" -o ${OBJECTDIR}/_ext/1472/curiosity-2.0-pic32mz-ef.o ../curiosity-2.0-pic32mz-ef.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  .generated_files/flags/default/6dd153bc94500e5614712d8c5642dca3698e1266 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d" -o ${OBJECTDIR}/_ext/1472/main.o ../main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o: ../../../drivers/clock/clock-pic32mx.c  .generated_files/flags/default/18951011bf818e924db9144da51baba966a7514b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1343658459" 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1343658459/clock-pic32mx.o ../../../drivers/clock/clock-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o: ../../../drivers/mux/mux-pic32mx.c  .generated_files/flags/default/9a00e86469009d7e9f18280db226c64b363ede10 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462858973" 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462858973/mux-pic32mx.o ../../../drivers/mux/mux-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o: ../../../drivers/gpio/gpio-pic32mx.c  .generated_files/flags/default/107c2e9faf6a6af6db0e18bfac0fade4e1aed08 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1896195998" 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1896195998/gpio-pic32mx.o ../../../drivers/gpio/gpio-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o: ../../../drivers/uart/uart-pic32mx.c  .generated_files/flags/default/25455dbaf2f479f5f3509e3ce1cda264ed13dae4 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1895793055" 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1895793055/uart-pic32mx.o ../../../drivers/uart/uart-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o: ../../../drivers/misc/misc-pic32mx.c  .generated_files/flags/default/3a865f9aeaf4165e55ead8873d30c9d66776890c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1896023681" 
	@${RM} ${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1896023681/misc-pic32mx.o ../../../drivers/misc/misc-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o: ../../../drivers/spi/spi-pic32mx.c  .generated_files/flags/default/64d229f41059423e838caf4aee4029a5e2269d79 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462864569" 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462864569/spi-pic32mx.o ../../../drivers/spi/spi-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/229905376/wd-pic32mx.o: ../../../drivers/wd/wd-pic32mx.c  .generated_files/flags/default/2e143ed19708b94cc97e056411ebbfc412fc7364 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/229905376" 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/229905376/wd-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/229905376/wd-pic32mx.o.d" -o ${OBJECTDIR}/_ext/229905376/wd-pic32mx.o ../../../drivers/wd/wd-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o: ../../../arch/mips32/m51xx/picoRTOS_port.c  .generated_files/flags/default/630b83ff05168187dcf30658e2f59c7df2661e57 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1430854306" 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o.d" -o ${OBJECTDIR}/_ext/1430854306/picoRTOS_port.o ../../../arch/mips32/m51xx/picoRTOS_port.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o: ../../../ipc/picoRTOS_cond.c  .generated_files/flags/default/832fb8b1b6c8505c426b3d1ca161081ba0864140 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1386518765" 
	@${RM} ${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o.d" -o ${OBJECTDIR}/_ext/1386518765/picoRTOS_cond.o ../../../ipc/picoRTOS_cond.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o: ../../../ipc/picoRTOS_mutex.c  .generated_files/flags/default/7cb79dfcff169704ae159dd793b0e8e8d954b58e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1386518765" 
	@${RM} ${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o.d" -o ${OBJECTDIR}/_ext/1386518765/picoRTOS_mutex.o ../../../ipc/picoRTOS_mutex.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/2124829536/picoRTOS.o: ../../../picoRTOS.c  .generated_files/flags/default/b3e20160e90242aa16feb1eb13317cd52237848b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2124829536" 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/2124829536/picoRTOS.o.d" -o ${OBJECTDIR}/_ext/2124829536/picoRTOS.o ../../../picoRTOS.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o: ../../../drivers/pwm/pwm-pic32mx.c  .generated_files/flags/default/3615e99b7bce8da5f2befc46c4d63abb0590ebb3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1462861907" 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../" -I"../" -I"../../../ipc" -I"../../../arch/include" -I"../../../arch/mips32/m51xx" -I"../../../devices/microchip/pic32mz-ef" -I"../../../drivers/include" -I"../../../drivers/misc" -I"../../../drivers/clock" -I"../../../drivers/mux" -I"../../../drivers/gpio" -I"../../../drivers/uart" -I"../../../drivers/spi" -I"../../../drivers/wd" -I"../../../drivers/pwm" -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o.d" -o ${OBJECTDIR}/_ext/1462861907/pwm-pic32mx.o ../../../drivers/pwm/pwm-pic32mx.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
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
