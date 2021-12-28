set(SOURCES per_def.h  per_prt.h  per_smp.h  per_xpt.h per_prt.c )

add_compile_definitions(_SH MODEL_S)

add_library( sega_per STATIC
             ${SOURCES} )

include_directories(${PROJECT_SOURCE_DIR}/include)

SET_TARGET_PROPERTIES(sega_per PROPERTIES PREFIX "")

install(TARGETS sega_per DESTINATION ${SEGALIB_DESTINATION_LIB})
