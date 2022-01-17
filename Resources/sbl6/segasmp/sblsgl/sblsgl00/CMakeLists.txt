cmake_minimum_required(VERSION 3.0)

set(SOURCES
  crt0.s
  ${PROJECT_SOURCE_DIR}/v_blank/v_blank.c
  ${PROJECT_SOURCE_DIR}/v_blank/set_vb.c
  polygon.c
  sblsgl00.c
  work.c
  )

add_compile_definitions(MODEL_S _SH)

add_executable( sblsgl00.elf ${SOURCES} )

target_include_directories(sblsgl00.elf PUBLIC .
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${SEGALIB}/include
    ${SEGASGL}/include/sgl
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/per/smpclib)

target_compile_options(sblsgl00.elf PUBLIC -g -O0 -Wall -m2)

target_link_options(sblsgl00.elf PUBLIC
    "SHELL:-Xlinker -Map -Xlinker ${OUTPUT_NAME}.map"
    "SHELL:-Xlinker --strip-debug"
    "SHELL:-m2 -nostartfiles"
    "SHELL:-T${CMAKE_CURRENT_SOURCE_DIR}/Saturn.lnk"
    "SHELL:-lc -lgcc -lm"
)

target_link_libraries(sblsgl00.elf PUBLIC ${SEGASGL}/lib/libsgl.a )
#target_link_libraries(sblsgl00.elf PUBLIC ${SEGALIB}/lib/sega_sgl.a )
target_link_libraries(sblsgl00.elf PUBLIC ${SEGALIB}/lib/sega_scl.a )
target_link_libraries(sblsgl00.elf PUBLIC ${SEGALIB}/lib/sega_spr.a )
target_link_libraries(sblsgl00.elf PUBLIC ${SEGALIB}/lib/sega_mth.a )
#target_link_libraries(sblsgl00.elf PUBLIC ${SEGALIB}/lib/sega_cdc.a )
target_link_libraries(sblsgl00.elf PUBLIC ${SEGALIB}/lib/sega_per.a )
target_link_libraries(sblsgl00.elf PUBLIC ${SEGALIB}/lib/sega_int.a )
target_link_libraries(sblsgl00.elf PUBLIC ${SEGALIB}/lib/sega_dma.a )
target_link_libraries(sblsgl00.elf PUBLIC ${SEGALIB}/lib/sega_dsp.a )
target_link_libraries(sblsgl00.elf PUBLIC ${SEGASGL}/lib/sglarea.o )


add_custom_target(run_sblsgl00 ALL DEPENDS sblsgl00.elf sblsgl00.bin)

add_custom_command(OUTPUT sblsgl00.bin
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMAND ${CMAKE_OBJCOPY}
                   ARGS -O binary sblsgl00.elf sblsgl00.bin )

install(TARGETS sblsgl00.elf DESTINATION ${SEGASMP_DESTINATION}/sblsgl/sblsgl00 )
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/sblsgl00.bin DESTINATION ${SEGASMP_DESTINATION}/sblsgl/sblsgl00 )
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT_NAME}.map DESTINATION ${SEGASMP_DESTINATION}/sblsgl/sblsgl00 )
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION ${SEGASMP_DESTINATION}/sblsgl/sblsgl00/Sources )
