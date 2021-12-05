set(SOURCES ${PROJECT_SOURCE_DIR}/include/sega_spr.h
  spr_1c.c  spr_2c.c  spr_3c0.c  spr_3c1.c  spr_3c.c
          spr_3dsp.c spr_slv.c )

add_compile_definitions(_SH MODEL_S USE_DSP)

ADD_LIBRARY( sega_spr STATIC
             ${SOURCES} )

include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/gfs)

SET_TARGET_PROPERTIES(sega_spr PROPERTIES PREFIX "")

install(TARGETS sega_spr DESTINATION ${SEGALIB_DESTINATION_LIB})
