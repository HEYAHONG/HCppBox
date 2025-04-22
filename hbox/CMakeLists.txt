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

#添加dotfontscan工具构建
add_custom_target(dotfontscan_tool 	${CMAKE_COMMAND} -E make_directory dotfontscan && ${CMAKE_COMMAND}  -S ${CMAKE_CURRENT_SOURCE_DIR}/gui/tools/dotfontscan/ -B dotfontscan && ${CMAKE_COMMAND} --build dotfontscan 
									WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/
									USES_TERMINAL)
