set(SOURCES gfs_buf.c gfs_cdb.c gfs_cdc.c gfs_cdf.c gfs_dir.c gfs_mmb.c
  gfs_mmc.c gfs_mmf.c gfs_sfb.c gfs_sff.c gfs_trn.c gfs.c)

add_compile_definitions(_SH MODEL_S DEBUG_LIB)

add_library( sega_gfs STATIC
             ${SOURCES} )

include_directories(${PROJECT_SOURCE_DIR}/include)

SET_TARGET_PROPERTIES(sega_gfs PROPERTIES PREFIX "")

install(TARGETS sega_gfs DESTINATION ${SEGALIB_DESTINATION_LIB})
