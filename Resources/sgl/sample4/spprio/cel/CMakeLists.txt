set(SOURCES
  ${SEGASGL}/samples/sample2/common/cinit.c
  ${SEGASGL}/share/sddrvs.c
  main.c
  )

set(CD_FILES
      0.bin
    )

set(TARGET_NAME spprio_cel)
set(TARGET_PATH spprio_cel)

add_compile_definitions(MODEL_S _SH)

add_executable( ${TARGET_NAME}.elf ${SOURCES} )

target_include_directories(${TARGET_NAME}.elf PUBLIC .
    ${SEGASGL}/include
    ${SEGALIB}/include
    ${PROJECT_SOURCE_DIR} )

target_compile_options(${TARGET_NAME}.elf PUBLIC $<$<COMPILE_LANGUAGE:C>:${c_flags}>
                                                 $<$<COMPILE_LANGUAGE:ASM>:${as_flags}> )

target_link_options(${TARGET_NAME}.elf PUBLIC
    "SHELL:-Xlinker -Map -Xlinker ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.map"
    "SHELL:-Xlinker --strip-debug"
    "SHELL:-Xlinker -fno-lto"
    "SHELL:-m2 -nostartfiles"
    "SHELL:-T${SEGASGL}/samples/sample1/common/sl.lnk"
)

target_link_libraries( ${TARGET_NAME}.elf PUBLIC ${SEGASGL}/lib/sglarea.o )
target_link_libraries( ${TARGET_NAME}.elf PUBLIC ${SEGASGL}/lib/libsgl.a )
target_link_libraries( ${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_mem.a )
target_link_libraries( ${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_gfs.a )
target_link_libraries( ${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_dma.a )
target_link_libraries( ${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_csh_sgl.a )
target_link_libraries( ${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_sys.a )
target_link_libraries( ${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_cdc.a )
target_link_libraries( ${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_int.a )
target_link_libraries( ${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_stm.a )
target_link_libraries( ${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_pcm.a )
target_link_libraries( ${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_snd.a )

add_custom_target(run_${TARGET_NAME} ALL DEPENDS ${TARGET_NAME}.elf ${TARGET_NAME}.bin IP.BIN)

add_custom_command(OUTPUT ${TARGET_NAME}.bin
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMAND ${CMAKE_OBJCOPY}
                   ARGS -O binary ${TARGET_NAME}.elf ${TARGET_NAME}.bin )

add_custom_command(OUTPUT IP.BIN
                  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                  COMMAND $ENV{SATURN_IPMAKER}/IPMaker
                  ARGS ${ip_maker_verbose} -o ${CMAKE_CURRENT_BINARY_DIR}/IP.BIN
                        -t ${TARGET_NAME} -p 2 )

include(${SATURN_CMAKE}/install.cmake)
include(${SATURN_CMAKE}/create_cd.cmake)
