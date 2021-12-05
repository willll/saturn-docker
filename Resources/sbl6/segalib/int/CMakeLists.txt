set(SOURCES int.c)

add_compile_definitions(_SH MODEL_S)

ADD_LIBRARY( sega_int STATIC
             ${SOURCES} )

include_directories(${PROJECT_SOURCE_DIR}/include)

SET_TARGET_PROPERTIES(sega_int PROPERTIES PREFIX "")

install(TARGETS sega_int DESTINATION ${SEGALIB_DESTINATION_LIB})
