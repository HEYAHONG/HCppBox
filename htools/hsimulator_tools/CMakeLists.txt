
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

if(NOT MSVC)
#添加hmini-rv32ima工具构建
add_custom_target(hmini-rv32ima_tool  ${CMAKE_COMMAND} -E make_directory hmini-rv32ima && ${CMAKE_COMMAND} -DHTOOLS_INSTALL_DIR=${HTOOLS_INSTALL_DIR}  -S ${CMAKE_CURRENT_SOURCE_DIR}/hmini-rv32ima/ -B hmini-rv32ima && ${CMAKE_COMMAND} --build hmini-rv32ima -t install
                                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/
                                    USES_TERMINAL)
add_dependencies(hsimulator_tools hmini-rv32ima_tool)
message(STATUS "Enable hmini-rv32ima_tool!" )
else()
message(STATUS "Disable hmini-rv32ima_tool!" )
endif()
