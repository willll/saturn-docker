set(SOURCES dbg_disp.c dbg_moni.c dbg_intr.s)

add_compile_definitions(_SH MODEL_S)

add_library( sega_dbg STATIC
             ${SOURCES} )

include_directories(${PROJECT_SOURCE_DIR}/include)

SET_TARGET_PROPERTIES(sega_dbg PROPERTIES PREFIX "")

install(TARGETS sega_dbg DESTINATION ${SEGALIB_DESTINATION_LIB})
