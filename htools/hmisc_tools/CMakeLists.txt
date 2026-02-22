
add_custom_target(hmisc_tools)


#添加hgzip工具构建
add_custom_target(hgzip_tool  ${CMAKE_COMMAND} -E make_directory hgzip && ${CMAKE_COMMAND}  -DHTOOLS_INSTALL_DIR=${HTOOLS_INSTALL_DIR} -S ${CMAKE_CURRENT_SOURCE_DIR}/hgzip/ -B hgzip && ${CMAKE_COMMAND} --build hgzip -t install
                                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/
                                    USES_TERMINAL)
add_dependencies(hcrypto_tools hgzip_tool)
message(STATUS "Enable hgzip_tool!" )

#添加hlz4工具构建
add_custom_target(hlz4_tool  ${CMAKE_COMMAND} -E make_directory hlz4 && ${CMAKE_COMMAND}  -DHTOOLS_INSTALL_DIR=${HTOOLS_INSTALL_DIR} -S ${CMAKE_CURRENT_SOURCE_DIR}/hlz4/ -B hlz4 && ${CMAKE_COMMAND} --build hlz4 -t install
                                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/
                                    USES_TERMINAL)
add_dependencies(hcrypto_tools hlz4_tool)
message(STATUS "Enable hlz4_tool!" )


