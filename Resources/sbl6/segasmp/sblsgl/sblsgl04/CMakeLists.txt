cmake_minimum_required(VERSION 3.12)

set(SOURCES
  ${SATURN_COMMON}/crt0.s
  ${PROJECT_SOURCE_DIR}/v_blank/v_blank.c
  ${PROJECT_SOURCE_DIR}/v_blank/set_vb.c
  sgl_func.c
  scl_data.c
  sblsgl04.c
  )

set(CD_FILES
      0.bin
    )

set(TARGET_NAME sblsgl04)
set(TARGET_PATH sblsgl/sblsgl04)

add_compile_definitions(MODEL_S _SH)

add_executable( ${TARGET_NAME}.elf ${SOURCES} )

target_include_directories(${TARGET_NAME}.elf PUBLIC .
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${SEGALIB}/include
    ${SEGASGL}/include
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/per/smpclib)

target_link_options(${TARGET_NAME}.elf PUBLIC
    "SHELL:-Xlinker -Map -Xlinker ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.map"
    "SHELL:-Xlinker --strip-debug"
    "SHELL:-m2 -nostartfiles"
    "SHELL:-T${SATURN_COMMON}/Saturn.lnk"
)

target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGASGL}/lib/sglarea.o )
target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGASGL}/lib/libsgl.a )
target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_per.a )
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
