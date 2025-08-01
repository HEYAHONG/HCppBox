cmake_minimum_required(VERSION 3.20)
set(PROJECT_NAME dotfontscan)
project(${PROJECT_NAME}
	VERSION 1.0.0.0
	LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 99)
file(GLOB C_CXX_FILES *.h *.cpp *.c *.hpp *.cxx)
add_executable(${PROJECT_NAME}  ${C_CXX_FILES})
#添加线程库
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME}  ${CMAKE_THREAD_LIBS_INIT})

#设定HCppBox使用纯C++
set(HCPPBOX_USE_PURE_CPLUSPLUS ON)

#设定HRC文件系统目录为fs
set(HRC_FS_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/fs/)
#添加3rdparty/HCppBox
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../../  HCppBox EXCLUDE_FROM_ALL)
#启用HCppBox、HRC
hcppbox_enable(${PROJECT_NAME})
hrc_enable(${PROJECT_NAME})

include(FindPkgConfig)
#添加FreeType
pkg_check_modules(FREETYPE2 IMPORTED_TARGET REQUIRED freetype2)
target_link_libraries(${PROJECT_NAME}  PkgConfig::FREETYPE2)

