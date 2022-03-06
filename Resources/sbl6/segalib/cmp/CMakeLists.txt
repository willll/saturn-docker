set(SOURCES cmplrun.c cmplrunb.c cmplrund.c cmplrunw.c cmpltool.c)

add_compile_definitions(_SH MODEL_S)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_library( sega_cmp STATIC
             ${SOURCES} )

SET_TARGET_PROPERTIES(sega_cmp PROPERTIES PREFIX "")

install(TARGETS sega_cmp DESTINATION ${SEGALIB_DESTINATION_LIB})
