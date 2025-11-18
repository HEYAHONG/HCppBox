
cmake_minimum_required(VERSION 3.21)

project(SoftPLC C CXX ASM)

#设定HRC文件系统目录为fs
set(HRC_FS_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/fs/)


if(MSVC)
    add_compile_options(-utf-8 )
endif()

include(../../../hbox/cmake/FindMATIEC.cmake)

if(NOT MATIEC_IEC2C_FOUND)
    message(FATAL_ERROR "iec2c must be existed!")
else()
    message(STATUS "iec2c: ${IEC2C}")
endif()

#添加C语言头文件
include_directories(${MATIEC_C_HEADER})

#blink.st
add_custom_target(blink_st ALL
                 SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/blink.st
                 COMMAND ${IEC2C} -p -l -n -r -R -a -b -c -I ${MATIEC_IEC_HEADER} -T ${CMAKE_CURRENT_SOURCE_DIR}/blink ${CMAKE_CURRENT_SOURCE_DIR}/blink.st
                 )

##blink.st库文件
file(GLOB BLINK_ST_PLC_C_FILES ${CMAKE_CURRENT_SOURCE_DIR}/blink/Config*.c ${CMAKE_CURRENT_SOURCE_DIR}/blink/Res*.c)
add_library(blink_st_plc ${BLINK_ST_PLC_C_FILES})
add_dependencies(blink_st_plc blink_st)
target_include_directories(blink_st_plc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/blink)
set_property(TARGET blink_st_plc PROPERTY CXX_STANDARD 11)
set_property(TARGET blink_st_plc PROPERTY C_STANDARD 11)
