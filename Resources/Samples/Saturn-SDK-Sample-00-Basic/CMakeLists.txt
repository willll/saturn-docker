set(SOURCES
  Source/Source/Main.c
  Source/Source/crt0.s
  )

add_compile_definitions(MODEL_S)

add_executable(Basic ${SOURCES})

target_compile_options(Basic PUBLIC -g -O0 -Wall -m2 )

target_link_options(Basic PUBLIC
    "SHELL:-Xlinker -Map -Xlinker ${TARGET_NAME}.map"
    "SHELL:-nostartfiles"
    "SHELL:-T${CMAKE_CURRENT_SOURCE_DIR}/Source/Saturn.lnk"
)

install(TARGETS Basic DESTINATION ${SAMPLES_DESTINATION}/${TARGET_NAME} )
install(DIRECTORY Source DESTINATION ${SAMPLES_DESTINATION}/${TARGET_NAME})
