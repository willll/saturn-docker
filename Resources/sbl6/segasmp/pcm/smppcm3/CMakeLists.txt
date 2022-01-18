set(SOURCES
  crt0.s
  smppcm3.c
  )

add_compile_definitions(MODEL_S)


add_executable( smppcm3.elf ${SOURCES} )

target_include_directories(smppcm3.elf PUBLIC . ${SEGALIB}/include
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/per/smpclib)

target_compile_options(smppcm3.elf PUBLIC -Wall -m2 -MMD)

target_link_options(smppcm3.elf PUBLIC
    "SHELL:-Xlinker -Map -Xlinker ${OUTPUT_NAME}.map"
    "SHELL:-Xlinker --strip-debug"
    "SHELL:-m2 -nostartfiles"
    "SHELL:-T${CMAKE_CURRENT_SOURCE_DIR}/Saturn.lnk"
    "SHELL:-lc -lgcc -lm"
)

target_link_libraries(smppcm3.elf PUBLIC ${SEGALIB}/lib/sega_int.a )
target_link_libraries(smppcm3.elf PUBLIC ${SEGALIB}/lib/sega_scl.a )
target_link_libraries(smppcm3.elf PUBLIC ${SEGALIB}/lib/sega_mth.a )
target_link_libraries(smppcm3.elf PUBLIC ${SEGALIB}/lib/sega_spr.a )
target_link_libraries(smppcm3.elf PUBLIC ${SEGALIB}/lib/sega_per.a )
target_link_libraries(smppcm3.elf PUBLIC ${SEGALIB}/lib/sega_mem.a )
target_link_libraries(smppcm3.elf PUBLIC ${SEGALIB}/lib/sega_pcm.a )
target_link_libraries(smppcm3.elf PUBLIC ${SEGALIB}/lib/sega_stm.a )
target_link_libraries(smppcm3.elf PUBLIC ${SEGALIB}/lib/sega_gfs.a )
target_link_libraries(smppcm3.elf PUBLIC ${SEGALIB}/lib/sega_dma.a )
target_link_libraries(smppcm3.elf PUBLIC ${SEGALIB}/lib/sega_csh.a )
target_link_libraries(smppcm3.elf PUBLIC ${SEGALIB}/lib/sega_cdc.a )
target_link_libraries(smppcm3.elf PUBLIC ${SEGALIB}/lib/sega_snd.a )
target_link_libraries(smppcm3.elf PUBLIC ${SEGASGL}/lib/libadp.a )

add_custom_target(run_smppcm3 ALL DEPENDS smppcm3.elf smppcm3.bin)

add_custom_command(OUTPUT smppcm3.bin
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMAND ${CMAKE_OBJCOPY}
                   ARGS -O binary smppcm3.elf smppcm3.bin )

install(TARGETS smppcm3.elf DESTINATION ${SEGASMP_DESTINATION}/pcm/smppcm3 )
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/smppcm3.bin DESTINATION ${SEGASMP_DESTINATION}/pcm/smppcm3 )
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION ${SEGASMP_DESTINATION}/pcm/smppcm3/Sources )
