
include(FindPkgConfig)
include(CheckIncludeFile)

pkg_check_modules(DOTFONTSCAN_TOOL_FREETYPE2 IMPORTED_TARGET freetype2)

find_package(OpenCV QUIET)

find_package(wxWidgets 3.2.0 QUIET)

pkg_check_modules(LIBSERIALPORT QUIET IMPORTED_TARGET libserialport)

pkg_check_modules(LIBEVENT QUIET IMPORTED_TARGET libevent)
pkg_check_modules(LIBEVENT_CORE QUIET IMPORTED_TARGET libevent_core)
pkg_check_modules(LIBEVENT_EXTRA QUIET IMPORTED_TARGET libevent_extra)


find_program(XMAKE xmake)
if(XMAKE STREQUAL "XMAKE-NOTFOUND")
set(XMAKE-NOTFOUND TRUE)
endif()
if(NOT XMAKE-NOTFOUND)
#检查tbox
execute_process( COMMAND  ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_LIST_DIR}/3rdparty/tbox/" "${CMAKE_CURRENT_LIST_DIR}/build/${CMAKE_SYSTEM_NAME}/${CMAKE_SYSTEM_VERSION}/${CMAKE_C_COMPILER_ID}/"
                 WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/3rdparty/tbox/"
                 RESULT_VARIABLE  TBOX_CMAKE_GENERATE_ERRORLEVEL
               )

if(${TBOX_CMAKE_GENERATE_ERRORLEVEL} EQUAL 0)
if(WIN32)
execute_process( COMMAND cmd.exe /c  ${XMAKE} project -k cmake
                 WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/build/${CMAKE_SYSTEM_NAME}/${CMAKE_SYSTEM_VERSION}/${CMAKE_C_COMPILER_ID}/"
                 RESULT_VARIABLE  TBOX_CMAKE_GENERATE_ERRORLEVEL
                 OUTPUT_VARIABLE  TBOX_CMAKE_GENERATE_OUTPUT
               )
else()
execute_process( COMMAND ${XMAKE} project -k cmake
                 WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/build/${CMAKE_SYSTEM_NAME}/${CMAKE_SYSTEM_VERSION}/${CMAKE_C_COMPILER_ID}/"
                 RESULT_VARIABLE  TBOX_CMAKE_GENERATE_ERRORLEVEL
                 OUTPUT_VARIABLE  TBOX_CMAKE_GENERATE_OUTPUT
               )
endif()
endif()

if(${TBOX_CMAKE_GENERATE_ERRORLEVEL} EQUAL 0)
set(TBOX ON)
message(STATUS "TBOX:ON")
else()
set(TBOX OFF)
message(STATUS "TBOX:OFF")
endif()

endif()
