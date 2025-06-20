cmake_minimum_required(VERSION 3.16)


if(MSVC)
    add_compile_options(-utf-8 )
endif()

add_library(HBox EXCLUDE_FROM_ALL)
file(GLOB  HBOX_C_CPP_FILES *.cpp *.hpp *.c *.h cpp/*.cpp cpp/*.hpp cpp/*.c cpp/*.h h3rdparty/*.cpp h3rdparty/*.hpp h3rdparty/*.c h3rdparty/*.h )
target_sources(HBox PRIVATE ${HBOX_C_CPP_FILES})
target_include_directories(HBox PUBLIC ./ )
target_include_directories(HBox PUBLIC ./cpp/ )


#设置属性
set_property(TARGET HBox PROPERTY CXX_STANDARD 11)
#虽然最低支持C99，但此处仍然设置为C11
set_property(TARGET HBox PROPERTY C_STANDARD 11)

#启用HBox
function(hbox_enable targetname)
target_link_libraries(${targetname} HBox)
target_compile_definitions(${targetname} PUBLIC  HBOX_ENABLE=1)
endfunction()

include(CheckIncludeFile)
#查找libdl库
check_include_file(dlfcn.h HAVE_DLFCN_H)
if(HAVE_DLFCN_H)
target_link_libraries(HBox dl)
target_compile_definitions(HBox PUBLIC  HAVE_DLFCN_H=1)
endif()

#查找sys/mman.h
check_include_file(sys/mman.h HAVE_SYS_MMAN_H)
if(HAVE_SYS_MMAN_H)
target_compile_definitions(HBox PUBLIC  HAVE_SYS_MMAN_H=1)
endif()

#查找unistd.h
check_include_file(unistd.h HAVE_UNISTD_H)
if(HAVE_UNISTD_H)
target_compile_definitions(HBox PUBLIC  HAVE_UNISTD_H=1)
endif()

#查找fcntl.h
check_include_file(fcntl.h HAVE_FCNTL_H)
if(HAVE_FCNTL_H)
target_compile_definitions(HBox PUBLIC  HAVE_FCNTL_H=1)
endif()

