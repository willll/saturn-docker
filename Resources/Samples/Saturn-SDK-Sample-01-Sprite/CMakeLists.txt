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

add_executable(Sprite.elf ${SOURCES})

target_compile_options(Sprite.elf  PUBLIC -g -O0 -Wall -m2 -MMD)

target_include_directories(Sprite.elf PUBLIC
              ${SATURN_SAMPLE_COMMON}/Source/Headers/)

target_link_options(Sprite.elf PUBLIC
    "SHELL:-Xlinker -Map -Xlinker ${OUTPUT_NAME}.map"
    "SHELL:-Xlinker --strip-debug"
    "SHELL:-m2 -nostartfiles"
    "SHELL:-T${CMAKE_CURRENT_SOURCE_DIR}/Source/Saturn.lnk"
)

add_custom_target(run_Sprite ALL DEPENDS Sprite.elf Sprite.bin)

add_custom_command(OUTPUT Sprite.bin
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMAND ${CMAKE_OBJCOPY}
                   ARGS -O binary Sprite.elf Sprite.bin )

install(TARGETS Sprite.elf DESTINATION ${SAMPLES_DESTINATION}/Sprite )
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/Sprite.bin DESTINATION ${SAMPLES_DESTINATION}/Sprite )
install(DIRECTORY Source DESTINATION ${SAMPLES_DESTINATION}/Sprite )
