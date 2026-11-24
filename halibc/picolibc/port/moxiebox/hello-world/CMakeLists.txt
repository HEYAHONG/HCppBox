#***************************************************************
# Name:      CMakeLists.txt
# Purpose:   picolibc moxiebox hello-world模板
# Author:    HYH (hyhsystem.cn)
# Created:   2026-11-24
# Copyright: HYH (hyhsystem.cn)
# License:   MIT
#***************************************************************
cmake_minimum_required(VERSION 3.25)

include(${CMAKE_CURRENT_SOURCE_DIR}/toolchain.cmake)

project(picolibc_moxiebox_hello_world LANGUAGES C CXX ASM)

#
# picolibc源代码目录，默认使用当前模板所在仓库中的源码。
#
if(NOT PICOLIBC_DIR)
    set(PICOLIBC_DIR
        "${CMAKE_CURRENT_SOURCE_DIR}/../../.."
        CACHE PATH "picolibc source tree")
endif()

set(HALIBC_ENABLE_PICOLIBC ON CACHE BOOL "" FORCE)
set(HALIBC_PICOLIBC_MOXIEBOX ON CACHE BOOL "" FORCE)
set(__THREAD_LOCAL_STORAGE OFF CACHE BOOL "" FORCE)

add_subdirectory(${PICOLIBC_DIR} picolibc)

get_target_property(PICOLIBC_INCLUDE_DIRECTORIES
    c INTERFACE_INCLUDE_DIRECTORIES)

add_executable(hello-world hello-world.c)

target_include_directories(hello-world PRIVATE
    ${PICOLIBC_INCLUDE_DIRECTORIES})

target_link_libraries(hello-world PRIVATE
    "-Wl,--start-group"
    picolibc_c_port
    c
    "-Wl,--end-group"
    gcc)

target_link_options(hello-world PRIVATE
    -nostdlib
    -nostartfiles
    -T${PICOLIBC_DIR}/port/moxiebox/moxiebox.ld
    -Wl,-e,_start)

set_target_properties(hello-world PROPERTIES
    SUFFIX ".elf")

#
# C++ hello-world，使用moxie工具链的libstdc++。
#
add_executable(hello-world-cpp hello-world.cpp)

target_include_directories(hello-world-cpp PRIVATE
    ${PICOLIBC_INCLUDE_DIRECTORIES})

target_link_libraries(hello-world-cpp PRIVATE
    "-Wl,--start-group"
    picolibc_c_port
    c
    stdc++
    supc++
    "-Wl,--end-group"
    gcc)

target_link_options(hello-world-cpp PRIVATE
    -nostdlib
    -nostartfiles
    -T${PICOLIBC_DIR}/port/moxiebox/moxiebox.ld
    -Wl,-e,_start)

set_target_properties(hello-world-cpp PROPERTIES
    SUFFIX ".elf")
