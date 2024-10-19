
cmake_minimum_required(VERSION 3.14)


#添加可执行文件
add_executable(ModbusTCPClient)

#设置C++标准
set_property(TARGET ModbusTCPClient  PROPERTY CXX_STANDARD 23)


#启用HCppBox
hcppbox_enable(ModbusTCPClient)
#启用HBox
hbox_enable(ModbusTCPClient)

#添加源代码
file(GLOB  CPP_C_FILES *.cpp *.CPP *.c *.C *.h)
target_sources(ModbusTCPClient PUBLIC ${CPP_C_FILES})

include(FindPkgConfig)
if(PKG_CONFIG_FOUND)
pkg_check_modules(READLINE IMPORTED_TARGET readline)
if(READLINE_FOUND)
target_link_libraries(ModbusTCPClient PkgConfig::READLINE)
add_definitions(-DHAVE_READLINE=1)
endif()
endif()
