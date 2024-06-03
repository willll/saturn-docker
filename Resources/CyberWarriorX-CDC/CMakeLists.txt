cmake_minimum_required(VERSION 3.5)

SET(CMAKE_SYSTEM_NAME Generic)

set(INSTALL_DESTINATION lib)

set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_EXE_LINKER_FLAGS "")
set(CMAKE_STATIC_LIBRARY_SUFFIX ".a")
set(CMAKE_STATIC_LIBRARY_PREFIX "")

set(SEGALIB $ENV{SATURN_SBL}/segalib)

set(SATURN_COMMON $ENV{SATURN_COMMON})
set(SATURN_CMAKE $ENV{SATURN_CMAKE})

project( cdcrepv100rc1 VERSION 1.0
                  DESCRIPTION "Reverse-engineering of the Sega's CDC library"
                  LANGUAGES C)

set(c_flags -Wall
            -Wpointer-sign
            -m2
            -Os
            -finline-functions
            -fomit-frame-pointer)

add_compile_options(  "$<$<COMPILE_LANGUAGE:C>:${c_flags}>"
)

set(SOURCES
    src/cdc_bif.c
    src/cdc_bio.c
    src/cdc_cfs.c
    src/cdc_cmn.c
    src/cdc_dev.c
    src/cdc_drv.c
    src/cdc_mbu.c
    src/cdc_mdc.c
    src/cdc_mfb.c
    src/cdc_mls.c
    src/cdc_msb.c
    src/cdc_mst.c
    src/cdc_mwn.c
    src/cdc_reg.c
    src/cdc_scd.c
    src/cdc_sel.c
    src/cdc_sub.c
    src/cdc_unk.c
  )

add_compile_definitions(_SH MODEL_S)

add_library( sega_cdc STATIC
             ${SOURCES} )

include_directories(${PROJECT_SOURCE_DIR}/include
                    ${SEGALIB}/include)

SET_TARGET_PROPERTIES(sega_cdc PROPERTIES PREFIX "")

install(TARGETS sega_cdc DESTINATION ${INSTALL_DESTINATION})
