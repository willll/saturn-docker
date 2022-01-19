set(SOURCES
  crt0.s
  smpdbg10.c
  ${PROJECT_SOURCE_DIR}/v_blank/v_blank.c
  ${PROJECT_SOURCE_DIR}/v_blank/set_vb.c
  )

add_compile_definitions(MODEL_S _SH)


add_executable( smpdbg1.elf ${SOURCES} )

target_include_directories(smpdbg1.elf PUBLIC . ${SEGALIB}/include
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/per/smpclib)

target_compile_options(smpdbg1.elf PUBLIC -Wall -m2 -MMD)

target_link_options(smpdbg1.elf PUBLIC
    "SHELL:-Xlinker -Map -Xlinker ${OUTPUT_NAME}.map"
    "SHELL:-Xlinker --strip-debug"
    "SHELL:-m2 -nostartfiles"
    "SHELL:-T${CMAKE_CURRENT_SOURCE_DIR}/Saturn.lnk"
    "SHELL:-lc -lgcc -lm"
)

target_link_libraries(smpdbg1.elf PUBLIC ${SEGALIB}/lib/sega_per.a )
target_link_libraries(smpdbg1.elf PUBLIC ${SEGALIB}/lib/sega_sat.a )


add_custom_target(run_smpdbg1 ALL DEPENDS smpdbg1.elf smpdbg1.bin)

add_custom_command(OUTPUT smpdbg1.bin
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMAND ${CMAKE_OBJCOPY}
                   ARGS -O binary smpdbg1.elf smpdbg1.bin )

install(TARGETS smpdbg1.elf DESTINATION ${SEGASMP_DESTINATION}/smpdbg1 )
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/smpdbg1.bin DESTINATION ${SEGASMP_DESTINATION}/smpdbg1 )
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION ${SEGASMP_DESTINATION}/smpdbg1/Sources )
