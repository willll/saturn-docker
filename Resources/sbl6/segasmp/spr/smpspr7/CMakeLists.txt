cmake_minimum_required(VERSION 3.0)

set(SOURCES
  crt0.s
  smpspr70.c
  ${PROJECT_SOURCE_DIR}/v_blank/v_blank.c
  ${PROJECT_SOURCE_DIR}/v_blank/set_vb.c
  )

add_compile_definitions(MODEL_S _SH)

add_executable( smpspr7.elf ${SOURCES} )

target_include_directories(smpspr7.elf PUBLIC .
    ${SEGALIB}/include
    ${SEGASGL}/include
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/per/smpclib)

target_compile_options(smpspr7.elf PUBLIC -g -O0 -Wall -m2)

target_link_options(smpspr7.elf PUBLIC
    "SHELL:-Xlinker -Map -Xlinker ${OUTPUT_NAME}.map"
    "SHELL:-Xlinker --strip-debug"
    "SHELL:-m2 -nostartfiles"
    "SHELL:-T${CMAKE_CURRENT_SOURCE_DIR}/Saturn.lnk"
    "SHELL:-lc -lgcc -lm"
)

target_link_libraries(smpspr7.elf PUBLIC ${SEGALIB}/lib/sega_spr.a )
target_link_libraries(smpspr7.elf PUBLIC ${SEGALIB}/lib/sega_scl.a )
target_link_libraries(smpspr7.elf PUBLIC ${SEGALIB}/lib/sega_mth.a )
#target_link_libraries(smpspr7.elf PUBLIC ${SEGALIB}/lib/sega_cdc.a )
target_link_libraries(smpspr7.elf PUBLIC ${SEGALIB}/lib/sega_per.a )
target_link_libraries(smpspr7.elf PUBLIC ${SEGALIB}/lib/sega_dma.a )
target_link_libraries(smpspr7.elf PUBLIC ${SEGALIB}/lib/sega_dsp.a )
target_link_libraries(smpspr7.elf PUBLIC ${SEGASGL}/lib/libsgl.a )


add_custom_target(run_smpspr7 ALL DEPENDS smpspr7.elf smpspr7.bin)

add_custom_command(OUTPUT smpspr7.bin
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMAND ${CMAKE_OBJCOPY}
                   ARGS -O binary smpspr7.elf smpspr7.bin )

install(TARGETS smpspr7.elf DESTINATION ${SEGASMP_DESTINATION}/spr/smpspr7 )
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/smpspr7.bin DESTINATION ${SEGASMP_DESTINATION}/spr/smpspr7 )
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT_NAME}.map DESTINATION ${SEGASMP_DESTINATION}/spr/smpspr7 )
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION ${SEGASMP_DESTINATION}/spr/smpspr7/Sources )
