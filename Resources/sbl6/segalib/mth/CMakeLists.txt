set(SOURCES mth_curv.c mth_dsp.c mth_dsps.c mth_rand.c
  mth_mtrx.s  mth_norm.s  mth_ps2d.s  mth_tri.s mth_fixd.s
 )

add_compile_definitions(_SH MODEL_S)

ADD_LIBRARY( sega_mth STATIC
             ${SOURCES} )

target_link_options(sega_mth PUBLIC)

include_directories(${PROJECT_SOURCE_DIR}/include)

SET_TARGET_PROPERTIES(sega_mth PROPERTIES PREFIX "")

install(TARGETS sega_mth DESTINATION ${SEGALIB_DESTINATION_LIB})
