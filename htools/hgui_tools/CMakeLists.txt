
add_custom_target(hgui_tools)

include(FindPkgConfig)

pkg_check_modules(DOTFONTSCAN_TOOL_FREETYPE2 IMPORTED_TARGET freetype2)
if(DOTFONTSCAN_TOOL_FREETYPE2_FOUND)
#添加dotfontscan工具构建
add_custom_target(dotfontscan_tool  ${CMAKE_COMMAND} -E make_directory dotfontscan && ${CMAKE_COMMAND}  -S ${CMAKE_CURRENT_SOURCE_DIR}/dotfontscan/ -B dotfontscan && ${CMAKE_COMMAND} --build dotfontscan 
                                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/
                                    USES_TERMINAL)
add_dependencies(hgui_tools dotfontscan_tool)
message(STATUS "Enable dotfontscan_tool!" )
else()
message(STATUS "Disable dotfontscan_tool!" )
endif()

find_package(OpenCV QUIET)
if(OPENCV_FOUND)
#添加imageresourcegenerate工具构建
add_custom_target(imageresourcegenerate_tool  ${CMAKE_COMMAND} -E make_directory imageresourcegenerate && ${CMAKE_COMMAND}  -S ${CMAKE_CURRENT_SOURCE_DIR}/imageresourcegenerate/ -B imageresourcegenerate && ${CMAKE_COMMAND} --build imageresourcegenerate
                                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/
                                    USES_TERMINAL)
add_dependencies(hgui_tools imageresourcegenerate_tool)
message(STATUS "Enable imageresourcegenerate_tool!" )
else()
message(STATUS "Disable imageresourcegenerate_tool!" )
endif()

