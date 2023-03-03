cmake_minimum_required(VERSION 3.16)

#设置C++标准
set(CMAKE_CXX_STANDARD 20)

if(MSVC)
    add_compile_options(-utf-8 )
endif()

add_library(CPPObject)
file(GLOB  CPPOBJECT_CPP_FILES *.cpp *.h)
target_sources(CPPObject PUBLIC ${CPPOBJECT_CPP_FILES})
target_include_directories(CPPObject PUBLIC ./)

#添加线程库
find_package(Threads REQUIRED)
target_link_libraries(CPPObject  ${CMAKE_THREAD_LIBS_INIT})

