

file(GLOB HSUNRISEANDSUNSET_C_FILES *.h *.c)
add_library(hsunriseandsunset STATIC EXCLUDE_FROM_ALL ${HSUNRISEANDSUNSET_C_FILES})
set_property(TARGET hsunriseandsunset PROPERTY C_STANDARD 99)
