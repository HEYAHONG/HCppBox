
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
include_directories(
${MATIEC_C_HEADER}
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}/trafic_light
)
#启用hsoftplc组件
add_compile_options( -DHSOFTPLC=1 )

#添加HCppBox库
add_subdirectory(../../../ HCppBox EXCLUDE_FROM_ALL)

#trafic_light.st
add_custom_target(trafic_light_st ALL
                 SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/trafic_light.st
                 COMMAND ${IEC2C} -p -l -n -r -R -a -b -c -I ${MATIEC_IEC_HEADER} -T ${CMAKE_CURRENT_SOURCE_DIR}/trafic_light ${CMAKE_CURRENT_SOURCE_DIR}/trafic_light.st
                 )

##trafic_light.st库文件
file(GLOB BLINK_ST_PLC_C_FILES ${CMAKE_CURRENT_SOURCE_DIR}/trafic_light/Config*.c ${CMAKE_CURRENT_SOURCE_DIR}/trafic_light/Res*.c ${CMAKE_CURRENT_SOURCE_DIR}/trafic_light/*.h)
add_library(trafic_light_st_plc ${BLINK_ST_PLC_C_FILES})
add_dependencies(trafic_light_st_plc trafic_light_st)
set_property(TARGET trafic_light_st_plc PROPERTY CXX_STANDARD 11)
set_property(TARGET trafic_light_st_plc PROPERTY C_STANDARD 11)
target_link_libraries(HBox trafic_light_st_plc)

#主程序
file(GLOB BLINK_C_FILES  *.cpp *.h *.c)
add_executable(trafic_light ${BLINK_C_FILES})
target_link_libraries(trafic_light trafic_light_st_plc)
hcppbox_enable(trafic_light)
