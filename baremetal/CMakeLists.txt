cmake_minimum_required(VERSION 3.12.0)

# Set up the project
project(bear
    LANGUAGES C ASM
)

set(TARGET "bear")
enable_language( C ASM )

# Include
include_directories(.)
include_directories(./src-s/core)
include_directories(./src-s/board/mpu)
include_directories(./src-comm/common)
include_directories(./src-comm/ips/ns16550)
include_directories(./src-comm/board/ns16550)
include_directories(./src-comm/board/reset)
include_directories(./src-comm/board/clock)
include_directories(./src-comm/board/stop)
include_directories(./src-comm/board/pinmux)
include_directories(./src-comm/cmsis)
include_directories(./src-comm/ipstest)

# Link target
set(LD_SCRIPT "src-s/bm.lds")

set(CMAKE_EXE_LINKER_FLAGS "-T ${CMAKE_HOME_DIRECTORY}/${LD_SCRIPT} \
    -Wl,-Map=linkmap.txt,--build-id=none -nostartfiles")

# Source
aux_source_directory(. SRC_LIST)
set(SRC_LIST
    ${SRC_LIST}
    ${CMAKE_HOME_DIRECTORY}/src-s/core/startup_ARMCM33.S
    ${CMAKE_HOME_DIRECTORY}/src-s/core/system_ARMCM33.c
    ${CMAKE_HOME_DIRECTORY}/src-s/board/mpu/board-mpu.c
    ${CMAKE_HOME_DIRECTORY}/src-s/main-s.c
    ${CMAKE_HOME_DIRECTORY}/src-comm/common/common.c
    ${CMAKE_HOME_DIRECTORY}/src-comm/common/console.c
    ${CMAKE_HOME_DIRECTORY}/src-comm/common/kermit.c
    ${CMAKE_HOME_DIRECTORY}/src-comm/common/systick.c
    ${CMAKE_HOME_DIRECTORY}/src-comm/common/interrupt.c
    ${CMAKE_HOME_DIRECTORY}/src-comm/common/tiny_console.c
    ${CMAKE_HOME_DIRECTORY}/src-comm/board/ns16550/board-ns16550.c
    ${CMAKE_HOME_DIRECTORY}/src-comm/board/clock/board-clock.c
    ${CMAKE_HOME_DIRECTORY}/src-comm/board/reset/board-reset.c
    ${CMAKE_HOME_DIRECTORY}/src-comm/board/stop/board-stop.c
    ${CMAKE_HOME_DIRECTORY}/src-comm/board/pinmux/board-pinmux.c
    ${CMAKE_HOME_DIRECTORY}/src-comm/common/tiny_command.c
    ${CMAKE_HOME_DIRECTORY}/src-comm/ips/ns16550/ns16550.c
    )

# Test
set(SRC_LIST
    ${SRC_LIST}
    ${CMAKE_HOME_DIRECTORY}/src-comm/ipstest/t-ns16550.c
    ${CMAKE_HOME_DIRECTORY}/src-comm/ipstest/t-qspi-flash.c
    )

# Target
add_executable(${TARGET}.elf "")

target_sources(${TARGET}.elf
    PRIVATE
    ${SRC_LIST}
)

add_custom_target(${TARGET}.bin ALL
    COMMAND ${OBJCOPY} -O binary ${TARGET}.elf ${TARGET}.bin
    COMMAND ${OBJDUMP} -xd ${TARGET}.elf > ${TARGET}.asm
    DEPENDS ${TARGET}.elf
)
