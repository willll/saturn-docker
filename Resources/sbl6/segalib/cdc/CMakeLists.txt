set(SOURCES cd_int.h  cdc_bif.c  cdc_cmn.c  cdc_mbu.c
          cdc_mls.c  cdc_mwn.c  cdc_sel.c
          cdc_bio.c  cdc_dev.c  cdc_mdc.c  cdc_msb.c  cdc_reg.c  cdc_sub.c
          cdc_cfs.c  cdc_drv.c  cdc_mfb.c  cdc_mst.c  cdc_scd.c  cdc_unk.c)

add_compile_definitions(_SH MODEL_S)

add_library( sega_cdc STATIC
             ${SOURCES} )

include_directories(${PROJECT_SOURCE_DIR}/include)

SET_TARGET_PROPERTIES(sega_cdc PROPERTIES PREFIX "")

install(TARGETS sega_cdc DESTINATION ${SEGALIB_DESTINATION_LIB})
