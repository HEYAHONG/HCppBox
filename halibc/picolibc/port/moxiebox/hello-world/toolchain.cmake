#***************************************************************
# Name:      toolchain.cmake
# Purpose:   moxie moxiebox交叉编译工具链设置
# Author:    HYH (hyhsystem.cn)
# Created:   2026-11-24
# Copyright: HYH (hyhsystem.cn)
# License:   MIT
#***************************************************************
cmake_minimum_required(VERSION 3.20)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR moxie)

#
# 工具链优先从PATH中查找，也支持通过-DMOXIE_TOOLCHAIN=<bin目录>
# 或环境变量MOXIE_TOOLCHAIN指定。
#
set(_MOXIE_TOOLCHAIN_HINT)
if(MOXIE_TOOLCHAIN)
    set(_MOXIE_TOOLCHAIN_HINT "${MOXIE_TOOLCHAIN}")
elseif(DEFINED ENV{MOXIE_TOOLCHAIN})
    set(_MOXIE_TOOLCHAIN_HINT "$ENV{MOXIE_TOOLCHAIN}")
endif()

find_program(_MOXIE_GCC
    NAMES moxie-unknown-elf-gcc moxie-unknown-moxiebox-gcc
    HINTS ${_MOXIE_TOOLCHAIN_HINT})

if(NOT _MOXIE_GCC)
    message(FATAL_ERROR
        "未找到moxie交叉编译工具链(moxie-unknown-elf-gcc或moxie-unknown-moxiebox-gcc)，"
        "请将其加入PATH或设置MOXIE_TOOLCHAIN。")
endif()

get_filename_component(_MOXIE_TOOLCHAIN_DIR "${_MOXIE_GCC}" DIRECTORY)
get_filename_component(_MOXIE_GCC_NAME "${_MOXIE_GCC}" NAME)
string(REGEX REPLACE "-gcc$" "" _MOXIE_PREFIX "${_MOXIE_GCC_NAME}")

find_program(_MOXIE_GXX
    NAMES ${_MOXIE_PREFIX}-g++
    HINTS "${_MOXIE_TOOLCHAIN_DIR}" ${_MOXIE_TOOLCHAIN_HINT})

if(NOT _MOXIE_GXX)
    message(FATAL_ERROR "未找到${_MOXIE_PREFIX}-g++。")
endif()

set(CMAKE_C_COMPILER "${_MOXIE_GCC}")
set(CMAKE_CXX_COMPILER "${_MOXIE_GXX}")
set(CMAKE_ASM_COMPILER "${_MOXIE_GCC}")
set(CMAKE_AR "${_MOXIE_TOOLCHAIN_DIR}/${_MOXIE_PREFIX}-ar")
set(CMAKE_RANLIB "${_MOXIE_TOOLCHAIN_DIR}/${_MOXIE_PREFIX}-ranlib")
set(CMAKE_OBJCOPY "${_MOXIE_TOOLCHAIN_DIR}/${_MOXIE_PREFIX}-objcopy")
set(CMAKE_OBJDUMP "${_MOXIE_TOOLCHAIN_DIR}/${_MOXIE_PREFIX}-objdump")
set(CMAKE_READELF "${_MOXIE_TOOLCHAIN_DIR}/${_MOXIE_PREFIX}-readelf")
set(CMAKE_SIZE "${_MOXIE_TOOLCHAIN_DIR}/${_MOXIE_PREFIX}-size")
set(CMAKE_STRIP "${_MOXIE_TOOLCHAIN_DIR}/${_MOXIE_PREFIX}-strip")
set(CMAKE_LINKER "${_MOXIE_TOOLCHAIN_DIR}/${_MOXIE_PREFIX}-ld")
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
