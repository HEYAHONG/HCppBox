
cmake_minimum_required(VERSION 3.14)


#添加可执行文件
add_executable(ModbusTCPServerTiny)

#设置C++标准
set_property(TARGET ModbusTCPServerTiny  PROPERTY CXX_STANDARD 23)


#启用HCppBox
hcppbox_enable(ModbusTCPServerTiny)
#启用HBox
hbox_enable(ModbusTCPServerTiny)

#添加源代码
file(GLOB  MODBUSTCPSERVERTINY_CPP_C_FILES *.cpp *.CPP *.c *.C *.h)
target_sources(ModbusTCPServerTiny PUBLIC ${MODBUSTCPSERVERTINY_CPP_C_FILES})

include(FindPkgConfig)
if(PKG_CONFIG_FOUND)
pkg_check_modules(READLINE IMPORTED_TARGET readline)
if(READLINE_FOUND)
target_link_libraries(ModbusTCPServerTiny PkgConfig::READLINE)
add_definitions(-DHAVE_READLINE=1)
endif()
endif()
