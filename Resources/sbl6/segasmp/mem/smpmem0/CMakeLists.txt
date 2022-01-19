set(SOURCES
  crt0.s
  smpmem0.c
  ${PROJECT_SOURCE_DIR}/v_blank/v_blank.c
  ${PROJECT_SOURCE_DIR}/v_blank/set_vb.c
  )

add_compile_definitions(MODEL_S _SH)


add_executable( smpmem0.elf ${SOURCES} )

target_include_directories(smpmem0.elf PUBLIC . ${SEGALIB}/include
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/per/smpclib)

target_compile_options(smpmem0.elf PUBLIC -Wall -m2 -MMD)

target_link_options(smpmem0.elf PUBLIC
    "SHELL:-Xlinker -Map -Xlinker ${OUTPUT_NAME}.map"
    "SHELL:-Xlinker --strip-debug"
    "SHELL:-m2 -nostartfiles"
    "SHELL:-T${CMAKE_CURRENT_SOURCE_DIR}/Saturn.lnk"
    "SHELL:-lc -lgcc -lm"
)

target_link_libraries(smpmem0.elf PUBLIC ${SEGALIB}/lib/sega_mem.a )

add_custom_target(run_smpmem0 ALL DEPENDS smpmem0.elf smpmem0.bin)

add_custom_command(OUTPUT smpmem0.bin
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMAND ${CMAKE_OBJCOPY}
                   ARGS -O binary smpmem0.elf smpmem0.bin )

install(TARGETS smpmem0.elf DESTINATION ${SEGASMP_DESTINATION}/mem/smpmem0 )
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/smpmem0.bin DESTINATION ${SEGASMP_DESTINATION}/mem/smpmem0 )
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION ${SEGASMP_DESTINATION}/mem/smpmem0/Sources )
