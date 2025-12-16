
cmake_minimum_required(VERSION 3.21)

project(SoftPLC C CXX ASM)

#设定HRC文件系统目录为fs
set(HRC_FS_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/fs/)

if("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options(-fPIC )
endif()
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
file(GLOB TRAFIC_LIGHT_ST_PLC_C_FILES ${CMAKE_CURRENT_SOURCE_DIR}/trafic_light/Config*.c ${CMAKE_CURRENT_SOURCE_DIR}/trafic_light/Res*.c ${CMAKE_CURRENT_SOURCE_DIR}/trafic_light/*.h)
add_library(trafic_light_st_plc ${TRAFIC_LIGHT_ST_PLC_C_FILES})
add_dependencies(trafic_light_st_plc trafic_light_st)
set_property(TARGET trafic_light_st_plc PROPERTY CXX_STANDARD 11)
set_property(TARGET trafic_light_st_plc PROPERTY C_STANDARD 11)
target_link_libraries(HBox trafic_light_st_plc)

#主程序
file(GLOB TRAFIC_LIGHT_C_FILES  *.cpp *.h *.c)
add_executable(trafic_light ${TRAFIC_LIGHT_C_FILES})
target_link_libraries(trafic_light trafic_light_st_plc)
hcppbox_enable(trafic_light)

#dll(用于其它程序加载)
file(GLOB PLC_DLL_C_FILES  plc_dll.cpp)
add_library(plc SHARED ${PLC_DLL_C_FILES})
target_link_libraries(plc trafic_light_st_plc)
hcppbox_enable(plc)
if("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
    target_link_libraries(plc  -static-libgcc -static-libstdc++ )
endif()


if("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
file(GLOB PLC_OBJECT_C_FILES  ${CMAKE_CURRENT_SOURCE_DIR}/trafic_light/Config*.c ${CMAKE_CURRENT_SOURCE_DIR}/trafic_light/Res*.c )
add_library(plc_object OBJECT ${PLC_OBJECT_C_FILES})
target_compile_options(plc_object PUBLIC -fPIC )
file(GENERATE OUTPUT ${CMAKE_BINARY_DIR}/plc_mod.lst
                          CONTENT "$<TARGET_OBJECTS:plc_object>")
file(GENERATE OUTPUT ${CMAKE_BINARY_DIR}/plc_mod.cmake
                          INPUT ${CMAKE_CURRENT_SOURCE_DIR}/plc_mod.cmake.in)
#创建可重定位文件用于运行时直接加载
add_custom_target(plc_mod ALL
                                  COMMAND  cmake -P ${CMAKE_BINARY_DIR}/plc_mod.cmake && ${CMAKE_LINKER}  --relocatable -o plc.mod  @${CMAKE_BINARY_DIR}/plc_mod.lst
                                  DEPENDS plc_object
                                  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                                 )
endif()


#安装相关文件到对应目录
include(GNUInstallDirs)
if(DEFINED SOFTPLC_INSTALL_DIR)
install(TARGETS trafic_light plc
       RUNTIME DESTINATION ${SOFTPLC_INSTALL_DIR}/trafic_light/bin
       LIBRARY DESTINATION ${SOFTPLC_INSTALL_DIR}/trafic_light/lib
       ARCHIVE DESTINATION ${SOFTPLC_INSTALL_DIR}/trafic_light/lib
       )
if("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
install(FILES ${CMAKE_BINARY_DIR}/plc.mod
       DESTINATION ${SOFTPLC_INSTALL_DIR}/trafic_light/bin
       )
endif()
endif()
