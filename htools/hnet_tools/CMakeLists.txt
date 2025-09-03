
add_custom_target(hnet_tools)

include(FindPkgConfig)

find_package(wxWidgets 3.2.0 QUIET)

pkg_check_modules(LIBSERIALPORT QUIET IMPORTED_TARGET libserialport)

if(WXWIDGETS_FOUND AND LIBSERIALPORT_FOUND)
#添加hmodbushelper工具构建
add_custom_target(hmodbushelper_tool  ${CMAKE_COMMAND} -E make_directory hmodbushelper && ${CMAKE_COMMAND}  -S ${CMAKE_CURRENT_SOURCE_DIR}/hmodbushelper/ -B hmodbushelper && ${CMAKE_COMMAND} --build hmodbushelper
                                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/
                                    USES_TERMINAL)
add_dependencies(hnet_tools hmodbushelper_tool)
message(STATUS "Enable hmodbushelper_tool!" )
else()
message(STATUS "Disable hmodbushelper_tool!" )
endif()

pkg_check_modules(LIBEVENT QUIET IMPORTED_TARGET libevent)
pkg_check_modules(LIBEVENT_CORE QUIET IMPORTED_TARGET libevent_core)
pkg_check_modules(LIBEVENT_EXTRA QUIET IMPORTED_TARGET libevent_extra)
if(LIBSERIALPORT_FOUND AND LIBEVENT_FOUND AND LIBEVENT_CORE_FOUND AND LIBEVENT_EXTRA_FOUND)
#添加hmodbustcpgateway工具构建
add_custom_target(hmodbustcpgateway_tool  ${CMAKE_COMMAND} -E make_directory hmodbustcpgateway && ${CMAKE_COMMAND}  -S ${CMAKE_CURRENT_SOURCE_DIR}/hmodbustcpgateway/ -B hmodbustcpgateway && ${CMAKE_COMMAND} --build hmodbustcpgateway
                                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/
                                    USES_TERMINAL)
add_dependencies(hnet_tools hmodbustcpgateway_tool)
message(STATUS "Enable hmodbustcpgateway_tool!" )
else()
message(STATUS "Disable hmodbustcpgateway_tool!" )
endif()
