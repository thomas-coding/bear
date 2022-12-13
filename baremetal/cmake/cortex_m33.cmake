# Name of the target
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-m33)
set(MCPU_FLAGS "-mthumb -mcpu=cortex-m33 -mcmse")
set(VFP_FLAGS "")
set(SPEC_FLAGS "--specs=nosys.specs")

include(${CMAKE_CURRENT_LIST_DIR}/arm-none-eabi.cmake)