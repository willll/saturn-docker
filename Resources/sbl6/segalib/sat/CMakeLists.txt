cmake_minimum_required(VERSION 3.5)

SET(CMAKE_SYSTEM_NAME Generic)

set(SEGALIB $ENV{SATURN_SBL}/segalib)
set(SEGALIB_DESTINATION_LIB segalib/lib)
set(SEGASAT ${CMAKE_CURRENT_BINARY_DIR}/sega_sat.a)

set( LIBS
            ${SEGALIB}/lib/sega_cdc.a
            ${SEGALIB}/lib/sega_cmp.a
            ${SEGALIB}/lib/sega_csh.a
            ${SEGALIB}/lib/sega_dbg.a
            ${SEGALIB}/lib/sega_dma.a
            ${SEGALIB}/lib/sega_dsp.a
            ${SEGALIB}/lib/sega_gfs.a
            ${SEGALIB}/lib/sega_mem.a
            ${SEGALIB}/lib/sega_mth.a
            ${SEGALIB}/lib/sega_scl.a
            ${SEGALIB}/lib/sega_spr.a
            ${SEGALIB}/lib/sega_snd.a
            ${SEGALIB}/lib/sega_stm.a
            )

project(segalib_sat
        VERSION 6.1
        DESCRIPTION "Sega Saturn SBL SAT"
        LANGUAGES C)

add_custom_command(OUTPUT ${SEGASAT}
                   COMMAND $ENV{AR} ARGS -rcT ${SEGASAT} ${LIBS})

add_custom_target(sat ALL DEPENDS ${SEGASAT})

install(PROGRAMS ${SEGASAT} DESTINATION ${SEGALIB_DESTINATION_LIB})
