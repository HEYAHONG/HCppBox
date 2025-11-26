
add_custom_target(hsoftplc_tools)

#添加dumpplcdll工具构建
add_custom_target(dumpplcdll_tool  ${CMAKE_COMMAND} -E make_directory dumpplcdll && ${CMAKE_COMMAND}  -S ${CMAKE_CURRENT_SOURCE_DIR}/dumpplcdll/ -B dumpplcdll && ${CMAKE_COMMAND} --build dumpplcdll 
                                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/
                                    USES_TERMINAL)
add_dependencies(hsoftplc_tools dumpplcdll_tool)
