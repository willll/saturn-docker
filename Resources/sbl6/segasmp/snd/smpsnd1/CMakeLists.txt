set(SOURCES
  crt0.s
  smpsnd1.c
  )

add_compile_definitions(MODEL_S)

add_executable( smpsnd1.elf ${SOURCES} )

target_include_directories(smpsnd1.elf PUBLIC . ${SEGALIB}/include
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/per/smpclib)

target_compile_options(smpsnd1.elf PUBLIC -Wall -m2 -MMD)

target_link_options(smpsnd1.elf PUBLIC
    "SHELL:-Xlinker -Map -Xlinker ${OUTPUT_NAME}.map"
    "SHELL:-Xlinker --strip-debug"
    "SHELL:-m2 -nostartfiles"
    "SHELL:-T${CMAKE_CURRENT_SOURCE_DIR}/Saturn.lnk"
    "SHELL:-lc -lgcc -lm"
)

target_link_libraries(smpsnd1.elf PUBLIC ${SEGALIB}/lib/sega_sat.a )


add_custom_target(run_smpsnd1 ALL DEPENDS smpsnd1.elf smpsnd1.bin)

add_custom_command(OUTPUT smpsnd1.bin
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMAND ${CMAKE_OBJCOPY}
                   ARGS -O binary smpsnd1.elf smpsnd1.bin )

install(TARGETS smpsnd1.elf DESTINATION ${SEGASMP_DESTINATION}/pcm/smpsnd1 )
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/smpsnd1.bin DESTINATION ${SEGASMP_DESTINATION}/pcm/smpsnd1 )
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION ${SEGASMP_DESTINATION}/pcm/smpsnd1/Sources )
