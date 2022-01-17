set(SOURCES
  crt0.s
  smpcsh.c
  #${PROJECT_SOURCE_DIR}/lib/strt2_g.c
  #${PROJECT_SOURCE_DIR}/lib/strt1_g.src
  )

add_compile_definitions(MODEL_S _SH)


add_executable( csh.elf ${SOURCES} )

target_include_directories(csh.elf PUBLIC . ${SEGALIB}/include )

target_compile_options(csh.elf PUBLIC -Wall -m2 -MMD)

target_link_options(csh.elf PUBLIC
    "SHELL:-Xlinker -Map -Xlinker ${OUTPUT_NAME}.map"
    "SHELL:-Xlinker --strip-debug"
    "SHELL:-m2 -nostartfiles"
    "SHELL:-T${CMAKE_CURRENT_SOURCE_DIR}/Saturn.lnk"
    "SHELL:-lc -lgcc -lm"
)

target_link_libraries(csh.elf PUBLIC ${SEGALIB}/lib/sega_scl.a )
target_link_libraries(csh.elf PUBLIC ${SEGALIB}/lib/sega_mth.a )
target_link_libraries(csh.elf PUBLIC ${SEGALIB}/lib/sega_csh.a )

add_custom_target(run_csh ALL DEPENDS csh.elf csh.bin)

add_custom_command(OUTPUT csh.bin
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMAND ${CMAKE_OBJCOPY}
                   ARGS -O binary csh.elf csh.bin )

install(TARGETS csh.elf DESTINATION ${SEGASMP_DESTINATION}/csh )
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/csh.bin DESTINATION ${SEGASMP_DESTINATION}/csh )
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION ${SEGASMP_DESTINATION}/csh/Sources )
