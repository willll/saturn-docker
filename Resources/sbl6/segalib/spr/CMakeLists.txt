set(SOURCES spr_1c.c spr_2a.s spr_2c.c spr_3a.s spr_3c.c)

add_compile_definitions(_SH MODEL_S USE_DSP)

add_library( sega_spr STATIC
             ${SOURCES} )

include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/gfs)

SET_TARGET_PROPERTIES(sega_spr PROPERTIES PREFIX "")

install(TARGETS sega_spr DESTINATION ${SEGALIB_DESTINATION_LIB})
