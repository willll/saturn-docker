set(SOURCES
  crt0.s
  smpdma1.c
  #${PROJECT_SOURCE_DIR}/lib/strt1_g.src
  #${PROJECT_SOURCE_DIR}/lib/strt2_g.c
  ${PROJECT_SOURCE_DIR}/v_blank/v_blank.c
  ${PROJECT_SOURCE_DIR}/v_blank/set_vb.c
  )

add_compile_definitions(MODEL_S _SH)


add_executable( smpdma1.elf ${SOURCES} )

target_include_directories(smpdma1.elf PUBLIC . ${SEGALIB}/include
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/per/smpclib)

target_compile_options(smpdma1.elf PUBLIC -Wall -m2 -MMD)

target_link_options(smpdma1.elf PUBLIC
    "SHELL:-Xlinker -Map -Xlinker ${OUTPUT_NAME}.map"
    "SHELL:-Xlinker --strip-debug"
    "SHELL:-m2 -nostartfiles"
    "SHELL:-T${CMAKE_CURRENT_SOURCE_DIR}/Saturn.lnk"
    "SHELL:-lc -lgcc -lm"
)

target_link_libraries(smpdma1.elf PUBLIC ${SEGALIB}/lib/sega_csh.a )
target_link_libraries(smpdma1.elf PUBLIC ${SEGALIB}/lib/sega_dma.a )

add_custom_target(run_dma1 ALL DEPENDS smpdma1.elf smpdma1.bin)

add_custom_command(OUTPUT smpdma1.bin
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMAND ${CMAKE_OBJCOPY}
                   ARGS -O binary smpdma1.elf smpdma1.bin )

install(TARGETS smpdma1.elf DESTINATION ${SEGASMP_DESTINATION}/dma/smpdma1 )
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/smpdma1.bin DESTINATION ${SEGASMP_DESTINATION}/dma/smpdma1 )
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION ${SEGASMP_DESTINATION}/dma/smpdma1/Sources )
