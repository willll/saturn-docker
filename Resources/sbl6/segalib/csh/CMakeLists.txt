set(SOURCES csh_main.c)

add_library( sega_csh STATIC
             ${SOURCES} )

add_compile_definitions(sega_csh PUBLIC _SH MODEL_S)

include_directories(${PROJECT_SOURCE_DIR}/include)

SET_TARGET_PROPERTIES(sega_csh PROPERTIES PREFIX "")

install(TARGETS sega_csh DESTINATION ${SEGALIB_DESTINATION_LIB})
