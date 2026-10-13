cmake_minimum_required(VERSION 3.20)

if(NOT CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/arm-none-eabi.cmake)
endif()

project(scale_firmware LANGUAGES C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

set(MCU_FLAGS -mcpu=cortex-m3 -mthumb -mfloat-abi=soft)
set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/firmware/platform/STM32F103C8TX_FLASH.ld)

add_executable(${PROJECT_NAME}
    firmware/src/main.c
    firmware/src/leds.c
    firmware/common/delay/delay.c
    firmware/common/gpio_drv/gpio.c
    firmware/common/i2c_drv/i2c.c
    firmware/common/hx711_drv/hx711.c
    firmware/common/lcd_drv/lcd1602.c
    firmware/platform/clock.c
    firmware/platform/startup_stm32f103xb.s
    firmware/platform/system_stm32f1xx.c
)

set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX .elf)

target_include_directories(${PROJECT_NAME} PRIVATE
    firmware/inc
    firmware/platform
    firmware/common/delay
    firmware/common/gpio_drv
    firmware/common/i2c_drv
    firmware/common/hx711_drv
    firmware/common/lcd_drv
)

# Vendor headers are treated as system headers so that warnings
# are only reported for project code.
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
    vendor/CMSIS/Include
    vendor/CMSIS/Device/ST/STM32F1xx/Include
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    STM32F103xB
)

target_compile_options(${PROJECT_NAME} PRIVATE
    ${MCU_FLAGS}
    -ffunction-sections
    -fdata-sections
    $<$<COMPILE_LANGUAGE:C>:-Wall -Wextra>
    $<$<CONFIG:Debug>:-Og -g3>
    $<$<CONFIG:Release>:-Os -g>
)

# Vendor-provided system file: keep it untouched and silence its warnings.
set_source_files_properties(firmware/platform/system_stm32f1xx.c
    PROPERTIES COMPILE_OPTIONS -Wno-unused-parameter
)

target_link_options(${PROJECT_NAME} PRIVATE
    ${MCU_FLAGS}
    -T${LINKER_SCRIPT}
    --specs=nano.specs
    --specs=nosys.specs
    -Wl,--gc-sections
    -Wl,-Map=${PROJECT_NAME}.map,--cref
    -Wl,--print-memory-usage
)

set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY LINK_DEPENDS ${LINKER_SCRIPT})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_NAME}.bin
    COMMAND ${CMAKE_OBJCOPY} -O ihex   $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${PROJECT_NAME}>
    BYPRODUCTS ${PROJECT_NAME}.bin ${PROJECT_NAME}.hex ${PROJECT_NAME}.map
    VERBATIM
)
