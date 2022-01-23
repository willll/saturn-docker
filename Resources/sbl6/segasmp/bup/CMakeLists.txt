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
    "SHELL:-Xlinker -Map -Xlinker bup.map"
    "SHELL:-Xlinker --strip-debug"
    "SHELL:-Xlinker -fno-lto"
    "SHELL:-m2 -nostartfiles"
    "SHELL:-T${CMAKE_CURRENT_SOURCE_DIR}/Saturn.lnk"
    "SHELL:-lc -lgcc -lm"
)

target_link_libraries(bup.elf PUBLIC ${SEGALIB}/lib/sega_scl.a )
target_link_libraries(bup.elf PUBLIC ${SEGALIB}/lib/sega_mth.a )
target_link_libraries(bup.elf PUBLIC ${SEGALIB}/lib/sega_spr.a )
target_link_libraries(bup.elf PUBLIC ${SEGALIB}/lib/sega_cdc.a )
target_link_libraries(bup.elf PUBLIC ${SEGALIB}/lib/sega_per.a )
target_link_libraries(bup.elf PUBLIC ${SEGALIB}/lib/sega_int.a )

add_custom_target(run_bup ALL DEPENDS bup.elf bup.bin)

add_custom_command(OUTPUT bup.bin
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMAND ${CMAKE_OBJCOPY}
                   ARGS -O binary bup.elf bup.bin )

install(TARGETS bup.elf DESTINATION ${SEGASMP_DESTINATION}/bup )

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/bup.bin
              ${CMAKE_CURRENT_BINARY_DIR}/bup.map
        DESTINATION ${SEGASMP_DESTINATION}/bup )

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/bup.bin
          DESTINATION ${SEGASMP_DESTINATION}/bup/CD
          RENAME 0.bin )
install(FILES $ENV{SATURN_CD}/CPY.txt
              $ENV{SATURN_CD}/BIB.txt
              $ENV{SATURN_CD}/ABS.txt
              ${CMAKE_CURRENT_SOURCE_DIR}/IP.BIN
              DESTINATION ${SEGASMP_DESTINATION}/bup/CD )
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/bup.cue DESTINATION ${SEGASMP_DESTINATION}/bup )
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/cd_included_files.txt DESTINATION ${SEGASMP_DESTINATION}/bup/CD
          RENAME FILES.txt )
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/cd_excludes_files.txt DESTINATION ${SEGASMP_DESTINATION}/bup/CD
          RENAME EXCLUDES.txt )

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION ${SEGASMP_DESTINATION}/bup/Sources)

set(MAKE_CMD $ENV{SATURN_CD}/create_cd.sh
    "bup" "XXX" "YYY" "IP.BIN"
    "${CMAKE_INSTALL_PREFIX}/${SEGASMP_DESTINATION}/bup/bup.iso" )

install(CODE "execute_process(COMMAND
                                ${MAKE_CMD}
                                WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/${SEGASMP_DESTINATION}/bup/CD
                              )"
        )
