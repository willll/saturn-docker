set(SOURCES
  crt0.s
  smppcm4.c
  ${PROJECT_SOURCE_DIR}/v_blank/v_blank.c
  ${PROJECT_SOURCE_DIR}/v_blank/set_vb.c
  )

add_compile_definitions(MODEL_S)


add_executable( smppcm4.elf ${SOURCES} )

target_include_directories(smppcm4.elf PUBLIC . ${SEGALIB}/include
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/per/smpclib)

target_compile_options(smppcm4.elf PUBLIC -Wall -m2 -MMD)

target_link_options(smppcm4.elf PUBLIC
    "SHELL:-Xlinker -Map -Xlinker ${OUTPUT_NAME}.map"
    "SHELL:-Xlinker --strip-debug"
    "SHELL:-m2 -nostartfiles"
    "SHELL:-T${CMAKE_CURRENT_SOURCE_DIR}/Saturn.lnk"
    "SHELL:-lc -lgcc -lm"
)

target_link_libraries(smppcm4.elf PUBLIC ${SEGALIB}/lib/sega_sgl.a )
target_link_libraries(smppcm4.elf PUBLIC ${SEGALIB}/lib/sega_sat.a )
target_link_libraries(smppcm4.elf PUBLIC ${SEGALIB}/lib/sega_cdc.a )
target_link_libraries(smppcm4.elf PUBLIC ${SEGALIB}/lib/sega_adp.a )
target_link_libraries(smppcm4.elf PUBLIC ${SEGALIB}/lib/sega_per.a )

add_custom_target(run_smppcm4 ALL DEPENDS smppcm4.elf smppcm4.bin)

add_custom_command(OUTPUT smppcm4.bin
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMAND ${CMAKE_OBJCOPY}
                   ARGS -O binary smppcm4.elf smppcm4.bin )

install(TARGETS smppcm4.elf DESTINATION ${SEGASMP_DESTINATION}/pcm/smppcm4 )
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/smppcm4.bin DESTINATION ${SEGASMP_DESTINATION}/pcm/smppcm4 )
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION ${SEGASMP_DESTINATION}/pcm/smppcm4/Sources )
