
add_custom_target(hcrypto_tools)

include(FindPkgConfig)

#添加hashtool工具构建
add_custom_target(hashtool_tool  ${CMAKE_COMMAND} -E make_directory hashtool && ${CMAKE_COMMAND}  -S ${CMAKE_CURRENT_SOURCE_DIR}/hashtool/ -B hashtool && ${CMAKE_COMMAND} --build hashtool
                                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/
                                    USES_TERMINAL)
add_dependencies(hcrypto_tools hashtool_tool)
message(STATUS "Enable hashtool_tool!" )

