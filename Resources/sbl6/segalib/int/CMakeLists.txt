set(SOURCES int.c)

add_compile_definitions(_SH MODEL_S)

ADD_LIBRARY( sega_int.a STATIC
             ${SOURCES} )

include_directories(${PROJECT_SOURCE_DIR}/include)
