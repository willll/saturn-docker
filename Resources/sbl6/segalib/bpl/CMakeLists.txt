set(SOURCES bpl_main.c sega_bpl.h bpl_def.h)

add_compile_definitions(_SH MODEL_S)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_library( sega_bpl STATIC
             ${SOURCES} )

SET_TARGET_PROPERTIES(sega_bpl PROPERTIES PREFIX "")

install(TARGETS sega_bpl DESTINATION ${SEGALIB_DESTINATION_LIB})
