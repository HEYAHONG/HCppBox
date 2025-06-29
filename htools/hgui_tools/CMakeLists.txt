

#添加dotfontscan工具构建
add_custom_target(dotfontscan_tool  ${CMAKE_COMMAND} -E make_directory dotfontscan && ${CMAKE_COMMAND}  -S ${CMAKE_CURRENT_SOURCE_DIR}/dotfontscan/ -B dotfontscan && ${CMAKE_COMMAND} --build dotfontscan 
                                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/
                                    USES_TERMINAL)

#添加imageresourcegenerate工具构建
add_custom_target(imageresourcegenerate_tool  ${CMAKE_COMMAND} -E make_directory imageresourcegenerate && ${CMAKE_COMMAND}  -S ${CMAKE_CURRENT_SOURCE_DIR}/imageresourcegenerate/ -B imageresourcegenerate && ${CMAKE_COMMAND} --build imageresourcegenerate
                                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/
                                    USES_TERMINAL)



add_custom_target(hgui_tools
					                DEPENDS  dotfontscan_tool imageresourcegenerate_tool)
