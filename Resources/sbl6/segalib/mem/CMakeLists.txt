set(SOURCES mem_call.c mem_free.c mem_init.c mem_mall.c mem_real.c sega_mem.h)

add_compile_definitions(_SH MODEL_S)

ADD_LIBRARY( sega_mem STATIC
             ${SOURCES} )

include_directories(${PROJECT_SOURCE_DIR}/include)

SET_TARGET_PROPERTIES(sega_mem PROPERTIES PREFIX "")

install(TARGETS sega_mem DESTINATION ${SEGALIB_DESTINATION_LIB})
