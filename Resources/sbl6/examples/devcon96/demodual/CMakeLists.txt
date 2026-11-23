set(SOURCES
  ${SATURN_COMMON}/crt0.s
  main.c
  )

set(ASM_SOURCES rbg0.s rbg1.s)

set_property(SOURCE ${ASM_SOURCES} PROPERTY LANGUAGE ASM)

set(CD_FILES
      0.bin
    )

set(TARGET_NAME demodual)
set(TARGET_PATH demodual)

add_compile_definitions(SATURN BYPASS_SCU_DMA MODEL_S _SH)

add_executable( ${TARGET_NAME}.elf ${SOURCES} ${ASM_SOURCES})

add_dependencies( ${TARGET_NAME}.elf lib3d libutil libmath)

target_include_directories(${TARGET_NAME}.elf PUBLIC
     .
     ${SEGALIB}/include
)

# 1996 code: implicit int and implicit function declarations are errors with GCC 14 unless gnu89
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

target_link_libraries( ${TARGET_NAME}.elf PUBLIC lib3d )
target_link_libraries( ${TARGET_NAME}.elf PUBLIC libutil )
target_link_libraries( ${TARGET_NAME}.elf PUBLIC libmath )

target_link_libraries( ${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_per.a )
target_link_libraries( ${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_dsp.a )
target_link_libraries( ${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_dma.a )
target_link_libraries( ${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_csh.a )
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

include(${SATURN_CMAKE}/install.cmake)
include(${SATURN_CMAKE}/create_cd.cmake)
