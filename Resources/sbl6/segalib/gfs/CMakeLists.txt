set(SOURCES sega_gfs.h gfs_def.h gfs_buf.h gfs_buf.c gfs_cdb.h gfs_cdb.c
  gfs_cdc.h gfs_cdc.c gfs_cdf.h gfs_cdf.c gfs_dir.c gfs_dir.h gfs_mmb.h gfs_mmb.c
  gfs_mmc.h gfs_mmc.c gfs_mmf.h gfs_mmf.c gfs_scsi.h gfs_sf.h gfs_sfb.c
  gfs_sff.c gfs_trn.c gfs_trn.h gfs.c)

add_compile_definitions(_SH MODEL_S DEBUG_LIB)

add_library( sega_gfs STATIC
             ${SOURCES} )

include_directories(${PROJECT_SOURCE_DIR}/include)

SET_TARGET_PROPERTIES(sega_gfs PROPERTIES PREFIX "")

install(TARGETS sega_gfs DESTINATION ${SEGALIB_DESTINATION_LIB})
