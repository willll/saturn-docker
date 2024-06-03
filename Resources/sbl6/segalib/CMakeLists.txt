cmake_minimum_required(VERSION 3.5)

SET(CMAKE_SYSTEM_NAME Generic)

set(SEGALIB_DESTINATION_LIB segalib/lib)
set(SEGALIB_DESTINATION_FLD segalib/fld)

project(segalib
        VERSION 6.1
        DESCRIPTION "Sega Saturn SBL"
        LANGUAGES C ASM)

set(c_flags -Wall
            -Wpointer-sign
            -m2
            -fomit-frame-pointer
            )

cmake_path(NORMAL_PATH CMAKE_SOURCE_DIR OUTPUT_VARIABLE PROJECT_ROOT_NORMALIZED)

add_compile_options(
  "$<$<COMPILE_LANGUAGE:C>:${c_flags}>"
  "$<$<COMPILE_LANGUAGE:C>:-ffile-prefix-map=${PROJECT_ROOT_NORMALIZED}=./src/${SEGALIB_DESTINATION_LIB}>"
)

add_subdirectory(bpl)
add_subdirectory(cdc)
add_subdirectory(clib)
add_subdirectory(cmp)
add_subdirectory(csh)
add_subdirectory(csh_sgl)
add_subdirectory(dbg)
add_subdirectory(dma)
add_subdirectory(dsp)
add_subdirectory(fld)
add_subdirectory(gfs)
add_subdirectory(int)
add_subdirectory(mem)
add_subdirectory(mpg)
add_subdirectory(mth)
add_subdirectory(pcm)
add_subdirectory(per)
add_subdirectory(scl)
add_subdirectory(snd)
add_subdirectory(spr)
add_subdirectory(stm)

install(DIRECTORY include DESTINATION segalib)
