set(SOURCES
  ${SATURN_COMMON}/crt0.s
  bup.c font/fnt_cod0.c font/fnt_cod1.c font/fnt_dat0.c
  font/fnt_dat1.c font/fnt_dat2.c
  v_blank/v_blank.c
  v_blank/set_vb.c
  )

set(CD_FILES
      0.bin
    )

set(TARGET_NAME bup)
set(TARGET_PATH bup)

add_compile_definitions(SATURN BYPASS_SCU_DMA MODEL_S _SH)

add_executable( ${TARGET_NAME}.elf ${SOURCES} )

target_include_directories(${TARGET_NAME}.elf PUBLIC
     .
     ${SEGALIB}/include
)

set(c_flags -Wall -m2 -MMD)

target_compile_options(${TARGET_NAME}.elf PUBLIC $<$<COMPILE_LANGUAGE:C>:${c_flags}>)

target_link_options(${TARGET_NAME}.elf PUBLIC
    "SHELL:-Xlinker -Map -Xlinker ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.map"
    "SHELL:-Xlinker --strip-debug"
    "SHELL:-Xlinker -fno-lto"
    "SHELL:-m2 -nostartfiles"
    "SHELL:-T${SATURN_COMMON}/Saturn.lnk"
    "SHELL: --specs=nosys.specs"
    #"SHELL: -lc -lgcc -lm"
)

target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_scl.a )
target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_mth.a )
target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_spr.a )
target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_cdc.a )
target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_per.a )
target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_int.a )

add_custom_target(run_${TARGET_NAME} ALL DEPENDS ${TARGET_NAME}.elf ${TARGET_NAME}.bin IP.BIN)

add_custom_command(OUTPUT ${TARGET_NAME}.bin
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMAND ${CMAKE_OBJCOPY}
                   ARGS -O binary ${TARGET_NAME}.elf ${TARGET_NAME}.bin )

add_custom_command(OUTPUT IP.BIN
                  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                  COMMAND $ENV{SATURN_IPMAKER}/IPMaker
                  ARGS -v -o ${CMAKE_CURRENT_BINARY_DIR}/IP.BIN
                        -t ${TARGET_NAME} -p 2 )

include(${SATURN_CMAKE}/install.cmake)
include(${SATURN_CMAKE}/create_cd.cmake)
