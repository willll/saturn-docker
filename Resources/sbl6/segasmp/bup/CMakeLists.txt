set(SOURCES
  crt0.s
  smpbup1.c font/fnt_cod0.c font/fnt_cod1.c font/fnt_dat0.c
  font/fnt_dat1.c font/fnt_dat2.c
  ${PROJECT_SOURCE_DIR}/v_blank/v_blank.c
  ${PROJECT_SOURCE_DIR}/v_blank/set_vb.c
  )

add_compile_definitions(MODEL_S _SH)


add_executable( bup.elf ${SOURCES} )

target_include_directories(bup.elf PUBLIC . ${SEGALIB}/include
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/per/smpclib)

target_compile_options(bup.elf PUBLIC -Wall -m2 -MMD)

target_link_options(bup.elf PUBLIC
    "SHELL:-Xlinker -Map -Xlinker ${OUTPUT_NAME}.map"
    "SHELL:-Xlinker --strip-debug"
    "SHELL:-Xlinker -fno-lto"
    "SHELL:-m2 -nostartfiles"
    "SHELL:-T${CMAKE_CURRENT_SOURCE_DIR}/Saturn.lnk"
    "SHELL:-lc -lgcc -lm"
)

target_link_libraries(bup.elf PUBLIC ${SEGALIB}/lib/sega_int.a )
target_link_libraries(bup.elf PUBLIC ${SEGALIB}/lib/sega_scl.a )
target_link_libraries(bup.elf PUBLIC ${SEGALIB}/lib/sega_mth.a )
target_link_libraries(bup.elf PUBLIC ${SEGALIB}/lib/sega_spr.a )
target_link_libraries(bup.elf PUBLIC ${SEGALIB}/lib/sega_cdc.a )
target_link_libraries(bup.elf PUBLIC ${SEGALIB}/lib/sega_per.a )


add_custom_target(run_bup ALL DEPENDS bup.elf bup.bin)

add_custom_command(OUTPUT bup.bin
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMAND ${CMAKE_OBJCOPY}
                   ARGS -O binary bup.elf bup.bin )

install(TARGETS bup.elf DESTINATION ${SEGASMP_DESTINATION}/bup )
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/bup.bin DESTINATION ${SEGASMP_DESTINATION}/bup )
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION ${SEGASMP_DESTINATION}/bup/Sources )
