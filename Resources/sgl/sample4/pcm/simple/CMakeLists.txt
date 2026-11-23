cmake_minimum_required(VERSION 3.5)

SET(CMAKE_SYSTEM_NAME Generic)

add_subdirectory(all)
add_subdirectory(gfs)
add_subdirectory(mem)
add_subdirectory(stm)
# xa plays SAMPLE.XA (ADPCM made with the SEGA ADPCM Encoder, on a MODE2 track: see xa/smpxa.scr): not in the archive,
# the ISO built here has no XA track, the program builds but has nothing to play
add_subdirectory(xa)
