
include(FindPkgConfig)
include(CheckIncludeFile)

pkg_check_modules(DOTFONTSCAN_TOOL_FREETYPE2 IMPORTED_TARGET freetype2)

find_package(OpenCV QUIET)

find_package(wxWidgets 3.2.0 QUIET)
if(NOT wxWidgets_FOUND)
    # vcpkg's wxWidgets 3.3 installs a CMake config package; fall back to it
    # when the module-mode FindwxWidgets cannot locate the installation.
    find_package(wxWidgets 3.2.0 CONFIG QUIET)
endif()

pkg_check_modules(LIBSERIALPORT QUIET IMPORTED_TARGET libserialport)

pkg_check_modules(LIBEVENT QUIET IMPORTED_TARGET libevent)
pkg_check_modules(LIBEVENT_CORE QUIET IMPORTED_TARGET libevent_core)
pkg_check_modules(LIBEVENT_EXTRA QUIET IMPORTED_TARGET libevent_extra)

find_program(AUTORECONF autoreconf)
if(AUTORECONF)
    message(STATUS "autoreconf:" ${AUTORECONF})
endif()

find_program(FLEX flex)
if(FLEX)
    message(STATUS "flex:" ${FLEX})
endif()

find_program(BISON bison)
if(BISON)
    message(STATUS "bison:" ${BISON})
endif()



