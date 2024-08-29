cmake_minimum_required(VERSION 3.16)
if(MSVC)
    add_compile_options(-utf-8 )
endif()
if(CYGWIN)
    find_program(G++ NAMES g++  c++ REQUIRED)
    if(NOT EXISTS ${G++})
        message(FATAL_ERROR "g++ is not found")
    endif()
    execute_process(
                        COMMAND ${G++} "${CMAKE_CURRENT_SOURCE_DIR}/fsgen.cpp" -o fsgen_cygwin
                        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
                        )
    find_program(FSGEN NAMES fsgen_cygwin fsgen_cygwin.exe PATHS .  "${CMAKE_CURRENT_BINARY_DIR}" REQUIRED NO_CACHE )
endif()
if(NOT DEFINED FSGEN)
    find_program(FSGEN NAMES fsgen  PATHS . NO_CACHE )
endif()
if(NOT EXISTS ${FSGEN})
    message("fsgen is not found,try to compile!")
    find_program(G++ NAMES g++  c++ REQUIRED)
    if(NOT EXISTS ${G++})
        message(FATAL_ERROR "g++ is not found")
    endif()
    execute_process(
                        COMMAND ${G++} -static -static-libgcc -static "${CMAKE_CURRENT_SOURCE_DIR}/fsgen.cpp" -o fsgen_custom
                        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
                        )
    find_program(FSGEN NAMES fsgen_custom fsgen_custom.exe  PATHS . "${CMAKE_CURRENT_BINARY_DIR}" REQUIRED NO_CACHE )
endif()
#当设定了HRC_FS_ROOT_DIR变量时,采用HRC_FS_ROOT_DIR变量指定的内容。
if(DEFINED ENV{HRC_FS_ROOT_DIR})
    set(HRC_FS_ROOT_DIR $ENV{HRC_FS_ROOT_DIR})
endif()
if(DEFINED  HRC_FS_ROOT_DIR)
    file(GLOB_RECURSE FS_FILES ${HRC_FS_ROOT_DIR}/*)
else()
    file(GLOB_RECURSE FS_FILES fs/*)
    set(HRC_FS_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/fs)
endif()
add_custom_command(
                     COMMAND ${FSGEN} "${HRC_FS_ROOT_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/RC_fs.c"
                     WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
                     OUTPUT  "${CMAKE_CURRENT_BINARY_DIR}/RC_fs.c"
                     DEPENDS ${FS_FILES}
)
message(STATUS "FSGEN:${FSGEN}")
message(STATUS "FS_ROOT:${HRC_FS_ROOT_DIR}")
message(STATUS "RC_fs.c:${CMAKE_CURRENT_BINARY_DIR}/RC_fs.c")

add_library(RC STATIC EXCLUDE_FROM_ALL RC.h RC.c "${CMAKE_CURRENT_BINARY_DIR}/RC_fs.c")
target_include_directories(RC PUBLIC .)

#设置属性
set_property(TARGET RC PROPERTY CXX_STANDARD 11)
set_property(TARGET RC PROPERTY C_STANDARD 99)

#启用RC
function(hrc_enable targetname)
target_link_libraries(${targetname} RC)
target_compile_definitions(${targetname} PUBLIC  HRC_ENABLE=1)
endfunction()
