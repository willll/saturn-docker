set(SOURCES
  ${SATURN_COMMON}/crt0.s
  source/smpcdem.c
  # libsmpc: the SMPC peripheral drivers and the monitor (source/lib/makefile)
  source/lib/ppm_.c
  source/lib/ppmspr.c
  source/lib/device.c
  source/lib/ppmrbg.c
  source/lib/pad.c
  source/lib/mainmenu.c
  source/lib/font8.c
  source/lib/init.c
  source/lib/menu.c
  source/lib/p_icon.c
  source/lib/smpcdisp.c
  source/lib/spaint.c
  source/lib/spcmd.c
  source/lib/spdat03.c
  source/lib/spdemo.c
  source/lib/command.c
  )

set(CD_FILES
      0.bin
    )

set(TARGET_NAME periphs)
set(TARGET_PATH periphs)

add_compile_definitions(SATURN BYPASS_SCU_DMA MODEL_S _SH)

add_executable( ${TARGET_NAME}.elf ${SOURCES})

# the SBL headers of the image first: periphs/include also has an older copy of them, only its smpc*.h and ssp*.h are used
target_include_directories(${TARGET_NAME}.elf PUBLIC
     ${SEGALIB}/include
     ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 1995 code: implicit int and implicit function declarations are errors with GCC 14 unless gnu89
set(c_flags -Wall -m2 -MMD -std=gnu89 -finline-functions -fomit-frame-pointer)

target_compile_options(${TARGET_NAME}.elf PUBLIC $<$<COMPILE_LANGUAGE:C>:${c_flags}>)

target_link_options(${TARGET_NAME}.elf PUBLIC
    "SHELL:-Xlinker -Map -Xlinker ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.map"
    "SHELL:-Xlinker --strip-debug"
    "SHELL:-Xlinker -fno-lto"
    "SHELL:-m2 -nostartfiles"
    "SHELL:-T${SATURN_COMMON}/Saturn.lnk"
    "SHELL:-lc -lgcc -lm"
)

# libsat.a of the archive is an early SBL build (unreadable object format): the SBL libraries of the image instead
target_link_libraries( ${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_scl.a )
target_link_libraries( ${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_spr.a )
target_link_libraries( ${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_mth.a )
target_link_libraries( ${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_dbg.a )
target_link_libraries( ${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_dma.a )
target_link_libraries( ${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_csh.a )
target_link_libraries( ${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_int.a )

# sin()/cos(): libm after the objects; newlib then needs the nosys stubs (_sbrk, _write...)
target_link_libraries( ${TARGET_NAME}.elf PUBLIC m )
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
