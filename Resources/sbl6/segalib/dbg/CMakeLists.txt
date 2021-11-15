set(SOURCES dbg_disp.c dbg_moni.c)

add_compile_definitions(_SH MODEL_S)

ADD_LIBRARY( sega_dbg.a STATIC
             ${SOURCES} )

include_directories(${PROJECT_SOURCE_DIR}/include)
