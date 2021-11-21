set(SOURCES
 smpbup1.c font/fnt_cod0.c font/fnt_cod1.c font/fnt_dat0.c
  font/fnt_dat1.c font/fnt_dat2.c
  ${PROJECT_SOURCE_DIR}/v_blank/v_blank.c
  ${PROJECT_SOURCE_DIR}/v_blank/set_vb.c ${PROJECT_SOURCE_DIR}/per/smpclib/per_x2.c
  ${PROJECT_SOURCE_DIR}/per/smpclib/per_x1.c
  )

add_compile_definitions(MODEL_S)

add_executable( bup ${SOURCES} )

target_include_directories(bup PUBLIC . ${SEGALIB}/include
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/per/smpclib)

add_compile_options(-g -O0 -Wall -m2)

target_link_libraries(bup PUBLIC ${SEGALIB}/lib/sega_mth.a )
target_link_libraries(bup PUBLIC ${SEGALIB}/lib/sega_int.a )
target_link_libraries(bup PUBLIC ${SEGALIB}/lib/sega_scl.a )
target_link_libraries(bup PUBLIC ${SEGALIB}/lib/sega_spr.a )
target_link_libraries(bup PUBLIC ${SEGALIB}/lib/sega_cdc.a )
#target_link_libraries(bup PUBLIC ${SEGALIB}/lib/sega_adp.a )
target_link_libraries(bup PUBLIC ${SEGALIB}/lib/sega_per.a )
