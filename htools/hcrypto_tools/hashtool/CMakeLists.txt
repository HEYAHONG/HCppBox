cmake_minimum_required(VERSION 3.20)
set(PROJECT_NAME hashtool)
project(${PROJECT_NAME}
	VERSION 1.0.0.0
	LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 99)

#添加线程库
find_package(Threads REQUIRED)

#设定HCppBox使用纯C++
set(HCPPBOX_USE_PURE_CPLUSPLUS ON)

#设定HRC文件系统目录为fs
set(HRC_FS_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/fs/)
#添加3rdparty/HCppBox
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../../  HCppBox EXCLUDE_FROM_ALL)

#hmd5sum
add_executable(hmd5sum  hmd5sum.cpp)
hcppbox_enable(hmd5sum)
hrc_enable(hmd5sum)
target_link_libraries(hmd5sum  ${CMAKE_THREAD_LIBS_INIT})
