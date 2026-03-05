
add_custom_target(3rdparty_tools)

#添加rvvm工具构建
add_custom_target(rvvm_tool  ${CMAKE_COMMAND} -E make_directory rvvm && ${CMAKE_COMMAND} -DHTOOLS_INSTALL_DIR=${HTOOLS_INSTALL_DIR}  -S ${CMAKE_CURRENT_SOURCE_DIR}/rvvm/ -B rvvm && ${CMAKE_COMMAND} --build rvvm -t install
                                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/
                                    USES_TERMINAL)
add_dependencies(3rdparty_tools rvvm_tool)
message(STATUS "Enable rvvm_tool!" )

if(AUTORECONF AND BISON AND FLEX)
#添加matiec工具构建
add_custom_target(matiec_tool  ${CMAKE_COMMAND} -E make_directory matiec && ${CMAKE_COMMAND} -DHTOOLS_INSTALL_DIR=${HTOOLS_INSTALL_DIR}  -S ${CMAKE_CURRENT_SOURCE_DIR}/matiec/ -B matiec && ${CMAKE_COMMAND} --build matiec -t install
                                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/
                                    USES_TERMINAL)
add_dependencies(3rdparty_tools matiec_tool)
message(STATUS "Enable matiec_tool!" )
else()
message(STATUS "Disable matiec_tool!" )
endif()

