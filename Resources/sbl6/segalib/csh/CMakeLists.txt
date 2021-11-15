set(SOURCES csh_main.c)

add_compile_definitions(_SH MODEL_S)

ADD_LIBRARY( sega_csh.a STATIC
             ${SOURCES} )

include_directories(${PROJECT_SOURCE_DIR}/include)
