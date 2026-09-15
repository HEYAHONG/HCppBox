
cmake_minimum_required(VERSION 3.14)


#添加可执行文件
add_executable(monochromescreen_simplegui)

#设置C++标准
set_property(TARGET monochromescreen_simplegui PROPERTY CXX_STANDARD 23)


#启用HCppBox
hcppbox_enable(monochromescreen_simplegui)
#启用Hrc
hrc_enable(monochromescreen_simplegui)


#添加源代码
file(GLOB  MAIN_CPP_C_FILES *.cpp *.CPP *.c *.C *.h)
target_sources(monochromescreen_simplegui PUBLIC ${MAIN_CPP_C_FILES})

#链接simplegui
target_link_libraries(monochromescreen_simplegui hsimplegui)
