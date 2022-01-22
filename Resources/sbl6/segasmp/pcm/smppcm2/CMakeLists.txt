set(SOURCES
  crt0.s
  smppcm2.c
  )

add_compile_definitions(MODEL_S)

add_executable( smppcm2.elf ${SOURCES} )

target_include_directories(smppcm2.elf PUBLIC . ${SEGALIB}/include
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/per/smpclib)

target_compile_options(smppcm2.elf PUBLIC -Wall -m2 -MMD)

target_link_options(smppcm2.elf PUBLIC
    "SHELL:-Xlinker -Map -Xlinker ${OUTPUT_NAME}.map"
    "SHELL:-Xlinker --strip-debug"
    "SHELL:-m2 -nostartfiles"
    "SHELL:-T${CMAKE_CURRENT_SOURCE_DIR}/Saturn.lnk"
    "SHELL:-lc -lgcc -lm"
)

target_link_libraries(smppcm2.elf PUBLIC ${SEGALIB}/lib/sega_sat.a )
#target_link_libraries(smppcm2.elf PUBLIC ${SEGALIB}/lib/sega_adp.a )
target_link_libraries(smppcm2.elf PUBLIC ${SEGALIB}/lib/sega_pcm.a )
target_link_libraries(smppcm2.elf PUBLIC ${SEGALIB}/lib/sega_cdc.a )
target_link_libraries(smppcm2.elf PUBLIC ${SEGALIB}/lib/sega_scl.a )
target_link_libraries(smppcm2.elf PUBLIC ${SEGALIB}/lib/sega_per.a )
target_link_libraries(smppcm2.elf PUBLIC ${SEGALIB}/lib/sega_int.a )


add_custom_target(run_smppcm2 ALL DEPENDS smppcm2.elf smppcm2.bin)

add_custom_command(OUTPUT smppcm2.bin
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMAND ${CMAKE_OBJCOPY}
                   ARGS -O binary smppcm2.elf smppcm2.bin )

install(TARGETS smppcm2.elf DESTINATION ${SEGASMP_DESTINATION}/pcm/smppcm2 )
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/smppcm2.bin DESTINATION ${SEGASMP_DESTINATION}/pcm/smppcm2 )
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION ${SEGASMP_DESTINATION}/pcm/smppcm2/Sources )
