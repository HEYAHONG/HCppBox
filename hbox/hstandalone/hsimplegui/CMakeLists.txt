

file(GLOB HSIMPLEGUI_C_FILES *.h *.c 3rdpaty/*.h 3rdparty/*.c port/*.h port/*.c)
add_library(hsimplegui STATIC EXCLUDE_FROM_ALL ${HSIMPLEGUI_C_FILES})
set_property(TARGET hsimplegui PROPERTY C_STANDARD 99)
target_include_directories(hsimplegui PUBLIC ${CMAKE_CURRENT_LIST_DIR}/3rdparty/source ${CMAKE_CURRENT_LIST_DIR}/port)
target_link_libraries(hsimplegui HBox)

