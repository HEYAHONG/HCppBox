cmake_minimum_required(VERSION 3.24)


if(MSVC)
    add_compile_options(-utf-8 )
endif()

add_library(HCppBox EXCLUDE_FROM_ALL)
file(GLOB  HCPPBOX_C_CPP_FILES *.cpp *.h)
target_sources(HCppBox PRIVATE ${HCPPBOX_C_CPP_FILES})
target_include_directories(HCppBox PUBLIC ./ )


#添加线程库
find_package(Threads REQUIRED)
target_link_libraries(HCppBox  ${CMAKE_THREAD_LIBS_INIT})

#链接ws2_32库
if(WIN32)
	target_link_libraries(HCppBox ws2_32)
endif()

if(CYGWIN)
        target_link_libraries(HCppBox ws2_32 gdi32)
endif()

if(DEFINED ENV{HCPPBOX_USE_PURE_CPLUSPLUS})
	set(HCPPBOX_USE_PURE_CPLUSPLUS ON)
endif()

if(NOT DEFINED HCPPBOX_USE_PURE_CPLUSPLUS)

include(FindPkgConfig)
if(PKG_CONFIG_FOUND)
pkg_check_modules(SDL IMPORTED_TARGET sdl)
pkg_check_modules(SDL2 IMPORTED_TARGET sdl2)
if(SDL_FOUND)
target_link_libraries(HCppBox PkgConfig::SDL)
add_definitions(-DHAVE_SDL=1)
else()
if(SDL2_FOUND)
target_link_libraries(HCppBox PkgConfig::SDL2)
add_definitions(-DHAVE_SDL2=1)
endif()
endif()
endif()

endif()


#链接hbox
hbox_enable(HCppBox)

#设置C++标准
set_property(TARGET HCppBox PROPERTY CXX_STANDARD 23)

#设置C标准
set_property(TARGET HCppBox PROPERTY C_STANDARD 23)

set(CMAKE_C_LINK_LIBRARY_USING_load_archive_SUPPORTED TRUE)
if(CMAKE_C_COMPILER_ID STREQUAL "AppleClang")
  set(CMAKE_C_LINK_LIBRARY_USING_load_archive "-force_load <LIB_ITEM>")
elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU" AND CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(CMAKE_C_LINK_LIBRARY_USING_load_archive
    "LINKER:--push-state,--whole-archive"
    "<LINK_ITEM>"
    "LINKER:--pop-state"
  )
elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
  set(CMAKE_C_LINK_LIBRARY_USING_load_archive "/WHOLEARCHIVE:<LIBRARY>")
else()
  # feature not yet supported for the other environments
  set(CMAKE_C_LINK_LIBRARY_USING_load_archive_SUPPORTED FALSE)
endif()


#启用HCppBox
function(hcppbox_enable targetname)
if(CMAKE_C_LINK_LIBRARY_USING_load_archive_SUPPORTED)
	target_link_libraries(${targetname} "$<LINK_LIBRARY:load_archive,HCppBox>")
else()
	target_link_libraries(${targetname} HCppBox)
endif()
target_compile_definitions(${targetname} PUBLIC HCPPBOX_ENABLE=1)
endfunction()
