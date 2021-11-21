set(SOURCES smpbup1.c)

add_compile_definitions(MODEL_S)

add_executable( bup ${SOURCES} )

target_include_directories(bup PUBLIC . ${SEGALIB}/include ${PROJECT_SOURCE_DIR}/per/smpclib)

add_compile_options(-g -O0 -Wall -m2)

target_link_libraries(bup PUBLIC ${SEGALIB}/lib/sega_int.a )
target_link_libraries(bup PUBLIC ${SEGALIB}/lib/sega_scl.a )
target_link_libraries(bup PUBLIC ${SEGALIB}/lib/sega_spr.a )
target_link_libraries(bup PUBLIC ${SEGALIB}/lib/sega_mth.a )
target_link_libraries(bup PUBLIC ${SEGALIB}/lib/sega_cdc.a )
#target_link_libraries(bup PUBLIC ${SEGALIB}/lib/sega_adp.a )
target_link_libraries(bup PUBLIC ${SEGALIB}/lib/sega_per.a )
