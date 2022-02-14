set(SOURCES profile.c )

set(ASM_SOURCES timer.s vect.s)

set(TARGET_NAME libprofl)

set(as_flags -I ${CMAKE_CURRENT_SOURCE_DIR}/)

add_compile_options(  "$<$<COMPILE_LANGUAGE:ASM>:${as_flags}>"
)

set_property(SOURCE ${ASM_SOURCES} PROPERTY LANGUAGE ASM)

add_compile_definitions(_SH MODEL_S)

add_library( ${TARGET_NAME} STATIC
             ${SOURCES}
             ${ASM_SOURCES}
             )

target_include_directories(${TARGET_NAME} PUBLIC
     .
     ${SEGALIB}/include
)

set_target_properties(${TARGET_NAME} PROPERTIES PREFIX "")

install(TARGETS ${TARGET_NAME} DESTINATION ${SEGASMP_DESTINATION})
