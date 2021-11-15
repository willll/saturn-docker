set(SOURCES mth_curv.c mth_dsp.c mth_dsps.c mth_rand.c )

add_compile_definitions(_SH MODEL_S)

ADD_LIBRARY( sega_mth.a STATIC
             ${SOURCES} )

include_directories(${PROJECT_SOURCE_DIR}/include)
