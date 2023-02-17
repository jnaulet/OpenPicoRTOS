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
FINAL_IMAGE=${DISTDIR}/pic18f16q40-cnano.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/pic18f16q40-cnano.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
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
SOURCEFILES_QUOTED_IF_SPACED=../main.c ../../../arch/pic/pic18f/picoRTOS_port.c ../../../picoRTOS.c ../../../arch/pic/pic18f/picoRTOS_portasm.S

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1472/main.p1 ${OBJECTDIR}/_ext/629782492/picoRTOS_port.p1 ${OBJECTDIR}/_ext/2124829536/picoRTOS.p1 ${OBJECTDIR}/_ext/629782492/picoRTOS_portasm.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1472/main.p1.d ${OBJECTDIR}/_ext/629782492/picoRTOS_port.p1.d ${OBJECTDIR}/_ext/2124829536/picoRTOS.p1.d ${OBJECTDIR}/_ext/629782492/picoRTOS_portasm.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1472/main.p1 ${OBJECTDIR}/_ext/629782492/picoRTOS_port.p1 ${OBJECTDIR}/_ext/2124829536/picoRTOS.p1 ${OBJECTDIR}/_ext/629782492/picoRTOS_portasm.o

# Source Files
SOURCEFILES=../main.c ../../../arch/pic/pic18f/picoRTOS_port.c ../../../picoRTOS.c ../../../arch/pic/pic18f/picoRTOS_portasm.S



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
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/pic18f16q40-cnano.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F16Q40
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/main.p1: ../main.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -Og -maddrqual=ignore -xassembler-with-cpp -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/pic/pic18f" -I"../../../devices/microchip/pic18f16q40" -v -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1472/main.p1 ../main.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/main.d ${OBJECTDIR}/_ext/1472/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/629782492/picoRTOS_port.p1: ../../../arch/pic/pic18f/picoRTOS_port.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/629782492" 
	@${RM} ${OBJECTDIR}/_ext/629782492/picoRTOS_port.p1.d 
	@${RM} ${OBJECTDIR}/_ext/629782492/picoRTOS_port.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -Og -maddrqual=ignore -xassembler-with-cpp -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/pic/pic18f" -I"../../../devices/microchip/pic18f16q40" -v -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/629782492/picoRTOS_port.p1 ../../../arch/pic/pic18f/picoRTOS_port.c 
	@-${MV} ${OBJECTDIR}/_ext/629782492/picoRTOS_port.d ${OBJECTDIR}/_ext/629782492/picoRTOS_port.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/629782492/picoRTOS_port.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/2124829536/picoRTOS.p1: ../../../picoRTOS.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/2124829536" 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.p1.d 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -Og -maddrqual=ignore -xassembler-with-cpp -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/pic/pic18f" -I"../../../devices/microchip/pic18f16q40" -v -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/2124829536/picoRTOS.p1 ../../../picoRTOS.c 
	@-${MV} ${OBJECTDIR}/_ext/2124829536/picoRTOS.d ${OBJECTDIR}/_ext/2124829536/picoRTOS.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/2124829536/picoRTOS.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/_ext/1472/main.p1: ../main.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -Og -maddrqual=ignore -xassembler-with-cpp -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/pic/pic18f" -I"../../../devices/microchip/pic18f16q40" -v -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1472/main.p1 ../main.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/main.d ${OBJECTDIR}/_ext/1472/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/629782492/picoRTOS_port.p1: ../../../arch/pic/pic18f/picoRTOS_port.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/629782492" 
	@${RM} ${OBJECTDIR}/_ext/629782492/picoRTOS_port.p1.d 
	@${RM} ${OBJECTDIR}/_ext/629782492/picoRTOS_port.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -Og -maddrqual=ignore -xassembler-with-cpp -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/pic/pic18f" -I"../../../devices/microchip/pic18f16q40" -v -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/629782492/picoRTOS_port.p1 ../../../arch/pic/pic18f/picoRTOS_port.c 
	@-${MV} ${OBJECTDIR}/_ext/629782492/picoRTOS_port.d ${OBJECTDIR}/_ext/629782492/picoRTOS_port.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/629782492/picoRTOS_port.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/2124829536/picoRTOS.p1: ../../../picoRTOS.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/2124829536" 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.p1.d 
	@${RM} ${OBJECTDIR}/_ext/2124829536/picoRTOS.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -Og -maddrqual=ignore -xassembler-with-cpp -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/pic/pic18f" -I"../../../devices/microchip/pic18f16q40" -v -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/2124829536/picoRTOS.p1 ../../../picoRTOS.c 
	@-${MV} ${OBJECTDIR}/_ext/2124829536/picoRTOS.d ${OBJECTDIR}/_ext/2124829536/picoRTOS.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/2124829536/picoRTOS.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/629782492/picoRTOS_portasm.o: ../../../arch/pic/pic18f/picoRTOS_portasm.S  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/629782492" 
	@${RM} ${OBJECTDIR}/_ext/629782492/picoRTOS_portasm.o.d 
	@${RM} ${OBJECTDIR}/_ext/629782492/picoRTOS_portasm.o 
	${MP_CC} -c $(MP_EXTRA_AS_PRE) -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -Og -maddrqual=ignore -xassembler-with-cpp -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/pic/pic18f" -I"../../../devices/microchip/pic18f16q40" -v -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto   -o ${OBJECTDIR}/_ext/629782492/picoRTOS_portasm.o  ../../../arch/pic/pic18f/picoRTOS_portasm.S 
	@${FIXDEPS} ${OBJECTDIR}/_ext/629782492/picoRTOS_portasm.o.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/_ext/629782492/picoRTOS_portasm.o: ../../../arch/pic/pic18f/picoRTOS_portasm.S  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/629782492" 
	@${RM} ${OBJECTDIR}/_ext/629782492/picoRTOS_portasm.o.d 
	@${RM} ${OBJECTDIR}/_ext/629782492/picoRTOS_portasm.o 
	${MP_CC} -c $(MP_EXTRA_AS_PRE) -mcpu=$(MP_PROCESSOR_OPTION)   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -Og -maddrqual=ignore -xassembler-with-cpp -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/pic/pic18f" -I"../../../devices/microchip/pic18f16q40" -v -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto   -o ${OBJECTDIR}/_ext/629782492/picoRTOS_portasm.o  ../../../arch/pic/pic18f/picoRTOS_portasm.S 
	@${FIXDEPS} ${OBJECTDIR}/_ext/629782492/picoRTOS_portasm.o.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/pic18f16q40-cnano.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=${DISTDIR}/pic18f16q40-cnano.X.${IMAGE_TYPE}.map  -D__DEBUG=1  -mdebugger=none  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -Og -maddrqual=ignore -xassembler-with-cpp -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/pic/pic18f" -I"../../../devices/microchip/pic18f16q40" -v -mwarn=-3 -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto        $(COMPARISON_BUILD) -Wl,--memorysummary,${DISTDIR}/memoryfile.xml -o ${DISTDIR}/pic18f16q40-cnano.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	@${RM} ${DISTDIR}/pic18f16q40-cnano.X.${IMAGE_TYPE}.hex 
	
else
${DISTDIR}/pic18f16q40-cnano.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=${DISTDIR}/pic18f16q40-cnano.X.${IMAGE_TYPE}.map  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -Og -maddrqual=ignore -xassembler-with-cpp -I"../../../" -I"../" -I"../../../arch/include" -I"../../../arch/pic/pic18f" -I"../../../devices/microchip/pic18f16q40" -v -mwarn=-3 -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     $(COMPARISON_BUILD) -Wl,--memorysummary,${DISTDIR}/memoryfile.xml -o ${DISTDIR}/pic18f16q40-cnano.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	
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
