set(SOURCES bpl_main.c sega_bpl.h bpl_def.h)

add_compile_definitions(_SH MODEL_S)

ADD_LIBRARY( sega_bpl.a STATIC
             ${SOURCES} )

include_directories(${PROJECT_SOURCE_DIR}/include)
