set(SOURCES per_x1.c per_x2.c per_x3.src)

add_compile_definitions(_SH MODEL_S)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_library( sega_clib STATIC
             ${SOURCES} )

SET_TARGET_PROPERTIES(sega_clib PROPERTIES PREFIX "")

install(TARGETS sega_clib DESTINATION ${SEGALIB_DESTINATION_LIB})
