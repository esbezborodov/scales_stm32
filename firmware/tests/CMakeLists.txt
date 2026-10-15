# Host unit tests for the hardware-independent firmware modules.
# Usage: ./test.sh (or cmake -S firmware/tests -B build/tests && ctest --test-dir build/tests)
cmake_minimum_required(VERSION 3.20)

project(scale_firmware_tests LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(FIRMWARE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

enable_testing()

foreach(module IN ITEMS scale weight_format indicator)
    add_executable(test_${module}
        test_${module}.c
        ${FIRMWARE_DIR}/src/${module}.c
    )
    target_include_directories(test_${module} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${FIRMWARE_DIR}/inc
    )
    target_compile_options(test_${module} PRIVATE
        -Wall -Wextra -Werror -g -fsanitize=address,undefined -fno-sanitize-recover=all
    )
    target_link_options(test_${module} PRIVATE -fsanitize=address,undefined)
    add_test(NAME ${module} COMMAND test_${module})
endforeach()
