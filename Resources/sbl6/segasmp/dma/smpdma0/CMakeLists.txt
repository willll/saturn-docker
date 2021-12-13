set(SOURCES
  crt0.s
  dma0.c
  font/fnt_dat1.c font/fnt_dat2.c
  ${PROJECT_SOURCE_DIR}/v_blank/v_blank.c
  ${PROJECT_SOURCE_DIR}/v_blank/set_vb.c
  )

add_compile_definitions(MODEL_S _SH)


add_executable( dma0.elf ${SOURCES} )

target_include_directories(dma0.elf PUBLIC . ${SEGALIB}/include
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/per/smpclib)

target_compile_options(dma0.elf PUBLIC -Wall -m2 -MMD)

target_link_options(dma0.elf PUBLIC
    "SHELL:-Xlinker -Map -Xlinker ${OUTPUT_NAME}.map"
    "SHELL:-Xlinker --strip-debug"
    "SHELL:-m2 -nostartfiles"
    "SHELL:-T${CMAKE_CURRENT_SOURCE_DIR}/Saturn.lnk"
    "SHELL:-lc -lgcc -lm"
)

target_link_libraries(dma0.elf PUBLIC ${SEGALIB}/lib/sega_xpt.a )
target_link_libraries(dma0.elf PUBLIC ${SEGALIB}/lib/sega_csh.a )
target_link_libraries(dma0.elf PUBLIC ${SEGALIB}/lib/sega_dma.a )

add_custom_target(run_dma0 ALL DEPENDS dma0.elf dma0.bin)

add_custom_command(OUTPUT dma0.bin
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMAND ${CMAKE_OBJCOPY}
                   ARGS -O binary dma0.elf dma0.bin )

install(TARGETS dma0.elf DESTINATION ${SEGASMP_DESTINATION}/dma/dma0 )
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/dma0.bin DESTINATION ${SEGASMP_DESTINATION}/dma/dma0 )
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION ${SAMPLES_DESTINATION}/dma/dma0/Sources )
