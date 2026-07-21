

file(GLOB HOPENBLT_C_FILES *.h *.c 3rdpaty/*.h 3rdparty/*.c port/*.h port *.c)
add_library(hopenblt STATIC EXCLUDE_FROM_ALL ${HOPENBLT_C_FILES})
set_property(TARGET hopenblt PROPERTY C_STANDARD 99)
target_include_directories(hopenblt PUBLIC ${CMAKE_CURRENT_LIST_DIR}/3rdparty ${CMAKE_CURRENT_LIST_DIR}/port)
target_link_libraries(hopenblt HBox)

