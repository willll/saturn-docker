cmake_minimum_required(VERSION 3.0)

set(SOURCES
  Source/Source/Main.c
  Source/Source/crt0.s
  )

add_compile_definitions(MODEL_S)

add_executable(Basic.elf ${SOURCES})

target_compile_options(Basic.elf  PUBLIC -g -O0 -Wall -m2 -MMD)

target_link_options(Basic.elf PUBLIC
    "SHELL:-Xlinker -Map -Xlinker ${OUTPUT_NAME}.map"
    "SHELL:-Xlinker --strip-debug"
    "SHELL:-m2 -nostartfiles"
    "SHELL:-T${CMAKE_CURRENT_SOURCE_DIR}/Source/Saturn.lnk"
)

add_custom_target(run ALL DEPENDS Basic.elf Basic.bin)

add_custom_command(OUTPUT Basic.bin
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMAND ${CMAKE_OBJCOPY}
                   ARGS -O binary Basic.elf Basic.bin )

install(TARGETS Basic.elf DESTINATION ${SAMPLES_DESTINATION}/Basic )
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/Basic.bin DESTINATION ${SAMPLES_DESTINATION}/Basic )
install(DIRECTORY Source DESTINATION ${SAMPLES_DESTINATION}/Basic )
