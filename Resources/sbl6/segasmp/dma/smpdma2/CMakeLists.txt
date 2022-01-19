set(SOURCES
  crt0.s
  smpdma2.c
  #${PROJECT_SOURCE_DIR}/lib/strt1_g.src
  #${PROJECT_SOURCE_DIR}/lib/strt2_g.c
  ${PROJECT_SOURCE_DIR}/v_blank/v_blank.c
  ${PROJECT_SOURCE_DIR}/v_blank/set_vb.c
  )

add_compile_definitions(MODEL_S _SH)


add_executable( smpdma2.elf ${SOURCES} )

target_include_directories(smpdma2.elf PUBLIC . ${SEGALIB}/include
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/per/smpclib)

target_compile_options(smpdma2.elf PUBLIC -Wall -m2 -MMD)

target_link_options(smpdma2.elf PUBLIC
    "SHELL:-Xlinker -Map -Xlinker ${OUTPUT_NAME}.map"
    "SHELL:-Xlinker --strip-debug"
    "SHELL:-m2 -nostartfiles"
    "SHELL:-T${CMAKE_CURRENT_SOURCE_DIR}/Saturn.lnk"
    "SHELL:-lc -lgcc -lm"
)

target_link_libraries(smpdma2.elf PUBLIC ${SEGALIB}/lib/sega_dma.a )
target_link_libraries(smpdma2.elf PUBLIC ${SEGALIB}/lib/sega_csh.a )

add_custom_target(run_dma2 ALL DEPENDS smpdma2.elf smpdma2.bin)

add_custom_command(OUTPUT smpdma2.bin
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMAND ${CMAKE_OBJCOPY}
                   ARGS -O binary smpdma2.elf smpdma2.bin )

install(TARGETS smpdma2.elf DESTINATION ${SEGASMP_DESTINATION}/dma/smpdma2 )
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/smpdma2.bin DESTINATION ${SEGASMP_DESTINATION}/dma/smpdma2 )
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION ${SEGASMP_DESTINATION}/dma/smpdma2/Sources )
