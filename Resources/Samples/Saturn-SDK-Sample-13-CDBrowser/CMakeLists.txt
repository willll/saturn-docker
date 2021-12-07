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

add_executable(CDBrowser ${SOURCES})

target_include_directories(CDBrowser PUBLIC
              ${SATURN_SAMPLE_COMMON}/Source/Headers/)

target_compile_options(CDBrowser PUBLIC -g -O0 -Wall -m2 )

target_link_options(TexturedSprite PUBLIC
    "SHELL:-Xlinker -Map -Xlinker ${TARGET_NAME}.map"
    "SHELL:-nostartfiles"
    "SHELL:-T${SATURN_SAMPLE_COMMON}/Source/Saturn.lnk"
)

target_link_libraries(CDBrowser PUBLIC /Source/Source/crt0s.a )

install(TARGETS CDBrowser DESTINATION ${SAMPLES_DESTINATION}/${TARGET_NAME} )
install(DIRECTORY Source DESTINATION ${SAMPLES_DESTINATION}/${TARGET_NAME})
