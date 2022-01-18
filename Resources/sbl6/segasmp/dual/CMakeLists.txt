cmake_minimum_required(VERSION 3.0)

set(SOURCES
  crt0.s
  ${PROJECT_SOURCE_DIR}/v_blank/v_blank.c
  ${PROJECT_SOURCE_DIR}/v_blank/set_vb.c
  slvsamp.c
  slavesh.c
  spr_xpt.c
  slv_mtx3.src
  spr_mtx3.src
  spr_fmat.src
  spr_sinc.src
  )

add_compile_definitions(MODEL_S _SH)

add_executable( dual.elf ${SOURCES} )

target_include_directories(dual.elf PUBLIC .
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${SEGALIB}/include
    ${SEGASGL}/include/sgl
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/per/smpclib)

target_compile_options(dual.elf PUBLIC -g -O0 -Wall -m2)

target_link_options(dual.elf PUBLIC
    "SHELL:-Xlinker -Map -Xlinker ${OUTPUT_NAME}.map"
    "SHELL:-Xlinker --strip-debug"
    "SHELL:-m2 -nostartfiles"
    "SHELL:-T${CMAKE_CURRENT_SOURCE_DIR}/Saturn.lnk"
    "SHELL:-lc -lgcc -lm"
)

target_link_libraries(dual.elf PUBLIC ${SEGASGL}/lib/libsgl.a )
#target_link_libraries(dual.elf PUBLIC ${SEGALIB}/lib/sega_sgl.a )
target_link_libraries(dual.elf PUBLIC ${SEGALIB}/lib/sega_scl.a )
target_link_libraries(dual.elf PUBLIC ${SEGALIB}/lib/sega_spr.a )
target_link_libraries(dual.elf PUBLIC ${SEGALIB}/lib/sega_mth.a )
#target_link_libraries(dual.elf PUBLIC ${SEGALIB}/lib/sega_cdc.a )
target_link_libraries(dual.elf PUBLIC ${SEGALIB}/lib/sega_per.a )
target_link_libraries(dual.elf PUBLIC ${SEGALIB}/lib/sega_int.a )
target_link_libraries(dual.elf PUBLIC ${SEGALIB}/lib/sega_dma.a )
target_link_libraries(dual.elf PUBLIC ${SEGALIB}/lib/sega_dsp.a )
#target_link_libraries(dual.elf PUBLIC ${SEGASGL}/lib/sglarea.o )


add_custom_target(run_dual ALL DEPENDS dual.elf dual.bin)

add_custom_command(OUTPUT dual.bin
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMAND ${CMAKE_OBJCOPY}
                   ARGS -O binary dual.elf dual.bin )

install(TARGETS dual.elf DESTINATION ${SEGASMP_DESTINATION}/sblsgl/dual )
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/dual.bin DESTINATION ${SEGASMP_DESTINATION}/sblsgl/dual )
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT_NAME}.map DESTINATION ${SEGASMP_DESTINATION}/sblsgl/dual )
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION ${SEGASMP_DESTINATION}/sblsgl/dual/Sources )
