
add_custom_target(hsimulator_tools)

include(FindPkgConfig)
include(CheckIncludeFile)

find_package(wxWidgets 3.2.0 QUIET)
check_include_file(rvvm/rvvm.h HAVE_RVVM_H)
if(WXWIDGETS_FOUND AND HAVE_RVVM_H)
#添加hsimulatortools工具构建
add_custom_target(hsimulatortools_tool  ${CMAKE_COMMAND} -E make_directory hsimulatortools && ${CMAKE_COMMAND}  -S ${CMAKE_CURRENT_SOURCE_DIR}/hsimulatortools/ -B hsimulatortools && ${CMAKE_COMMAND} --build hsimulatortools
                                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/
                                    USES_TERMINAL)
add_dependencies(hsimulator_tools hsimulatortools_tool)
message(STATUS "Enable hsimulatortools_tool!" )
else()
message(STATUS "Disable hsimulatortools_tool!" )
endif()
