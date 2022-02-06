set(SOURCES
  ${SATURN_COMMON}/crt0.s
  smpbup1.c font/fnt_cod0.c font/fnt_cod1.c font/fnt_dat0.c
  font/fnt_dat1.c font/fnt_dat2.c
  ${PROJECT_SOURCE_DIR}/v_blank/v_blank.c
  ${PROJECT_SOURCE_DIR}/v_blank/set_vb.c
  )

set(TARGET_NAME bup)
set(TARGET_PATH bup)

add_compile_definitions(MODEL_S _SH)

add_executable( ${TARGET_NAME}.elf ${SOURCES} )

target_include_directories(${TARGET_NAME}.elf PUBLIC . ${SEGALIB}/include
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/per/smpclib)

target_compile_options(${TARGET_NAME}.elf PUBLIC -Wall -m2 -MMD)

target_link_options(${TARGET_NAME}.elf PUBLIC
    "SHELL:-Xlinker -Map -Xlinker ${TARGET_NAME}.map"
    "SHELL:-Xlinker --strip-debug"
    "SHELL:-Xlinker -fno-lto"
    "SHELL:-m2 -nostartfiles"
    "SHELL:-T${SATURN_COMMON}//Saturn.lnk"
    "SHELL:-lc -lgcc -lm"
)

target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_scl.a )
target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_mth.a )
target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_spr.a )
target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_cdc.a )
target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_per.a )
target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_int.a )

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

install(TARGETS ${TARGET_NAME}.elf DESTINATION ${SEGASMP_DESTINATION}/${TARGET_PATH} )

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.bin
              ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.map
        DESTINATION ${SEGASMP_DESTINATION}/${TARGET_PATH} )

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.bin
          DESTINATION ${SEGASMP_DESTINATION}/${TARGET_PATH}/CD
          RENAME 0.bin )

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/IP.BIN
              DESTINATION ${SEGASMP_DESTINATION}/${TARGET_PATH}/CD )
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_NAME}.cue DESTINATION ${SEGASMP_DESTINATION}/${TARGET_PATH} )
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/cd_included_files.txt DESTINATION ${SEGASMP_DESTINATION}/${TARGET_PATH}/CD
          RENAME FILES.txt )
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/cd_excludes_files.txt DESTINATION ${SEGASMP_DESTINATION}/${TARGET_PATH}/CD
          RENAME EXCLUDES.txt )

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION ${SEGASMP_DESTINATION}/${TARGET_PATH}/Sources)

set(MAKE_CMD $ENV{SATURN_CD}/create_cd.sh
    "${TARGET_NAME}" "\"SBL\"" "\"SBL\"" IP.BIN
    "${TARGET_NAME}.iso" "${CMAKE_INSTALL_PREFIX}/${SEGASMP_DESTINATION}/${TARGET_PATH}/")

install(CODE "execute_process(COMMAND
                                ${MAKE_CMD}
                                WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/${SEGASMP_DESTINATION}/${TARGET_PATH}/CD
                              )"
        )
