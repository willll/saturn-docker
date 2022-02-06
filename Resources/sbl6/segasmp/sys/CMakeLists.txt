set(SOURCES
  crt0.s
  sys_id.src
  smpsys.c
  )

add_compile_definitions(MODEL_S _SH)

add_executable( smpsys.elf ${SOURCES} )

target_include_directories(smpsys.elf PUBLIC . ${SEGALIB}/include )

target_compile_options(smpsys.elf PUBLIC -O2 -Wall -m2 -MMD)

target_link_options(smpsys.elf PUBLIC
    "SHELL:-Xlinker -Map -Xlinker smpsys.map"
    "SHELL:-Xlinker --strip-debug"
    "SHELL:-m2 -nostartfiles"
    "SHELL:-T${CMAKE_CURRENT_SOURCE_DIR}/Saturn.lnk"
    "SHELL:-lc -lgcc -lm"
)

target_link_libraries(smpsys.elf PUBLIC ${SEGALIB}/lib/sega_sat.a )

add_custom_target(run_smpsys ALL DEPENDS smpsys.elf smpsys.bin)

add_custom_command(OUTPUT smpsys.bin
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMAND ${CMAKE_OBJCOPY}
                   ARGS -O binary smpsys.elf smpsys.bin )

install(TARGETS smpsys.elf DESTINATION ${SEGASMP_DESTINATION}/sys )
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/smpsys.bin DESTINATION ${SEGASMP_DESTINATION}/sys )
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION ${SEGASMP_DESTINATION}/sys/Sources )
