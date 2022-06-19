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

add_executable(CDBrowser.elf ${SOURCES})

target_include_directories(CDBrowser.elf PUBLIC
              ${SATURN_SAMPLE_COMMON}/Source/Headers/)

target_link_options(CDBrowser.elf PUBLIC
    "SHELL:-Xlinker -Map -Xlinker ${OUTPUT_NAME}.map"
    "SHELL:-Xlinker --strip-debug"
    "SHELL:-m2 -nostartfiles"
    "SHELL:-T${CMAKE_CURRENT_SOURCE_DIR}/Source/Saturn.lnk"
)

add_custom_target(run_CDBrowser ALL DEPENDS CDBrowser.elf CDBrowser.bin)

add_custom_command(OUTPUT CDBrowser.bin
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMAND ${CMAKE_OBJCOPY}
                   ARGS -O binary CDBrowser.elf CDBrowser.bin )

install(TARGETS CDBrowser.elf DESTINATION ${SAMPLES_DESTINATION}/CDBrowser )
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/CDBrowser.bin DESTINATION ${SAMPLES_DESTINATION}/CDBrowser )
install(DIRECTORY Source DESTINATION ${SAMPLES_DESTINATION}/CDBrowser )
