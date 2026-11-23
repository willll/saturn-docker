cmake_minimum_required(VERSION 3.5)

SET(CMAKE_SYSTEM_NAME Generic)

add_subdirectory(mapchg)
# sndif is not built: share/sgl_snd.c needs the SBL sound interface library 1.30+ (SND_Init_Sound) and libsgl_n
# (SGL without its sound functions); SBL 6 has the 1.04 library and the archives have no libsgl_n
#add_subdirectory(sndif)
add_subdirectory(sndtest)
