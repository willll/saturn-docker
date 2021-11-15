set(SOURCES dsp_ctrl.c)

add_compile_definitions(_SH MODEL_S)

ADD_LIBRARY( sega_dsp.a STATIC
             ${SOURCES} )

include_directories(${PROJECT_SOURCE_DIR}/include)
