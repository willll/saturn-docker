set(SOURCES dsp_ctrl.c)

add_compile_definitions(_SH MODEL_S)

ADD_LIBRARY( sega_dsp STATIC
             ${SOURCES} )

include_directories(${PROJECT_SOURCE_DIR}/include)

SET_TARGET_PROPERTIES(sega_dsp PROPERTIES PREFIX "")

install(TARGETS sega_dsp DESTINATION ${SEGALIB_DESTINATION_LIB})
