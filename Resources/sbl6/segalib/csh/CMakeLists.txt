set(SOURCES csh_main.c)

add_compile_definitions(_SH MODEL_S)

add_library( sega_csh STATIC
             ${SOURCES} )

include_directories(${PROJECT_SOURCE_DIR}/include)

SET_TARGET_PROPERTIES(sega_csh PROPERTIES PREFIX "")

install(TARGETS sega_csh DESTINATION ${SEGALIB_DESTINATION_LIB})
