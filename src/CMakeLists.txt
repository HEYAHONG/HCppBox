cmake_minimum_required(VERSION 3.16)

#设置默认C++标准
set(CMAKE_CXX_STANDARD 23)

if(MSVC)
    add_compile_options(-utf-8 )
endif()

add_library(HCppBox)
file(GLOB  HCPPBOX_C_CPP_FILES *.cpp *.h)
target_sources(HCppBox PUBLIC ${HCPPBOX_C_CPP_FILES})
target_include_directories(HCppBox PUBLIC ./ ../hbox/)

#添加线程库
find_package(Threads REQUIRED)
target_link_libraries(HCppBox  ${CMAKE_THREAD_LIBS_INIT})

#链接hbox
add_subdirectory(../hbox hbox EXCLUDE_FROM_ALL)
target_link_libraries(HCppBox HBox)

#设置C++标准
set_property(TARGET HCppBox PROPERTY CXX_STANDARD 23)
