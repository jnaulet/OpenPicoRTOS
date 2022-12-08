# CMake include file for RP2040 single-core
if (NOT PICORTOS_PATH)
   message(FATAL_ERROR "PICORTOS_PATH undefined")
endif()

set(PICORTOS_SRC_FILES
    ${PICORTOS_PATH}/picoRTOS-SMP.c
    ${PICORTOS_PATH}/arch/arm/cm0+/picoRTOS_port.c
    ${PICORTOS_PATH}/arch/arm/cm0+/rp2040/picoRTOS-SMP_port.c
    ${PICORTOS_PATH}/arch/arm/cm0+/rp2040/picoRTOS-SMP_portasm.S
    )

set(PICORTOS_INCLUDE_DIRS
    ${PICORTOS_PATH}
    ${PICORTOS_PATH}/arch/include
    ${PICORTOS_PATH}/arch/arm/cm0+/rp2040
    ${PICORTOS_PATH}/devices/raspberry/rp2040
    )
