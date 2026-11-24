#***************************************************************
# Name:      CMakeLists.txt
# Purpose:   moxiebox picolibc C运行库CMake构建脚本
# Author:    HYH (hyhsystem.cn)
# Created:   2026-11-24
# Copyright: HYH (hyhsystem.cn)
# License:   MIT
#**************************************************************/

set(MOXIEBOX_PICOCRT_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/picocrt/crt0.c
    )

file(GLOB MOXIEBOX_LIBOS_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/libos/*.c
    )

target_sources(picolibc_c_port PRIVATE
    ${MOXIEBOX_PICOCRT_FILES}
    ${MOXIEBOX_LIBOS_FILES}
    )

target_include_directories(picolibc_c_port PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    )

#
# Reuse the picolibc header search path from the main library.  This
# keeps the port building against the same generated picolibc.h and the
# same machine headers as libc itself.
#
get_target_property(MOXIEBOX_PICOLIBC_INCLUDE_DIRECTORIES
    c INTERFACE_INCLUDE_DIRECTORIES)
if(MOXIEBOX_PICOLIBC_INCLUDE_DIRECTORIES)
    target_include_directories(picolibc_c_port PRIVATE
        ${MOXIEBOX_PICOLIBC_INCLUDE_DIRECTORIES})
endif()

target_compile_options(picolibc_c_port PRIVATE
    -ffreestanding
    -fno-stack-protector
    -D_LIBC
    )
