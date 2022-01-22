set(SOURCES
  crt0.s
  smppcm5.c
  ${PROJECT_SOURCE_DIR}/v_blank/v_blank.c
  ${PROJECT_SOURCE_DIR}/v_blank/set_vb.c
  )

add_compile_definitions(MODEL_S)


add_executable( smppcm5.elf ${SOURCES} )

target_include_directories(smppcm5.elf PUBLIC . ${SEGALIB}/include
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/per/smpclib)

target_compile_options(smppcm5.elf PUBLIC -Wall -m2 -MMD)

target_link_options(smppcm5.elf PUBLIC
    "SHELL:-Xlinker -Map -Xlinker ${OUTPUT_NAME}.map"
    "SHELL:-Xlinker --strip-debug"
    "SHELL:-m2 -nostartfiles"
    "SHELL:-T${CMAKE_CURRENT_SOURCE_DIR}/Saturn.lnk"
    "SHELL:-lc -lgcc -lm"
)

target_link_libraries(smppcm5.elf PUBLIC ${SEGALIB}/lib/sega_sgl.a )
target_link_libraries(smppcm5.elf PUBLIC ${SEGALIB}/lib/sega_sat.a )
target_link_libraries(smppcm5.elf PUBLIC ${SEGALIB}/lib/sega_cdc.a )
target_link_libraries(smppcm5.elf PUBLIC ${SEGALIB}/lib/sega_adp.a )
target_link_libraries(smppcm5.elf PUBLIC ${SEGALIB}/lib/sega_per.a )

add_custom_target(run_smppcm5 ALL DEPENDS smppcm5.elf smppcm5.bin)

add_custom_command(OUTPUT smppcm5.bin
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMAND ${CMAKE_OBJCOPY}
                   ARGS -O binary smppcm5.elf smppcm5.bin )

install(TARGETS smppcm5.elf DESTINATION ${SEGASMP_DESTINATION}/pcm/smppcm5 )
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/smppcm5.bin DESTINATION ${SEGASMP_DESTINATION}/pcm/smppcm5 )
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION ${SEGASMP_DESTINATION}/pcm/smppcm5/Sources )
