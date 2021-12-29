cmake_minimum_required(VERSION 3.0)

SET(CMAKE_SYSTEM_NAME Generic)

set(SEGALIB_DESTINATION_LIB segalib/lib)

project(segalib
        VERSION 6.1
        DESCRIPTION "Sega Saturn SBL"
        LANGUAGES C ASM)

add_compile_options(  -Wall
                      -Wpointer-sign
                      -m2
                      -Os
                      -finline-functions
                      -fomit-frame-pointer
)

add_subdirectory(bpl)
add_subdirectory(cmp)
add_subdirectory(csh)
add_subdirectory(dbg)
add_subdirectory(dma)
add_subdirectory(dsp)
add_subdirectory(gfs)
add_subdirectory(int)
add_subdirectory(mem)
add_subdirectory(mpg)
add_subdirectory(mth)
add_subdirectory(scl)
add_subdirectory(spr)
add_subdirectory(cdc)
add_subdirectory(pcm)
add_subdirectory(per)
add_subdirectory(stm)
add_subdirectory(snd)

# TODO : Should be generated once the others are done

#add_library( sat INTERFACE )

#SET_TARGET_PROPERTIES(sat PROPERTIES PREFIX "")

#target_link_libraries(sat INTERFACE
#            ${SEGALIB}/lib/sega_cdc.a
#            ${SEGALIB}/lib/sega_cmp.a
#            ${SEGALIB}/lib/sega_csh.a
#            ${SEGALIB}/lib/sega_dbg.a
#            ${SEGALIB}/lib/sega_dma.a
#            ${SEGALIB}/lib/sega_dsp.a
#            ${SEGALIB}/lib/sega_gfs.a
#            ${SEGALIB}/lib/sega_mem.a
#            ${SEGALIB}/lib/sega_mth.a
#            ${SEGALIB}/lib/sega_per.a
#            ${SEGALIB}/lib/sega_spr.a
#            ${SEGALIB}/lib/sega_scl.a
#            ${SEGALIB}/lib/sega_snd.a
#            ${SEGALIB}/lib/sega_stm.a
#            )

#install(TARGETS sat DESTINATION ${SEGALIB_DESTINATION_LIB})

install(DIRECTORY include DESTINATION segalib)
