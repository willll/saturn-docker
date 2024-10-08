cmake_minimum_required(VERSION 3.12)

set(SOURCES
  ${SATURN_COMMON}/crt0.s
  ${PROJECT_SOURCE_DIR}/per/smpclib/per_x1.c
  ${PROJECT_SOURCE_DIR}/per/smpclib/per_x2.c
  ${PROJECT_SOURCE_DIR}/per/smpclib/per_x3.src
  title.c
  demo.c
  logo.c
  #v_blank.c
  polygon.c
  game_sub.c
  act.c
  game.c
  data.c
  sequence.c
  ${PROJECT_SOURCE_DIR}/v_blank/v_blank.c
  ${PROJECT_SOURCE_DIR}/v_blank/set_vb.c
  )

set(CD_FILES
      0.bin
    )

set(TARGET_NAME game)
set(TARGET_PATH game)

add_compile_definitions(MODEL_S _SH)

add_executable( ${TARGET_NAME}.elf ${SOURCES} )

target_include_directories(${TARGET_NAME}.elf PUBLIC
    .
    ${SEGALIB}/include
    ${SEGASGL}/include
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/per/smpclib)

target_compile_options(${TARGET_NAME}.elf PUBLIC $<$<COMPILE_LANGUAGE:C>:${c_flags}>
                                                 $<$<COMPILE_LANGUAGE:ASM>:${as_flags}> )

target_link_options(${TARGET_NAME}.elf PUBLIC
    "SHELL:-Xlinker -Map -Xlinker ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.map"
    "SHELL:-Xlinker --strip-debug"
    "SHELL:-m2 -nostartfiles"
    "SHELL:-T${SATURN_COMMON}/Saturn.lnk"
    "SHELL: --specs=nosys.specs"
)

target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_per.a )
target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGASGL}/lib/sglarea.o )
target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGASGL}/lib/libsgl.a )
target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_int.a )
target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_sat.a )

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
