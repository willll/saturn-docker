set(SOURCES mpg_loc.h mpg_mve.c mpg_mve2.c mpg_spc.c mpg_sys.c mpg_win.c sega_mpg.h)

add_compile_definitions(_SH MODEL_S)

ADD_LIBRARY( sega_mpg.a STATIC
             ${SOURCES} )

include_directories(${PROJECT_SOURCE_DIR}/include)
