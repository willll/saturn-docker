set(SOURCES
  ${SEGASGL}/demos/biplane/main.c
  ${SEGASGL}/demos/biplane/scroll.c
  ${SEGASGL}/demos/biplane/polygon.c
  ${SEGASGL}/demos/biplane/texture.c
  ${SEGASGL}/demos/biplane/pol.c
  )

set(CD_FILES
      0.bin
    )

set(TARGET_NAME biplane)
set(TARGET_PATH biplane)

add_compile_definitions(MODEL_S _SH)

add_executable( ${TARGET_NAME}.elf ${SOURCES} )

target_include_directories(${TARGET_NAME}.elf PUBLIC
    ${SEGASGL}/demos/biplane/
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
    "SHELL:-T${SEGASGL}/common/sl.lnk"
)

target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGASGL}/lib/libsgl.a )

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
