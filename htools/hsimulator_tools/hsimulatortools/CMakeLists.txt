
cmake_minimum_required(VERSION 3.20)


#程序名称
Set(PROJECT_NAME hsimulatortools)
#主版本
set(PROJECT_MAJOR_VERSION_STR 1)
#次版本号
set(PROJECT_MINOR_VERSION_STR 0)
#修订号
set(PROJECT_REVISION_VERSION_STR 0)
#构建版本（以时间戳天数作为构建版本）
string(TIMESTAMP PROJECT_BUILD_TIME_SECOND "%s" UTC)
math(EXPR  PROJECT_BUILD_VERSION_STR "${PROJECT_BUILD_TIME_SECOND}/60/60/24"  OUTPUT_FORMAT DECIMAL)
#程序版本
set(PROJECT_VERSION_STR "${PROJECT_MAJOR_VERSION_STR}.${PROJECT_MINOR_VERSION_STR}.${PROJECT_REVISION_VERSION_STR}.${PROJECT_BUILD_VERSION_STR}")
message(STATUS "Version:${PROJECT_VERSION_STR}")
configure_file(Version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/Version.h)
if(WIN32)
configure_file(Version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/win32/Version.h)
endif()

Project(${PROJECT_NAME} C CXX ASM)
Project(${PROJECT_NAME}	VERSION "${PROJECT_VERSION_STR}")

set(CMAKE_CXX_STANDARD 17)



if(WIN32)
	if(MSVC)
		add_compile_options(-utf-8)
	endif()
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../common
${CMAKE_CURRENT_SOURCE_DIR}/../../common/3rdparty/astyle/
${CMAKE_CURRENT_SOURCE_DIR}/../../common/3rdparty/TerminalWx/
${CMAKE_CURRENT_SOURCE_DIR}/../../common/3rdparty/TerminalWx/GTerm
${CMAKE_CURRENT_SOURCE_DIR}/../../common/3rdparty/TerminalWx/taTelnet
${CMAKE_CURRENT_SOURCE_DIR}/ )

file(GLOB MAIN_CXX_C_FILES *.cpp *.c *.h ../../common/common_wxwidgets.h ../../common/common_wxwidgets.cpp  guibase/*.cpp guibase/*.c guibase/*.h 
../../common/3rdparty/astyle/ASBeautifier.cpp
../../common/3rdparty/astyle/ASEnhancer.cpp
../../common/3rdparty/astyle/ASFormatter.cpp
../../common/3rdparty/astyle/ASLocalizer.cpp
../../common/3rdparty/astyle/ASResource.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../../common/3rdparty/TerminalWx/*.h
${CMAKE_CURRENT_SOURCE_DIR}/../../common/3rdparty/TerminalWx/GTerm/*.h
${CMAKE_CURRENT_SOURCE_DIR}/../../common/3rdparty/TerminalWx/taTelnet/*.h
${CMAKE_CURRENT_SOURCE_DIR}/../../common/3rdparty/TerminalWx/*.hpp
${CMAKE_CURRENT_SOURCE_DIR}/../../common/3rdparty/TerminalWx/GTerm/*.hpp
${CMAKE_CURRENT_SOURCE_DIR}/../../common/3rdparty/TerminalWx/taTelnet/*.hpp
${CMAKE_CURRENT_SOURCE_DIR}/../../common/3rdparty/TerminalWx/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../../common/3rdparty/TerminalWx/GTerm/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../../common/3rdparty/TerminalWx/taTelnet/*.cpp
)
list(APPEND  MAIN_SRCS ${MAIN_CXX_C_FILES})

if(WIN32)
	file(GLOB_RECURSE MAIN_RC_FILES win32/*.rc)
	list(APPEND  MAIN_SRCS ${MAIN_RC_FILES})
endif()

if(WIN32)
	add_executable(${PROJECT_NAME} WIN32 ${MAIN_SRCS})
else()
	add_executable(${PROJECT_NAME} ${MAIN_SRCS})
endif()
target_link_libraries(${PROJECT_NAME} ${MAIN_LIBS})



#添加线程库
FIND_PACKAGE(Threads REQUIRED)
TARGET_LINK_LIBRARIES(${PROJECT_NAME}  ${CMAKE_THREAD_LIBS_INIT})

if(WIN32)
	find_package(wxWidgets 3.2.0 REQUIRED core base adv aui stc webview html gl richtext )
else()
	find_package(wxWidgets 3.2.0 REQUIRED all)
endif()
include(${wxWidgets_USE_FILE})
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})

include(FindPkgConfig)
#添加rvvm 
pkg_check_modules(LIBRVVM REQUIRED IMPORTED_TARGET rvvm)
target_link_libraries(${PROJECT_NAME}  PkgConfig::LIBRVVM)


#设定HCppBox使用纯C++
set(HCPPBOX_USE_PURE_CPLUSPLUS ON)

#设定HRC文件系统目录为fs
set(HRC_FS_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/fs/)
#添加HCppBox
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../common/  common  EXCLUDE_FROM_ALL)
#启用HCppBox、HRC
hcppbox_enable(${PROJECT_NAME})
hrc_enable(${PROJECT_NAME})

include(GNUInstallDirs)
if(DEFINED HTOOLS_INSTALL_DIR)
install(TARGETS ${PROJECT_NAME}
       RUNTIME DESTINATION ${HTOOLS_INSTALL_DIR}/bin
       LIBRARY DESTINATION ${HTOOLS_INSTALL_DIR}/lib
       ARCHIVE DESTINATION ${HTOOLS_INSTALL_DIR}/lib
       )
endif()
