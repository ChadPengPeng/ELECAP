# CMSIS Build CMakeLists generated on 2024-06-04T21:10:34

cmake_minimum_required(VERSION 3.22)

# Target options

set(TARGET ELECAP)
set(CPU Cortex-M7)
set(PRJ_DIR "d:/my_project/ELECAP/MDK-ARM")
set(OUT_DIR "D:/my_project/ELECAP/MDK-ARM/out/ELECAP/ELECAP")
set(INT_DIR "D:/my_project/ELECAP/MDK-ARM/tmp/ELECAP/ELECAP")
set(FPU DP_FPU)
set(BYTE_ORDER Little-endian)
set(OPTIMIZE speed)
set(DEBUG on)
set(AS_FLAGS_GLOBAL "-masm=auto")
set(CC_FLAGS_GLOBAL "-std=c99 -fno-rtti -funsigned-char -fshort-enums -fshort-wchar -ffunction-sections -Wno-packed -Wno-missing-variable-declarations -Wno-missing-prototypes -Wno-missing-noreturn -Wno-sign-conversion -Wno-nonportable-include-path -Wno-reserved-id-macro -Wno-unused-macros -Wno-documentation-unknown-command -Wno-documentation -Wno-license-management -Wno-parentheses-equality")
set(CXX_FLAGS_GLOBAL "-xc++ -std=c++03 -fno-rtti -funsigned-char -fshort-enums -fshort-wchar -ffunction-sections -Wno-packed -Wno-missing-variable-declarations -Wno-missing-prototypes -Wno-missing-noreturn -Wno-sign-conversion -Wno-nonportable-include-path -Wno-reserved-id-macro -Wno-unused-macros -Wno-documentation-unknown-command -Wno-documentation -Wno-license-management -Wno-parentheses-equality")
set(LD_FLAGS_GLOBAL "--map --load_addr_map_info --xref --callgraph --symbols --info sizes --info totals --info unused --info veneers --strict --summary_stderr --info summarysizes")
set(LD_SCRIPT "d:/my_project/ELECAP/MDK-ARM/ELECAP_ELECAP.sct")

set(DEFINES
  STM32H750xx
  _RTE_
  USE_HAL_DRIVER
)

set(INC_PATHS
  "d:/my_project/ELECAP/Core/Inc"
  "d:/my_project/ELECAP/Drivers/STM32H7xx_HAL_Driver/Inc"
  "d:/my_project/ELECAP/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy"
  "d:/my_project/ELECAP/Drivers/CMSIS/Device/ST/STM32H7xx/Include"
  "d:/my_project/ELECAP/Drivers/CMSIS/Include"
  "d:/my_project/ELECAP/user/device"
  "d:/my_project/ELECAP/user/fatfs"
  "d:/my_project/ELECAP/MDK-ARM/RTE/_ELECAP"
  "C:/Users/ChadPeng/AppData/Local/Arm/Packs/ARM/CMSIS/5.9.0/CMSIS/Core/Include"
  "C:/Users/ChadPeng/AppData/Local/arm/packs/Keil/STM32H7xx_DFP/3.1.1/Drivers/CMSIS/Device/ST/STM32H7xx/Include"
)

set(AS_ARM_SRC_FILES
  "d:/my_project/ELECAP/MDK-ARM/startup_stm32h750xx.s"
)

set(CC_SRC_FILES
  "d:/my_project/ELECAP/Core/Src/main.c"
  "d:/my_project/ELECAP/Core/Src/stm32h7xx_hal_msp.c"
  "d:/my_project/ELECAP/Core/Src/stm32h7xx_it.c"
  "d:/my_project/ELECAP/Core/Src/system_stm32h7xx.c"
  "d:/my_project/ELECAP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c"
  "d:/my_project/ELECAP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.c"
  "d:/my_project/ELECAP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.c"
  "d:/my_project/ELECAP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma_ex.c"
  "d:/my_project/ELECAP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_exti.c"
  "d:/my_project/ELECAP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash.c"
  "d:/my_project/ELECAP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash_ex.c"
  "d:/my_project/ELECAP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.c"
  "d:/my_project/ELECAP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hsem.c"
  "d:/my_project/ELECAP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c.c"
  "d:/my_project/ELECAP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c_ex.c"
  "d:/my_project/ELECAP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mdma.c"
  "d:/my_project/ELECAP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.c"
  "d:/my_project/ELECAP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.c"
  "d:/my_project/ELECAP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.c"
  "d:/my_project/ELECAP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.c"
  "d:/my_project/ELECAP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_spi.c"
  "d:/my_project/ELECAP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_spi_ex.c"
  "d:/my_project/ELECAP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sram.c"
  "d:/my_project/ELECAP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim.c"
  "d:/my_project/ELECAP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim_ex.c"
  "d:/my_project/ELECAP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart.c"
  "d:/my_project/ELECAP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart_ex.c"
  "d:/my_project/ELECAP/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_fmc.c"
  "d:/my_project/ELECAP/user/device/bsp_delay.c"
  "d:/my_project/ELECAP/user/device/dev_lcd.c"
)

# Toolchain config map

set(REGISTERED_TOOLCHAIN_ROOT "c:/Users/ChadPeng/.vcpkg/artifacts/2139c4c6/compilers.arm.armclang/6.22.0/bin")
set(REGISTERED_TOOLCHAIN_VERSION "6.22.0")
set(TOOLCHAIN_VERSION_MIN "6.22.0")
include ("C:/Users/ChadPeng/.vcpkg/artifacts/2139c4c6/tools.open.cmsis.pack.cmsis.toolbox/2.4.0/etc/AC6.6.16.2.cmake")
include ("C:/Users/ChadPeng/.vcpkg/artifacts/2139c4c6/tools.open.cmsis.pack.cmsis.toolbox/2.4.0/etc/CMSIS-Build-Utils.cmake")

# Setup project

project(${TARGET} LANGUAGES AS_ARM C)

cbuild_get_running_toolchain(TOOLCHAIN_ROOT TOOLCHAIN_VERSION C)

# Global Flags

set(CMAKE_AS_ARM_FLAGS "${AS_ARM_CPU} ${AS_ARM_BYTE_ORDER} ${AS_ARM_DEFINES} ${AS_ARM_OPTIONS_FLAGS} ${AS_ARM_FLAGS} ${AS_FLAGS_GLOBAL}")
cbuild_get_system_includes(CC_SYS_INC_PATHS_LIST CC_SYS_INC_PATHS)
set(CMAKE_C_FLAGS "${CC_CPU} ${CC_BYTE_ORDER} ${CC_DEFINES} ${CC_OPTIONS_FLAGS} ${CC_FLAGS} ${CC_FLAGS_GLOBAL} ${CC_SYS_INC_PATHS}")
set(CMAKE_C_LINK_FLAGS "${LD_CPU} ${_LS}\"${LD_SCRIPT}\" ${LD_FLAGS_GLOBAL} ${LD_OPTIONS_FLAGS} ${LD_FLAGS}")

# Local Flags

foreach(SRC ${AS_ARM_SRC_FILES})
  set_source_files_properties(${SRC} PROPERTIES LANGUAGE AS_ARM)
endforeach()

# Compilation Database

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_custom_target(database COMMAND ${CMAKE_COMMAND} -E copy_if_different "${INT_DIR}/compile_commands.json" "${OUT_DIR}")

# Setup Target

add_executable(${TARGET} ${AS_ARM_SRC_FILES} ${CC_SRC_FILES})
set_target_properties(${TARGET} PROPERTIES PREFIX "" SUFFIX ".axf" OUTPUT_NAME "ELECAP")
set_target_properties(${TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUT_DIR} LINK_DEPENDS ${LD_SCRIPT})
target_include_directories(${TARGET} PUBLIC ${INC_PATHS})
