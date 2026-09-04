# =============================================================================
# Clion_h7_01 - self-maintained CMake build.
# CubeMX only GENERATES code (Core/Drivers/Middlewares). It does NOT manage
# this file, so regenerating from CubeMX will never break the build.
# Toolchain file + linker script + config overrides live outside CubeMX.
# =============================================================================
cmake_minimum_required(VERSION 3.22)
project(Clion_h7_01 LANGUAGES C ASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# ---------------------------------------------------------------------------
# Sources
# ---------------------------------------------------------------------------
# CubeMX-generated application code: auto-collect every .c in Core/Src.
# New peripherals added in CubeMX (usart.c, dma.c, ...) are picked up
# automatically after regeneration.
file(GLOB APP_SRC ${PROJECT_ROOT}/Core/Src/*.c)
# LTDC is owned by the board driver lcd_rgb.c -> exclude CubeMX's copy.
list(FILTER APP_SRC EXCLUDE REGEX ".*/ltdc\\.c$")

set(STARTUP_SRC ${PROJECT_ROOT}/startup_stm32h743xx.s)

# HAL drivers (extend this list when you enable a new peripheral in CubeMX)
set(HAL_SRC
  ${PROJECT_ROOT}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c
  ${PROJECT_ROOT}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.c
  ${PROJECT_ROOT}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.c
  ${PROJECT_ROOT}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.c
  ${PROJECT_ROOT}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash.c
  ${PROJECT_ROOT}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash_ex.c
  ${PROJECT_ROOT}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.c
  ${PROJECT_ROOT}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hsem.c
  ${PROJECT_ROOT}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.c
  ${PROJECT_ROOT}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma_ex.c
  ${PROJECT_ROOT}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mdma.c
  ${PROJECT_ROOT}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.c
  ${PROJECT_ROOT}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.c
  ${PROJECT_ROOT}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c.c
  ${PROJECT_ROOT}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c_ex.c
  ${PROJECT_ROOT}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_exti.c
  ${PROJECT_ROOT}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim.c
  ${PROJECT_ROOT}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim_ex.c
  ${PROJECT_ROOT}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_ltdc.c
  ${PROJECT_ROOT}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_ltdc_ex.c
  ${PROJECT_ROOT}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sdram.c
  ${PROJECT_ROOT}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_fmc.c
  ${PROJECT_ROOT}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_qspi.c
  ${PROJECT_ROOT}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sd.c
  ${PROJECT_ROOT}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_sdmmc.c
  ${PROJECT_ROOT}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sai.c
  ${PROJECT_ROOT}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma2d.c
  ${PROJECT_ROOT}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart.c
  ${PROJECT_ROOT}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart_ex.c
)

# FreeRTOS kernel (CMSIS-RTOS v2)
set(FREERTOS_SRC
  ${PROJECT_ROOT}/Middlewares/Third_Party/FreeRTOS/Source/croutine.c
  ${PROJECT_ROOT}/Middlewares/Third_Party/FreeRTOS/Source/event_groups.c
  ${PROJECT_ROOT}/Middlewares/Third_Party/FreeRTOS/Source/list.c
  ${PROJECT_ROOT}/Middlewares/Third_Party/FreeRTOS/Source/queue.c
  ${PROJECT_ROOT}/Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c
  ${PROJECT_ROOT}/Middlewares/Third_Party/FreeRTOS/Source/tasks.c
  ${PROJECT_ROOT}/Middlewares/Third_Party/FreeRTOS/Source/timers.c
  ${PROJECT_ROOT}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c
  ${PROJECT_ROOT}/Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c
  ${PROJECT_ROOT}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
)

# FatFs (microSD filesystem, CubeMX-generated middleware)
set(FATFS_SRC
  ${PROJECT_ROOT}/Middlewares/Third_Party/FatFs/src/ff.c
  ${PROJECT_ROOT}/Middlewares/Third_Party/FatFs/src/diskio.c
  ${PROJECT_ROOT}/Middlewares/Third_Party/FatFs/src/ff_gen_drv.c
  ${PROJECT_ROOT}/Middlewares/Third_Party/FatFs/src/option/unicode.c
  ${PROJECT_ROOT}/Middlewares/Third_Party/FatFs/src/option/syscall.c
  ${PROJECT_ROOT}/FATFS/App/fatfs.c
  ${PROJECT_ROOT}/FATFS/Target/sd_diskio.c
  ${PROJECT_ROOT}/FATFS/Target/bsp_driver_sd.c
)

# LVGL
file(GLOB_RECURSE LVGL_SRC ${PROJECT_ROOT}/LVGL/src/*.c)
set(LVGL_PORT_SRC
  ${PROJECT_ROOT}/LVGL/porting/lv_port_disp_template.c
  ${PROJECT_ROOT}/LVGL/porting/lv_port_indev_template.c
)

# SquareLine UI
file(GLOB UI_SRC
  ${PROJECT_ROOT}/ui/*.c
  ${PROJECT_ROOT}/ui/components/*.c
  ${PROJECT_ROOT}/ui/fonts/*.c
  ${PROJECT_ROOT}/ui/screens/*.c
  ${PROJECT_ROOT}/ui/images/*.c
)

# Board drivers (self-contained, own MSP init; not managed by CubeMX)
set(USER_DRV_SRC
  ${PROJECT_ROOT}/Drivers/User/Src/led.c
  ${PROJECT_ROOT}/Drivers/User/Src/sdram.c
  ${PROJECT_ROOT}/Drivers/User/Src/lcd_rgb.c
  ${PROJECT_ROOT}/Drivers/User/Src/lcd_pwm.c
  ${PROJECT_ROOT}/Drivers/User/Src/touch_800x480.c
  ${PROJECT_ROOT}/Drivers/User/Src/touch_iic.c
  ${PROJECT_ROOT}/Drivers/User/Src/qspi_w25q64.c
  ${PROJECT_ROOT}/Drivers/User/Src/usart.c
  ${PROJECT_ROOT}/Drivers/User/Src/audio_player.c
)

add_executable(${PROJECT_NAME}
  ${APP_SRC} ${STARTUP_SRC} ${HAL_SRC} ${FREERTOS_SRC} ${FATFS_SRC}
  ${LVGL_SRC} ${LVGL_PORT_SRC} ${UI_SRC} ${USER_DRV_SRC}
)

# ---------------------------------------------------------------------------
# Includes: config/ comes FIRST so OUR hal_conf.h / FreeRTOSConfig.h win
# over the CubeMX-generated ones (regeneration cannot break them).
# ---------------------------------------------------------------------------
target_include_directories(${PROJECT_NAME} PRIVATE
  ${PROJECT_ROOT}/config
  ${PROJECT_ROOT}/Core/Inc
  ${PROJECT_ROOT}/Drivers/STM32H7xx_HAL_Driver/Inc
  ${PROJECT_ROOT}/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy
  ${PROJECT_ROOT}/Drivers/CMSIS/Device/ST/STM32H7xx/Include
  ${PROJECT_ROOT}/Drivers/CMSIS/Include
  ${PROJECT_ROOT}/Middlewares/Third_Party/FreeRTOS/Source/include
  ${PROJECT_ROOT}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
  ${PROJECT_ROOT}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
  ${PROJECT_ROOT}/Middlewares/Third_Party/FatFs/src
  ${PROJECT_ROOT}/Middlewares/Third_Party/minimp3
  ${PROJECT_ROOT}/FATFS/Target
  ${PROJECT_ROOT}/FATFS/App
  ${PROJECT_ROOT}/Drivers/User/Inc
  ${PROJECT_ROOT}/LVGL
  ${PROJECT_ROOT}/LVGL/src
  ${PROJECT_ROOT}/LVGL/src/core
  ${PROJECT_ROOT}/LVGL/src/draw
  ${PROJECT_ROOT}/LVGL/src/draw/sw
  ${PROJECT_ROOT}/LVGL/src/extra
  ${PROJECT_ROOT}/LVGL/src/extra/libs
  ${PROJECT_ROOT}/LVGL/src/extra/others
  ${PROJECT_ROOT}/LVGL/src/extra/themes
  ${PROJECT_ROOT}/LVGL/src/extra/widgets
  ${PROJECT_ROOT}/LVGL/src/font
  ${PROJECT_ROOT}/LVGL/src/gpu
  ${PROJECT_ROOT}/LVGL/src/hal
  ${PROJECT_ROOT}/LVGL/src/misc
  ${PROJECT_ROOT}/LVGL/src/widgets
  ${PROJECT_ROOT}/LVGL/porting
  ${PROJECT_ROOT}/ui
  ${PROJECT_ROOT}/ui/components
  ${PROJECT_ROOT}/ui/fonts
  ${PROJECT_ROOT}/ui/screens
  ${PROJECT_ROOT}/ui/images
  ${PROJECT_ROOT}/Drivers/User/Inc
)

target_compile_definitions(${PROJECT_NAME} PRIVATE USE_HAL_DRIVER STM32H743xx)

# ---------------------------------------------------------------------------
# Link
# ---------------------------------------------------------------------------
target_link_options(${PROJECT_NAME} PRIVATE
  -T${PROJECT_ROOT}/stm32h743_qspi.ld
  --specs=nano.specs
  -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.map
  -Wl,--gc-sections
  -Wl,--print-memory-usage
)

# libm for sinf() etc.; target_link_libraries keeps it after the objects
target_link_libraries(${PROJECT_NAME} PRIVATE m)

# ---------------------------------------------------------------------------
# Post-build firmware artifacts (hex, internal-flash bin, qspi bin)
# ---------------------------------------------------------------------------
set(FW_HEX      ${CMAKE_CURRENT_BINARY_DIR}/Clion_h7_01.hex)
set(FW_FLASHBIN ${CMAKE_CURRENT_BINARY_DIR}/Clion_h7_01-flash.bin)
set(FW_QSPIBIN  ${CMAKE_CURRENT_BINARY_DIR}/Clion_h7_01-qspi.bin)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}> ${FW_HEX}
  COMMAND ${CMAKE_OBJCOPY} -O binary
          -j .isr_vector -j .text -j .rodata -j .ARM.extab -j .ARM.exidx
          -j .preinit_array -j .init_array -j .fini_array -j .data
          $<TARGET_FILE:${PROJECT_NAME}> ${FW_FLASHBIN}
  COMMAND ${CMAKE_OBJCOPY} -O binary --only-section=.qspi_ui
          $<TARGET_FILE:${PROJECT_NAME}> ${FW_QSPIBIN}
  COMMAND ${CMAKE_SIZE} --format=berkeley $<TARGET_FILE:${PROJECT_NAME}>
  BYPRODUCTS ${FW_HEX} ${FW_FLASHBIN} ${FW_QSPIBIN}
  COMMENT "building firmware artifacts (hex, flash bin, qspi bin)"
  VERBATIM
)
