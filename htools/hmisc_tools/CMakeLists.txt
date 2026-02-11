
add_custom_target(hmisc_tools)

include(FindPkgConfig)

#添加hgzip工具构建
add_custom_target(hgzip_tool  ${CMAKE_COMMAND} -E make_directory hgzip && ${CMAKE_COMMAND}  -DHTOOLS_INSTALL_DIR=${HTOOLS_INSTALL_DIR} -S ${CMAKE_CURRENT_SOURCE_DIR}/hgzip/ -B hgzip && ${CMAKE_COMMAND} --build hgzip -t install
                                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/
                                    USES_TERMINAL)
add_dependencies(hcrypto_tools hgzip_tool)
message(STATUS "Enable hgzip_tool!" )


