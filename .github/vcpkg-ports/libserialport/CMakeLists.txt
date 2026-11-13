# Minimal CMake build for libserialport, which upstream only provides with
# Autotools (Unix) and MSBuild (Windows). It compiles the same three sources
# and defines the same LIBSERIALPORT_MSBUILD macro as libserialport.vcxproj.
cmake_minimum_required(VERSION 3.20)
project(libserialport C)

set(CMAKE_C_STANDARD 99)

add_library(serialport SHARED
    serialport.c
    timing.c
    windows.c
)

target_compile_definitions(serialport PRIVATE LIBSERIALPORT_MSBUILD)
target_include_directories(serialport PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(serialport PRIVATE setupapi)

set_target_properties(serialport PROPERTIES
    OUTPUT_NAME serialport
    PREFIX ""
)

install(TARGETS serialport
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
install(FILES libserialport.h DESTINATION include)

# Relocatable pkg-config file; ${pcfiledir} is expanded by pkgconf/pkg-config.
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/libserialport.pc" [=[
prefix=${pcfiledir}/../..
exec_prefix=${prefix}
libdir=${prefix}/lib
includedir=${prefix}/include

Name: libserialport
Description: Cross-platform serial port access library
URL: https://sigrok.org/wiki/Libserialport
Version: 0.1.2
Libs: -L${libdir} -lserialport
Cflags: -I${includedir}
]=])
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/libserialport.pc"
    DESTINATION lib/pkgconfig)
