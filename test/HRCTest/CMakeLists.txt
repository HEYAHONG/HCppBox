
cmake_minimum_required(VERSION 3.14)


#添加可执行文件
add_executable(HRCTest)

#设置C++标准
set_property(TARGET HRCTest PROPERTY CXX_STANDARD 23)


#启用HRC
hrc_enable(HRCTest)


#添加源代码
file(GLOB  HRCTEST_CPP_C_FILES *.cpp *.CPP *.c *.C *.h)
target_sources(HRCTest PUBLIC ${HRCTEST_CPP_C_FILES})
