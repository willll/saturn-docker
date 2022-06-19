set(SOURCES
  Source/Source/Main.c
  ${SATURN_SAMPLE_COMMON}/Source/Source/VDP.c
  ${SATURN_SAMPLE_COMMON}/Source/Source/Debug.c
  ${SATURN_SAMPLE_COMMON}/Source/Source/Peripheral.c
  ${SATURN_SAMPLE_COMMON}/Source/Source/USBCart.c
  ${SATURN_SAMPLE_COMMON}/Source/Source/CDC.c
  ${SATURN_SAMPLE_COMMON}/Source/Source/SMPC.c
  ${SATURN_SAMPLE_COMMON}/Source/Source/crt0.s
  )

add_compile_definitions(MODEL_S)

add_executable(TexturedSprite.elf ${SOURCES})

target_include_directories(TexturedSprite.elf PUBLIC
              ${SATURN_SAMPLE_COMMON}/Source/Headers/)

target_link_options(TexturedSprite.elf PUBLIC
    "SHELL:-Xlinker -Map -Xlinker ${OUTPUT_NAME}.map"
    "SHELL:-Xlinker --strip-debug"
    "SHELL:-m2 -nostartfiles"
    "SHELL:-T${CMAKE_CURRENT_SOURCE_DIR}/Source/Saturn.lnk"
)

add_custom_target(run_TexturedSprite ALL DEPENDS TexturedSprite.elf TexturedSprite.bin)

add_custom_command(OUTPUT TexturedSprite.bin
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMAND ${CMAKE_OBJCOPY}
                   ARGS -O binary TexturedSprite.elf TexturedSprite.bin )

install(TARGETS TexturedSprite.elf DESTINATION ${SAMPLES_DESTINATION}/TexturedSprite )
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/TexturedSprite.bin DESTINATION ${SAMPLES_DESTINATION}/TexturedSprite )
install(DIRECTORY Source DESTINATION ${SAMPLES_DESTINATION}/TexturedSprite )
