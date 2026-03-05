
add_custom_target(3rdparty_tools)

#添加rvvm工具构建
add_custom_target(rvvm_tool  ${CMAKE_COMMAND} -E make_directory rvvm && ${CMAKE_COMMAND} -DHTOOLS_INSTALL_DIR=${HTOOLS_INSTALL_DIR}  -S ${CMAKE_CURRENT_SOURCE_DIR}/rvvm/ -B rvvm && ${CMAKE_COMMAND} --build rvvm -t install
                                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/
                                    USES_TERMINAL)
add_dependencies(3rdparty_tools rvvm_tool)
message(STATUS "Enable rvvm_tool!" )


