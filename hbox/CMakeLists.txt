cmake_minimum_required(VERSION 3.16)


if(MSVC)
    add_compile_options(-utf-8 )
endif()

add_library(HBox)
file(GLOB  HBOX_C_CPP_FILES *.cpp *.h *.c *.h)
target_sources(HBox PUBLIC ${HBOX_C_CPP_FILES})
target_include_directories(HBox PUBLIC ./ )


#设置属性
set_property(TARGET HBox PROPERTY CXX_STANDARD 11)
set_property(TARGET HBox PROPERTY C_STANDARD 99)

#启用HBox
function(hbox_enable targetname)
target_link_libraries(${targetname} HBox)
endfunction()
