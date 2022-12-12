cmake_minimum_required(VERSION 3.12.0)

# Set up the project
project(bear
    LANGUAGES C ASM
)

set(TARGET "bear_s")
enable_language( C ASM )

include(config.cmake)

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
    ${CMAKE_HOME_DIRECTORY}/src-comm/ips/qspi-flash/qspi-flash.c
    )

# Test
set(SRC_LIST
    ${SRC_LIST}
    ${CMAKE_HOME_DIRECTORY}/src-comm/ipstest/t-ns16550.c
    ${CMAKE_HOME_DIRECTORY}/src-comm/ipstest/t-qspi-bm-flash.c
    ${CMAKE_HOME_DIRECTORY}/src-comm/ipstest/t-qspi-flash.c
    ${CMAKE_HOME_DIRECTORY}/src-comm/ipstest/t-crypto-bm.c
    ${CMAKE_HOME_DIRECTORY}/src-comm/ipstest/crypto_test_data.c
    )


# Mbedtls
if(${CONFIG_MBEDTLS})
    add_definitions(-DINCLUDE_MBEDTLS)
    include_directories(../../mbedtls/include)
    link_directories(../../mbedtls/build/library)
    link_libraries(mbedcrypto)
    set(SRC_LIST
        ${SRC_LIST}
        ${CMAKE_HOME_DIRECTORY}/src-comm/ipstest/t-mbedtls.c
    )    
endif()

# Target
add_executable(${TARGET}.elf "")

target_link_options(${TARGET}.elf PRIVATE -T ${CMAKE_HOME_DIRECTORY}/src-s/bm.lds
    -Wl,-Map=linkmap.txt,--build-id=none  -Wl,--print-memory-usage -nostartfiles)

target_include_directories(${TARGET}.elf PUBLIC
    .
    ./src-s/core
    ./src-s/board/mpu
    ./src-comm/common
    ./src-comm/ips/ns16550
    ./src-comm/ips/qspi-flash
    ./src-comm/board/ns16550
    ./src-comm/board/reset
    ./src-comm/board/clock
    ./src-comm/board/stop
    ./src-comm/board/pinmux
    ./src-comm/cmsis
    ./src-comm/ipstest
    )

target_sources(${TARGET}.elf
    PRIVATE
    ${SRC_LIST}
)

add_custom_target(${TARGET}.bin ALL
    COMMAND ${OBJCOPY} -O binary ${TARGET}.elf ${TARGET}.bin
    COMMAND ${OBJDUMP} -xd ${TARGET}.elf > ${TARGET}.asm
    DEPENDS ${TARGET}.elf
)

# --------------------------------- NS target ----------------------------------
# Non-secure target
set(TARGET_NS "bear_ns")

# Source
set(NS_SRC_LIST
    ${CMAKE_HOME_DIRECTORY}/src-ns/core/startup_ARMCM33.S
    ${CMAKE_HOME_DIRECTORY}/src-ns/core/system_ARMCM33.c
    ${CMAKE_HOME_DIRECTORY}/src-ns/board/mpu/board-mpu.c
    ${CMAKE_HOME_DIRECTORY}/src-ns/main-ns.c
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
    ${CMAKE_HOME_DIRECTORY}/src-comm/ips/qspi-flash/qspi-flash.c
    )

# Test
set(NS_SRC_LIST
    ${NS_SRC_LIST}
    ${CMAKE_HOME_DIRECTORY}/src-comm/ipstest/t-ns16550.c
    ${CMAKE_HOME_DIRECTORY}/src-comm/ipstest/t-qspi-bm-flash.c
    ${CMAKE_HOME_DIRECTORY}/src-comm/ipstest/t-qspi-flash.c
    ${CMAKE_HOME_DIRECTORY}/src-comm/ipstest/t-crypto-bm.c
    ${CMAKE_HOME_DIRECTORY}/src-comm/ipstest/crypto_test_data.c
    )

add_executable(${TARGET_NS}.elf "")

target_link_options(${TARGET_NS}.elf PRIVATE -T ${CMAKE_HOME_DIRECTORY}/src-ns/bm.lds
    -Wl,-Map=linkmap.txt,--build-id=none  -Wl,--print-memory-usage -nostartfiles)

target_include_directories(${TARGET_NS}.elf PUBLIC
    .
    ./src-ns/core
    ./src-ns/board/mpu
    ./src-comm/common
    ./src-comm/ips/ns16550
    ./src-comm/ips/qspi-flash
    ./src-comm/board/ns16550
    ./src-comm/board/reset
    ./src-comm/board/clock
    ./src-comm/board/stop
    ./src-comm/board/pinmux
    ./src-comm/cmsis
    ./src-comm/ipstest
    )

target_sources(${TARGET_NS}.elf
    PRIVATE
    ${NS_SRC_LIST}
)

add_custom_target(${TARGET_NS}.bin ALL
    COMMAND ${OBJCOPY} -O binary ${TARGET_NS}.elf ${TARGET_NS}.bin
    COMMAND ${OBJDUMP} -xd ${TARGET_NS}.elf > ${TARGET_NS}.asm
    DEPENDS ${TARGET_NS}.elf
)
