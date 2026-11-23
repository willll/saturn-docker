set(SOURCES
  ${SATURN_COMMON}/crt0.s
  spr_demo.c
  # the v_blank of gfsdemo is a pre-release (0.90) that calls the removed SMPA_* pad API: use the 1.00 one of backram
  ../backram/gnu/v_blank/v_blank.c
  ../backram/gnu/v_blank/set_vb.c
  )

# sine table and fixed-point helpers
set(ASM_SOURCES test.s)

set_property(SOURCE ${ASM_SOURCES} PROPERTY LANGUAGE ASM)

set(CD_FILES
      0.bin
      tut160.bin
    )

set(TARGET_NAME gfsdemo)
set(TARGET_PATH gfsdemo)

add_compile_definitions(_SH MODEL_S)

add_executable( ${TARGET_NAME}.elf ${SOURCES} ${ASM_SOURCES})

target_include_directories(${TARGET_NAME}.elf PUBLIC
     .
     ${SEGALIB}/include
     ${CMAKE_CURRENT_SOURCE_DIR}/../backram/gnu
)

# 1995 code: implicit int and implicit function declarations are errors with GCC 14 unless gnu89
set(c_flags -Wall -m2 -MMD -std=gnu89)

target_compile_options(${TARGET_NAME}.elf PUBLIC $<$<COMPILE_LANGUAGE:C>:${c_flags}>)

target_link_options(${TARGET_NAME}.elf PUBLIC
    "SHELL:-Xlinker -Map -Xlinker ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.map"
    "SHELL:-Xlinker --strip-debug"
    "SHELL:-Xlinker -fno-lto"
    "SHELL:-m2 -nostartfiles"
    "SHELL:-T${SATURN_COMMON}/Saturn.lnk"
    "SHELL:-lc -lgcc -lm"
)

target_link_libraries( ${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_scl.a )
target_link_libraries( ${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_spr.a )
target_link_libraries( ${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_mth.a )
target_link_libraries( ${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_dbg.a )
target_link_libraries( ${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_gfs.a )
target_link_libraries( ${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_dma.a )
target_link_libraries( ${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_csh.a )
target_link_libraries( ${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_cdc.a )
target_link_libraries( ${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_per.a )
target_link_libraries( ${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_int.a )

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

# the 160x160 picture read from the CD
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/tut160.bin
        DESTINATION ${INSTALL_DESTINATION}/${TARGET_PATH}/CD )

include(${SATURN_CMAKE}/install.cmake)
include(${SATURN_CMAKE}/create_cd.cmake)
