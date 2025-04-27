
cmake_minimum_required(VERSION 3.14)


#添加可执行文件
add_executable(HBoxTest)

#设置C/C++标准
set_property(TARGET HBoxTest PROPERTY CXX_STANDARD 23)
set_property(TARGET HBoxTest PROPERTY C_STANDARD 23)

#启用HBox
hbox_enable(HBoxTest)

#启用HRC
hrc_enable(HBoxTest)

#添加源代码
file(GLOB  HBOXTEST_CPP_C_FILES *.cpp *.CPP *.c *.C *.h)
target_sources(HBoxTest PUBLIC ${HBOXTEST_CPP_C_FILES})
