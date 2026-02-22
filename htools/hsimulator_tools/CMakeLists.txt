
add_custom_target(hsimulator_tools)


if(WXWIDGETS_FOUND)
#添加hsimulatortools工具构建
add_custom_target(hsimulatortools_tool  ${CMAKE_COMMAND} -E make_directory hsimulatortools && ${CMAKE_COMMAND} -DHTOOLS_INSTALL_DIR=${HTOOLS_INSTALL_DIR}  -S ${CMAKE_CURRENT_SOURCE_DIR}/hsimulatortools/ -B hsimulatortools && ${CMAKE_COMMAND} --build hsimulatortools -t install
                                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/
                                    USES_TERMINAL)
add_dependencies(hsimulator_tools hsimulatortools_tool)
message(STATUS "Enable hsimulatortools_tool!" )
else()
message(STATUS "Disable hsimulatortools_tool!" )
endif()
