cmake_minimum_required(VERSION 3.16)

#设置默认C++标准
set(CMAKE_CXX_STANDARD 23)

if(MSVC)
    add_compile_options(-utf-8 )
endif()

add_library(HCppBox)
file(GLOB  HCPPBOX_CPP_FILES *.cpp *.h)
target_sources(HCppBox PUBLIC ${HCPPBOX_CPP_FILES})
target_include_directories(HCppBox PUBLIC ./)

#添加线程库
find_package(Threads REQUIRED)
target_link_libraries(HCppBox  ${CMAKE_THREAD_LIBS_INIT})

#设置C++标准
set_property(TARGET HCppBox PROPERTY CXX_STANDARD 23)
