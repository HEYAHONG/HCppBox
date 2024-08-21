
cmake_minimum_required(VERSION 3.21)

project(test C CXX ASM)

#设定HRC文件系统目录为fs
set(HRC_FS_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/fs/)

#包含库文件
add_subdirectory(../ HCppBox)

if(MSVC)
    add_compile_options(-utf-8 )
endif()

file(GLOB TESTS *)
foreach(TEST ${TESTS})
	if(IS_DIRECTORY ${TEST})
        if(EXISTS "${TEST}/CMakeLists.txt")
            file(RELATIVE_PATH TESTNAME ${CMAKE_CURRENT_SOURCE_DIR}/ ${TEST})
            message(STATUS "Add Test ${TESTNAME}")
            add_subdirectory(${TEST})
        endif()
	endif()
endforeach()

