cmake_minimum_required(VERSION 3.0)

set(SOURCES
  crt0.s
  smpspr80.c
  ${PROJECT_SOURCE_DIR}/v_blank/v_blank.c
  ${PROJECT_SOURCE_DIR}/v_blank/set_vb.c
  )

add_compile_definitions(MODEL_S _SH)

add_executable( smpspr8.elf ${SOURCES} )


target_include_directories(smpspr8.elf PUBLIC .
    ${SEGALIB}/include
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/per/smpclib)

target_compile_options(smpspr8.elf PUBLIC -g -O0 -Wall -m2)

target_link_options(smpspr8.elf PUBLIC
    "SHELL:-Xlinker -Map -Xlinker ${OUTPUT_NAME}.map"
    "SHELL:-Xlinker --strip-debug"
    "SHELL:-m2 -nostartfiles"
    "SHELL:-T${CMAKE_CURRENT_SOURCE_DIR}/Saturn.lnk"
    "SHELL:-lc -lgcc -lm"
)

target_link_libraries(smpspr8.elf PUBLIC ${SEGASGL}/lib/libsgl.a )
target_link_libraries(smpspr8.elf PUBLIC ${SEGALIB}/lib/sega_spr.a )
target_link_libraries(smpspr8.elf PUBLIC ${SEGALIB}/lib/sega_scl.a )
target_link_libraries(smpspr8.elf PUBLIC ${SEGALIB}/lib/sega_mth.a )
target_link_libraries(smpspr8.elf PUBLIC ${SEGALIB}/lib/sega_per.a )
target_link_libraries(smpspr8.elf PUBLIC ${SEGALIB}/lib/sega_dma.a )
target_link_libraries(smpspr8.elf PUBLIC ${SEGALIB}/lib/sega_dsp.a )

add_custom_target(run_smpspr8 ALL DEPENDS smpspr8.elf smpspr8.bin)

add_custom_command(OUTPUT smpspr8.bin
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMAND ${CMAKE_OBJCOPY}
                   ARGS -O binary smpspr8.elf smpspr8.bin )

install(TARGETS smpspr8.elf DESTINATION ${SEGASMP_DESTINATION}/spr/smpspr8 )
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/smpspr8.bin DESTINATION ${SEGASMP_DESTINATION}/spr/smpspr8 )
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT_NAME}.map DESTINATION ${SEGASMP_DESTINATION}/spr/smpspr8 )
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION ${SEGASMP_DESTINATION}/spr/smpspr8/Sources )
