set(SOURCES mpg_mve.c mpg_mve2.c mpg_spc.c mpg_sys.c mpg_win.c )

add_compile_definitions(_SH MODEL_S)

add_library( sega_mpg STATIC
             ${SOURCES} )

include_directories(${PROJECT_SOURCE_DIR}/include)

SET_TARGET_PROPERTIES(sega_mpg PROPERTIES PREFIX "")

install(TARGETS sega_mpg DESTINATION ${SEGALIB_DESTINATION_LIB})
