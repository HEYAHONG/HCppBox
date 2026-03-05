
cmake_minimum_required(VERSION 3.20)

project(rvvm)

#添加rvvm代码
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../common/  common EXCLUDE_FROM_ALL)


add_custom_target(rvvm_all ALL)

#编译rvvm库及rvvm_cli
add_dependencies(rvvm_all rvvm_cli)
add_dependencies(rvvm_all rvvm)


include(GNUInstallDirs)
if(DEFINED HTOOLS_INSTALL_DIR)
install(TARGETS rvvm rvvm_cli
       RUNTIME DESTINATION ${HTOOLS_INSTALL_DIR}/bin
       LIBRARY DESTINATION ${HTOOLS_INSTALL_DIR}/lib
       ARCHIVE DESTINATION ${HTOOLS_INSTALL_DIR}/lib
       )
install(DIRECTORY   ${CMAKE_CURRENT_LIST_DIR}/../../common/3rdparty/rvvm/include/
	DESTINATION ${HTOOLS_INSTALL_DIR}/include/)
endif()

