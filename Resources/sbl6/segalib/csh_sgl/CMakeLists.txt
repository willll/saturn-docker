set(SOURCES ${PROJECT_SOURCE_DIR}/csh/csh_main.c)

add_library( sega_csh_sgl STATIC
            ${SOURCES} )

add_compile_definitions(sega_csh_sgl PUBLIC _SH MODEL_S SGL)

include_directories(${PROJECT_SOURCE_DIR}/include)

SET_TARGET_PROPERTIES(sega_csh_sgl PROPERTIES PREFIX "")

install(TARGETS sega_csh_sgl DESTINATION ${SEGALIB_DESTINATION_LIB})
