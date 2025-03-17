
cmake_minimum_required(VERSION 3.14)


#添加可执行文件
add_executable(HShell)

#设置C/C++标准
set_property(TARGET HShell PROPERTY CXX_STANDARD 23)
set_property(TARGET HShell PROPERTY C_STANDARD 23)

#启用HBox
hbox_enable(HShell)


#添加源代码
file(GLOB  HSHELL_CPP_C_FILES *.cpp *.CPP *.c *.C *.h)
target_sources(HShell PUBLIC ${HSHELL_CPP_C_FILES})
