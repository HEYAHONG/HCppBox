cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(base)

include_directories(${CMAKE_CURRENT_LIST_DIR})
add_compile_definitions(-DHBOX_CONFIG_HEADER=hbox_config.h )

target_sources(app PRIVATE src/main.cpp)

#设定HRC文件系统目录为fs
set(HRC_FS_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/fs/)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../../../../../hbox HBox EXCLUDE_FROM_ALL)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../../../../../hrc HRC EXCLUDE_FROM_ALL)
#启用HBox、HRC
target_link_libraries(app PUBLIC HBox RC)
target_link_libraries(HBox app)
target_link_libraries(h3rdparty_common app)
target_link_libraries(RC app)
target_compile_options(HBox PRIVATE -Wno-restrict )


#使用zephyr的链接脚本处理功能
zephyr_linker_sources(SECTIONS "${CMAKE_CURRENT_LIST_DIR}/../hbox.ld")
