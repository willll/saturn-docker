set(SOURCES
  crt0.s
  smppcm13.c
  )

add_compile_definitions(MODEL_S)


add_executable( smppcm13.elf ${SOURCES} )

target_include_directories(smppcm13.elf PUBLIC . ${SEGALIB}/include
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/per/smpclib)

target_compile_options(smppcm13.elf PUBLIC -Wall -m2 -MMD)

target_link_options(smppcm13.elf PUBLIC
    "SHELL:-Xlinker -Map -Xlinker ${OUTPUT_NAME}.map"
    "SHELL:-Xlinker --strip-debug"
    "SHELL:-m2 -nostartfiles"
    "SHELL:-T${CMAKE_CURRENT_SOURCE_DIR}/Saturn.lnk"
    "SHELL:-lc -lgcc -lm"
)

target_link_libraries(smppcm13.elf PUBLIC ${SEGALIB}/lib/sega_sgl.a )
target_link_libraries(smppcm13.elf PUBLIC ${SEGALIB}/lib/sega_sat.a )
target_link_libraries(smppcm13.elf PUBLIC ${SEGALIB}/lib/sega_cdc.a )
target_link_libraries(smppcm13.elf PUBLIC ${SEGALIB}/lib/sega_adp.a )
target_link_libraries(smppcm13.elf PUBLIC ${SEGALIB}/lib/sega_per.a )

add_custom_target(run_smppcm13 ALL DEPENDS smppcm13.elf smppcm13.bin)

add_custom_command(OUTPUT smppcm13.bin
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMAND ${CMAKE_OBJCOPY}
                   ARGS -O binary smppcm13.elf smppcm13.bin )

install(TARGETS smppcm13.elf DESTINATION ${SEGASMP_DESTINATION}/pcm/smppcm13 )
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/smppcm13.bin DESTINATION ${SEGASMP_DESTINATION}/pcm/smppcm13 )
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION ${SEGASMP_DESTINATION}/pcm/smppcm13/Sources )
