

file(GLOB HSOFTPLC_C_FILES *.h *.c)
add_library(hsoftplc STATIC EXCLUDE_FROM_ALL ${HSOFTPLC_C_FILES})
set_property(TARGET hsoftplc PROPERTY C_STANDARD 99)
target_include_directories(hsoftplc PUBLIC ${CMAKE_CURRENT_LIST_DIR}/)
target_link_libraries(hsoftplc HBox)

