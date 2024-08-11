cmake_minimum_required(VERSION 3.15)

include(FetchContent)

set(SOURCES
  ${SATURN_COMMON}/crt0.s
  smpfld01.c
  ${PROJECT_SOURCE_DIR}/v_blank/v_blank.c
  ${PROJECT_SOURCE_DIR}/v_blank/set_vb.c
  )

set(CD_FILES
      0.bin
      fld_knl.bin
      TEST.BIN
      TEST/0.BIN=TEST/0.BIN
      TEST/IP.BIN=TEST/IP.BIN
    )

set(TARGET_NAME smpfld01)
set(TARGET_PATH fld/smpfld01)

add_compile_definitions(MODEL_S _SH)

add_executable( ${TARGET_NAME}.elf ${SOURCES} )

target_include_directories(
    ${TARGET_NAME}.elf PUBLIC
    .
    ${SEGALIB}/include
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/per/smpclib
    )

target_compile_options(${TARGET_NAME}.elf PUBLIC $<$<COMPILE_LANGUAGE:C>:${c_flags}>
                                                 $<$<COMPILE_LANGUAGE:ASM>:${as_flags}> )

target_link_options(${TARGET_NAME}.elf PUBLIC
    "SHELL:-Xlinker -Map -Xlinker ${TARGET_NAME}.map"
    "SHELL:-Xlinker --strip-debug"
    "SHELL:-m2 -nostartfiles"
    "SHELL:-T${SATURN_COMMON}/Saturn.lnk"
    "SHELL: --specs=nosys.specs"
    #"SHELL:-lc -lgcc"
)

target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/fld_load.a )
target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_scl.a )
target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_mth.a )
target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_spr.a )
target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_gfs.a )
target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_cdc.a )
target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_per.a )
target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_int.a )
target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_csh.a )
target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_dma.a )

add_custom_target(run_${TARGET_NAME} ALL DEPENDS ${TARGET_NAME}.elf ${TARGET_NAME}.bin IP.BIN)

FetchContent_Declare(
                saturn_helloworld
                GIT_REPOSITORY https://github.com/willll/saturn_helloworld.git
                GIT_SHALLOW TRUE
                GIT_TAG tags/1,0
                )
FetchContent_GetProperties(saturn_helloworld)

FetchContent_MakeAvailable(saturn_helloworld)

add_custom_command(OUTPUT ${TARGET_NAME}.bin
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMAND ${CMAKE_OBJCOPY}
                   ARGS -O binary ${TARGET_NAME}.elf ${TARGET_NAME}.bin )

add_custom_command(OUTPUT IP.BIN
                  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                  COMMAND $ENV{SATURN_IPMAKER}/IPMaker
                  ARGS ${ip_maker_verbose} -o ${CMAKE_CURRENT_BINARY_DIR}/IP.BIN
                        -t ${TARGET_NAME} -p 2 )

install(FILES ${saturn_helloworld_BINARY_DIR}/IP.BIN
              DESTINATION ${INSTALL_DESTINATION}/${TARGET_PATH}/CD RENAME TEST.BIN )

install(FILES ${saturn_helloworld_BINARY_DIR}/IP.BIN
              DESTINATION ${INSTALL_DESTINATION}/${TARGET_PATH}/CD/TEST RENAME IP.BIN )

install(FILES ${saturn_helloworld_BINARY_DIR}/helloworld.bin
              DESTINATION ${INSTALL_DESTINATION}/${TARGET_PATH}/CD/TEST RENAME 0.BIN )

install(FILES ${SEGALIB}/fld/fld_knl.bin
              DESTINATION ${INSTALL_DESTINATION}/${TARGET_PATH}/CD/ )

include(${SATURN_CMAKE}/install.cmake)
include(${SATURN_CMAKE}/create_cd.cmake)
