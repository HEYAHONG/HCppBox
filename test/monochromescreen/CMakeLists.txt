
cmake_minimum_required(VERSION 3.14)


#添加可执行文件
add_executable(monochromescreen)

#设置C++标准
set_property(TARGET monochromescreen PROPERTY CXX_STANDARD 23)


#启用HCppBox
hcppbox_enable(monochromescreen)
#启用Hrc
hrc_enable(monochromescreen)


#添加源代码
file(GLOB  monochromescreen_CPP_C_FILES *.cpp *.CPP *.c *.C *.h)
target_sources(monochromescreen PUBLIC ${monochromescreen_CPP_C_FILES})
