set(SOURCES per_prt_new.c )

add_compile_definitions(_SH MODEL_S)

add_library( sega_per STATIC
             ${SOURCES}
             ${CMAKE_CURRENT_SOURCE_DIR}/per_prt.o
            )

include_directories(${PROJECT_SOURCE_DIR}/include)

set_target_properties(sega_per PROPERTIES PREFIX "")

install(TARGETS sega_per DESTINATION ${SEGALIB_DESTINATION_LIB})
