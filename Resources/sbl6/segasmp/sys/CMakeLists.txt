cmake_minimum_required(VERSION 3.12)

set(SOURCES
  ${SATURN_COMMON}/crt0.s
  sys_id.src
  smpsys.c
  )

set(CD_FILES
      0.bin
    )

set(TARGET_NAME sys)
set(TARGET_PATH sys)

add_compile_definitions(MODEL_S _SH)

add_executable( ${TARGET_NAME}.elf ${SOURCES} )

target_include_directories(${TARGET_NAME}.elf PUBLIC . ${SEGALIB}/include )

target_compile_options(${TARGET_NAME}.elf PUBLIC -Wall -m2 -MMD)

target_link_options(${TARGET_NAME}.elf PUBLIC
    "SHELL:-Xlinker -Map -Xlinker ${TARGET_NAME}.map"
    "SHELL:-Xlinker --strip-debug"
    "SHELL:-m2 -nostartfiles"
    "SHELL:-T${SATURN_COMMON}/Saturn.lnk"
    "SHELL:-lc -lgcc -lm"
)

target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_sat.a )

add_custom_target(run_${TARGET_NAME} ALL DEPENDS ${TARGET_NAME}.elf ${TARGET_NAME}.bin)

add_custom_command(OUTPUT ${TARGET_NAME}.bin
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMAND ${CMAKE_OBJCOPY}
                   ARGS -O binary ${TARGET_NAME}.elf ${TARGET_NAME}.bin )

include(${SATURN_CMAKE}/install.cmake)
