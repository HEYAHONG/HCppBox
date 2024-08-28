cmake_minimum_required(VERSION 3.21)


if(MSVC)
    add_compile_options(-utf-8 )
endif()

add_library(HCppBox EXCLUDE_FROM_ALL)
file(GLOB  HCPPBOX_C_CPP_FILES *.cpp *.h)
target_sources(HCppBox PRIVATE ${HCPPBOX_C_CPP_FILES})
target_include_directories(HCppBox PUBLIC ./ )


#添加线程库
find_package(Threads REQUIRED)
target_link_libraries(HCppBox  ${CMAKE_THREAD_LIBS_INIT})

#链接hbox
hbox_enable(HCppBox)

#设置C++标准
set_property(TARGET HCppBox PROPERTY CXX_STANDARD 23)

#设置C标准
set_property(TARGET HCppBox PROPERTY C_STANDARD 23)

#启用HCppBox
function(hcppbox_enable targetname)
target_link_libraries(${targetname} HCppBox)
target_compile_definitions(${targetname} PUBLIC HCPPBOX_ENABLE=1)
endfunction()
