set(SOURCES snd_main.c)

add_compile_definitions(_SH MODEL_S)

add_library( sega_snd STATIC
             ${SOURCES} )

include_directories(${PROJECT_SOURCE_DIR}/include)

SET_TARGET_PROPERTIES(sega_snd PROPERTIES PREFIX "")

install(TARGETS sega_snd DESTINATION ${SEGALIB_DESTINATION_LIB})
