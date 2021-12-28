set(SOURCES pcm_lib.c
            pcm_gfs.c
            pcm_stm.c
            pcm_mp.c
            pcm_time.c
            pcm_audi.c
            pcm_aif.c
            pcm_aif2.c
            pcm_drv.c
            pcm_etc.c
            pcm_adp.c)

add_compile_definitions(_SH MODEL_S)

add_library( sega_pcm STATIC
             ${SOURCES} )

include_directories(${PROJECT_SOURCE_DIR}/include)

SET_TARGET_PROPERTIES(sega_pcm PROPERTIES PREFIX "")

install(TARGETS sega_pcm DESTINATION ${SEGALIB_DESTINATION_LIB})
