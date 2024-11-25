
cmake_minimum_required(VERSION 3.14)


#添加可执行文件
add_executable(HCPPGuiTest)

#设置C++标准
set_property(TARGET HCPPGuiTest PROPERTY CXX_STANDARD 23)


#启用HCppBox
hcppbox_enable(HCPPGuiTest)


#添加源代码
file(GLOB  HCPPGUITEST_CPP_C_FILES *.cpp *.CPP *.c *.C *.h)
target_sources(HCPPGuiTest PUBLIC ${HCPPGUITEST_CPP_C_FILES})
