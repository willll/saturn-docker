cmake_minimum_required(VERSION 3.0)

set(SOURCES
  crt0.s
  smpspr20.c
  ${PROJECT_SOURCE_DIR}/v_blank/v_blank.c
  ${PROJECT_SOURCE_DIR}/v_blank/set_vb.c
  )

add_compile_definitions(MODEL_S _SH)

add_executable( smpspr2.elf ${SOURCES} )

target_include_directories(smpspr2.elf PUBLIC . ${SEGALIB}/include
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/per/smpclib)

target_compile_options(smpspr2.elf PUBLIC -Wall -m2 -MMD)

target_link_options(smpspr2.elf PUBLIC
    "SHELL:-Xlinker -Map -Xlinker ${OUTPUT_NAME}.map"
    "SHELL:-Xlinker --strip-debug"
    "SHELL:-m2 -nostartfiles"
    "SHELL:-T${CMAKE_CURRENT_SOURCE_DIR}/Saturn.lnk"
    "SHELL:-lc -lgcc -lm"
)

target_link_libraries(smpspr2.elf PUBLIC ${SEGALIB}/lib/sega_int.a )
target_link_libraries(smpspr2.elf PUBLIC ${SEGALIB}/lib/sega_scl.a )
target_link_libraries(smpspr2.elf PUBLIC ${SEGALIB}/lib/sega_mth.a )
target_link_libraries(smpspr2.elf PUBLIC ${SEGALIB}/lib/sega_spr.a )
#target_link_libraries(smpspr2.elf PUBLIC ${SEGALIB}/lib/sega_cdc.a )
target_link_libraries(smpspr2.elf PUBLIC ${SEGALIB}/lib/sega_per.a )

add_custom_target(run_smpspr2 ALL DEPENDS smpspr2.elf smpspr2.bin)

add_custom_command(OUTPUT smpspr2.bin
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMAND ${CMAKE_OBJCOPY}
                   ARGS -O binary smpspr2.elf smpspr2.bin )

install(TARGETS smpspr2.elf DESTINATION ${SEGASMP_DESTINATION}/spr/smpspr2 )
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/smpspr2.bin DESTINATION ${SEGASMP_DESTINATION}/spr/smpspr2 )
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT_NAME}.map DESTINATION ${SEGASMP_DESTINATION}/spr/smpspr2 )
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION ${SEGASMP_DESTINATION}/spr/smpspr2/Sources )
