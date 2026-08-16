

file(GLOB HFLASHDB_C_FILES *.h *.c 3rdpaty/*.h 3rdparty/*.c port/*.h port/*.c)
add_library(hflashdb STATIC EXCLUDE_FROM_ALL ${HFLASHDB_C_FILES})
set_property(TARGET hflashdb PROPERTY C_STANDARD 99)
target_include_directories(hflashdb PUBLIC ${CMAKE_CURRENT_LIST_DIR}/3rdparty/source ${CMAKE_CURRENT_LIST_DIR}/port)
target_link_libraries(hflashdb HBox)

