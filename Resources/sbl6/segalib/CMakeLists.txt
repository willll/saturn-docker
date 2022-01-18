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

install(DIRECTORY include DESTINATION segalib)
