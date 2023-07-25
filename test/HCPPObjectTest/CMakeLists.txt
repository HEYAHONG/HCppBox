
cmake_minimum_required(VERSION 3.14)


#添加可执行文件
add_executable(HCPPObjectTest)

#设置C++标准
set_property(TARGET HCPPObjectTest PROPERTY CXX_STANDARD 20)


#包含HCPPObject
add_subdirectory(../../ HCppBox)

#启用HCppBox
hcppbox_enable(HCPPObjectTest)


#添加源代码
file(GLOB  HCPPOBJECTTEST_CPP_C_FILES *.cpp *.CPP *.c *.C *.h)
target_sources(HCPPObjectTest PUBLIC ${HCPPOBJECTTEST_CPP_C_FILES})
