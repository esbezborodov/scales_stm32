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
set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/STM32F103XX_FLASH.ld)

set(COMMON_DEFINITIONS USE_HAL_DRIVER STM32F103xB)
set(COMMON_OPTIONS
    ${MCU_FLAGS}
    -ffunction-sections
    -fdata-sections
    $<$<CONFIG:Debug>:-Og -g3>
    $<$<CONFIG:Release>:-Os -g>
)

# ST HAL is vendor code: build it separately, without project warnings.
set(HAL_DIR Drivers/STM32F1xx_HAL_Driver/Src)
add_library(stm32f1_hal STATIC
    ${HAL_DIR}/stm32f1xx_hal.c
    ${HAL_DIR}/stm32f1xx_hal_cortex.c
    ${HAL_DIR}/stm32f1xx_hal_dma.c
    ${HAL_DIR}/stm32f1xx_hal_exti.c
    ${HAL_DIR}/stm32f1xx_hal_flash.c
    ${HAL_DIR}/stm32f1xx_hal_flash_ex.c
    ${HAL_DIR}/stm32f1xx_hal_gpio.c
    ${HAL_DIR}/stm32f1xx_hal_gpio_ex.c
    ${HAL_DIR}/stm32f1xx_hal_i2c.c
    ${HAL_DIR}/stm32f1xx_hal_pwr.c
    ${HAL_DIR}/stm32f1xx_hal_rcc.c
    ${HAL_DIR}/stm32f1xx_hal_rcc_ex.c
    Core/Src/system_stm32f1xx.c
)
target_include_directories(stm32f1_hal SYSTEM PUBLIC
    Core/Inc
    Drivers/STM32F1xx_HAL_Driver/Inc
    Drivers/STM32F1xx_HAL_Driver/Inc/Legacy
    Drivers/CMSIS/Device/ST/STM32F1xx/Include
    Drivers/CMSIS/Include
)
target_compile_definitions(stm32f1_hal PUBLIC ${COMMON_DEFINITIONS})
target_compile_options(stm32f1_hal PRIVATE ${COMMON_OPTIONS})

add_executable(${PROJECT_NAME}
    Core/Src/main.c
    Core/Src/leds.c
    Core/Src/stm32f1xx_it.c
    Core/Src/stm32f1xx_hal_msp.c
    Core/common/drivers/hx711.c
    Core/common/lcd_lib/lcd_i2c.c
    Core/common/delay_lib/dwt_delay.c
    startup_stm32f103xb.s
)

set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX .elf)

target_include_directories(${PROJECT_NAME} PRIVATE
    Core/Inc
    Core/common
    Core/common/drivers
    Core/common/delay_lib
    Core/common/lcd_lib
)

target_compile_options(${PROJECT_NAME} PRIVATE
    ${COMMON_OPTIONS}
    $<$<COMPILE_LANGUAGE:C>:-Wall -Wextra>
)

target_link_libraries(${PROJECT_NAME} PRIVATE stm32f1_hal)

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
