
cmake_minimum_required(VERSION 3.21)

find_package(Git)

if(GIT_FOUND)
	message(STATUS "Git found:${GIT_EXECUTABLE}")
	execute_process(COMMAND ${GIT_EXECUTABLE} pull
							 WORKING_DIRECTORY	${CMAKE_SOURCE_DIR})
	execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
		  					WORKING_DIRECTORY 	${CMAKE_SOURCE_DIR}
		 					OUTPUT_VARIABLE PROJECT_VERSION)
	message(STATUS "Project Version:${PROJECT_VERSION}")
endif()

project(test C CXX ASM)

#设定HRC文件系统目录为fs
set(HRC_FS_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/fs/)

#包含库文件
add_subdirectory(../ HCppBox)

if(MSVC)
    add_compile_options(-utf-8 )
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/toolchain.cmake.in ${CMAKE_BINARY_DIR}/toolchain.cmake @ONLY)

if(NOT DEFINED TEST_INSTALL_DIR)
    set(TEST_INSTALL_DIR ${CMAKE_BINARY_DIR}/test)
endif()


file(GLOB TESTS *)
foreach(TEST ${TESTS})
	if(IS_DIRECTORY ${TEST})
        if(EXISTS "${TEST}/CMakeLists.txt")
            file(RELATIVE_PATH TESTNAME ${CMAKE_CURRENT_SOURCE_DIR}/ ${TEST})
            message(STATUS "Add Test ${TESTNAME}")
            add_subdirectory(${TEST})
            if(DEFINED TEST_INSTALL_DIR)
                install(    TARGETS ${TESTNAME}
                            RUNTIME DESTINATION ${TEST_INSTALL_DIR}/bin
                            LIBRARY DESTINATION ${TEST_INSTALL_DIR}/lib
                            ARCHIVE DESTINATION ${TEST_INSTALL_DIR}/lib
                       )
            endif()
        endif()
	endif()
endforeach()

if(NOT MSVC)

if(NOT DEFINED SOFTPLC_INSTALL_DIR)
    set(SOFTPLC_INSTALL_DIR ${TEST_INSTALL_DIR}/SoftPLC)
endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/../hbox/cmake/FindMATIEC.cmake)

if(MATIEC_IEC2C_FOUND)

file(GLOB SOFTPLCS SoftPLC/*)
foreach(SOFTPLC ${SOFTPLCS})
    if(IS_DIRECTORY ${SOFTPLC})
        if(EXISTS "${SOFTPLC}/CMakeLists.txt")
            file(RELATIVE_PATH SOFTPLCNAME ${CMAKE_CURRENT_SOURCE_DIR}/SoftPLC ${SOFTPLC})
            message(STATUS "Add SoftPLC ${SOFTPLCNAME}")
            add_custom_target(${SOFTPLCNAME}_softplc ALL  ${CMAKE_COMMAND} -E make_directory ${SOFTPLCNAME}-softplc  && ${CMAKE_COMMAND} -DCMAKE_TOOLCHAIN_FILE=${CMAKE_BINARY_DIR}/toolchain.cmake  -DSOFTPLC_INSTALL_DIR=${SOFTPLC_INSTALL_DIR}  -S ${SOFTPLC} -B ${SOFTPLCNAME}-softplc && ${CMAKE_COMMAND} --build ${SOFTPLCNAME}-softplc  -t install
                                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/
                                    USES_TERMINAL)
        endif()
    endif()
endforeach()

endif()

endif()
