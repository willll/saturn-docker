set(SOURCES
  crt0.s
  smpdma12.c
  #${PROJECT_SOURCE_DIR}/lib/strt1_g.src
  #${PROJECT_SOURCE_DIR}/lib/strt2_g.c
  ${PROJECT_SOURCE_DIR}/v_blank/v_blank.c
  ${PROJECT_SOURCE_DIR}/v_blank/set_vb.c
  )

add_compile_definitions(MODEL_S _SH)


add_executable( smpdma12.elf ${SOURCES} )

target_include_directories(smpdma12.elf PUBLIC . ${SEGALIB}/include
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/per/smpclib)

target_compile_options(smpdma12.elf PUBLIC -Wall -m2 -MMD)

target_link_options(smpdma12.elf PUBLIC
    "SHELL:-Xlinker -Map -Xlinker ${OUTPUT_NAME}.map"
    "SHELL:-Xlinker --strip-debug"
    "SHELL:-m2 -nostartfiles"
    "SHELL:-T${CMAKE_CURRENT_SOURCE_DIR}/Saturn.lnk"
    "SHELL:-lc -lgcc -lm"
)

target_link_libraries(smpdma12.elf PUBLIC ${SEGALIB}/lib/sega_dma.a )
target_link_libraries(smpdma12.elf PUBLIC ${SEGALIB}/lib/sega_csh.a )
target_link_libraries(smpdma12.elf PUBLIC ${SEGALIB}/lib/sega_int.a )

add_custom_target(run_dma12 ALL DEPENDS smpdma12.elf smpdma12.bin)

add_custom_command(OUTPUT smpdma12.bin
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMAND ${CMAKE_OBJCOPY}
                   ARGS -O binary smpdma12.elf smpdma12.bin )

install(TARGETS smpdma12.elf DESTINATION ${SEGASMP_DESTINATION}/dma/smpdma12 )
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/smpdma12.bin DESTINATION ${SEGASMP_DESTINATION}/dma/smpdma12 )
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION ${SEGASMP_DESTINATION}/dma/smpdma12/Sources )
