set(SOURCES
    int.c
    )

add_compile_definitions(_SH MODEL_S)

add_library( sega_int STATIC
             ${SOURCES}
             ${CMAKE_CURRENT_SOURCE_DIR}/int_trmp.o
             )

include_directories(${PROJECT_SOURCE_DIR}/include)

SET_TARGET_PROPERTIES(sega_int PROPERTIES PREFIX "")

#target_link_libraries(sega_int PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/int_trmp.o )

install(TARGETS sega_int DESTINATION ${SEGALIB_DESTINATION_LIB})
