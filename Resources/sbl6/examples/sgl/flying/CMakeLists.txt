set(SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/cinit.c
  main.c
  list.c
  mem_mgr.c
  vector.c
  tree.c
  ailand.c
  actor.c
  ai.c
  aictrl.c
  satrend.c
  demo.c
  aisaucer.c
  saturn.c
  texture.c
  collide.c
  aitarget.c
  )

set(CD_FILES
      0.bin
    )

set(TARGET_NAME flying)
set(TARGET_PATH sgl/flying)

add_compile_definitions(MODEL_S _SH)

add_executable( ${TARGET_NAME}.elf ${SOURCES} )

target_include_directories(${TARGET_NAME}.elf PUBLIC .
    ${SEGASGL}/include
    ${SEGALIB}/include )

# 1996 code: implicit int and implicit function declarations are errors with GCC 14 unless gnu89
set(c_flags -Wall -m2 -MMD -std=gnu89)

target_compile_options(${TARGET_NAME}.elf PUBLIC $<$<COMPILE_LANGUAGE:C>:${c_flags}>)

target_link_options(${TARGET_NAME}.elf PUBLIC
    "SHELL:-Xlinker -Map -Xlinker ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.map"
    "SHELL:-Xlinker --strip-debug"
    "SHELL:-Xlinker -fno-lto"
    "SHELL:-m2 -nostartfiles"
    "SHELL:-T${CMAKE_CURRENT_SOURCE_DIR}/sl.lnk"
    "SHELL:-lc -lgcc -lm"
)

target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGASGL}/lib/sglarea.o )
target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGASGL}/lib/libsgl.a )
target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_per.a )
# sqrt(): libm after the objects; newlib then needs the nosys stubs (_sbrk, _write...)
target_link_libraries(${TARGET_NAME}.elf PUBLIC m )
target_link_options(${TARGET_NAME}.elf PUBLIC "SHELL:--specs=nosys.specs")

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
