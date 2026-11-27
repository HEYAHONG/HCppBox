
if(HALIBC_POSIX_SOCKET_DISABLE_FALLBACK)
    add_library(halibc_posix_socket INTERFACE)
    target_include_directories(halibc_posix_socket INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include)
else()
    add_library(halibc_posix_socket STATIC)
    target_include_directories(halibc_posix_socket PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include)
    file(GLOB HALIBC_POSIX_SOCKET_FALLBACK_SOURCES
        CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/fallback/*.c
        )
    target_sources(halibc_posix_socket PRIVATE
        ${HALIBC_POSIX_SOCKET_FALLBACK_SOURCES}
        )
    target_include_directories(halibc_posix_socket PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/fallback
        )
    target_compile_definitions(halibc_posix_socket PRIVATE
        HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS=1
        )
endif()

if(TARGET c)
    if(HALIBC_POSIX_SOCKET_DISABLE_FALLBACK)
        target_link_libraries(halibc_posix_socket INTERFACE c)
    else()
        target_link_libraries(halibc_posix_socket PUBLIC c)
    endif()
endif()
