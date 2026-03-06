
cmake_minimum_required(VERSION 3.20)

project(rvvm)

if(WIN32)
if(MSVC)
add_compile_options(-utf-8)
endif()
endif()


include(CheckLibraryExists)

if (LINUX OR BSD OR CMAKE_SYSTEM_NAME STREQUAL "SunOS")
check_library_exists(X11 XInitThreads ""  HAVE_X11)
if(HAVE_X11)
message(STATUS "X11:ON")
set(USE_X11 ON)
else()
message(STATUS "X11:OFF")
set(USE_X11 OFF)
endif()
endif()

if (LINUX)
check_library_exists(wayland-client wl_display_connect ""  HAVE_WAYLAND)
if(HAVE_WAYLAND)
message(STATUS "wayland:ON")
set(USE_WAYLAND ON)
else()
message(STATUS "wayland:OFF")
set(USE_WAYLAND OFF)
endif()
endif()

#添加rvvm代码
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../common/3rdparty/rvvm  rvvm)


add_custom_target(rvvm_all ALL)

#编译rvvm库及rvvm_cli
add_dependencies(rvvm_all rvvm_cli)
add_dependencies(rvvm_all rvvm)


include(GNUInstallDirs)
if(DEFINED HTOOLS_INSTALL_DIR)
install(TARGETS rvvm rvvm_cli
       RUNTIME DESTINATION ${HTOOLS_INSTALL_DIR}/bin
       LIBRARY DESTINATION ${HTOOLS_INSTALL_DIR}/lib
       ARCHIVE DESTINATION ${HTOOLS_INSTALL_DIR}/lib
       )
install(DIRECTORY   ${CMAKE_CURRENT_LIST_DIR}/../../common/3rdparty/rvvm/include/
	DESTINATION ${HTOOLS_INSTALL_DIR}/include/)
endif()

