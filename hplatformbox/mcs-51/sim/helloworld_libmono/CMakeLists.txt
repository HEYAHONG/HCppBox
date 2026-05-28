
cmake_minimum_required(VERSION 3.15)
project(SDCCProject NONE)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../toolchain.cmake)

#SDCC设置
set(SDCC_MCS51_MODEL large)
set(SDCC_MCS51_IRAM_SIZE 256)
set(SDCC_MCS51_XRAM_SIZE 4096)
set(SDCC_MCS51_CODE_SIZE 65536)

add_mcs51_executable(${CMAKE_CURRENT_SOURCE_DIR}/helloworld.c -l liblonglong.lib)
