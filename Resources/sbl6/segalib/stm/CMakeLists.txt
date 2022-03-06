set(SOURCES stm_acc.c  stm_init.c
          stm_loc.c  stm.shc  stm_svr.c  stm_trn.c)

add_compile_definitions(_SH MODEL_S)

add_library( sega_stm STATIC
             ${SOURCES} )

include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/gfs)

SET_TARGET_PROPERTIES(sega_stm PROPERTIES PREFIX "")

install(TARGETS sega_stm DESTINATION ${SEGALIB_DESTINATION_LIB})
