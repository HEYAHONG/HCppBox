

#添加dotfontscan工具构建
add_custom_target(dotfontscan_tool  ${CMAKE_COMMAND} -E make_directory dotfontscan && ${CMAKE_COMMAND}  -S ${CMAKE_CURRENT_SOURCE_DIR}/dotfontscan/ -B dotfontscan && ${CMAKE_COMMAND} --build dotfontscan 
                                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/
                                    USES_TERMINAL)
