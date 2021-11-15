set(SOURCES cmp.h cmplrun.c cmplrunb.c cmplrund.c cmplrunw.c cmpltool.c sega_cmp.h)

add_compile_definitions(_SH MODEL_S)

ADD_LIBRARY( sega_cmp.a STATIC
             ${SOURCES} )

include_directories(${PROJECT_SOURCE_DIR}/include)
