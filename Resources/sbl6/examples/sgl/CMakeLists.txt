cmake_minimum_required(VERSION 3.5)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/chrome)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/flying)
# biplane is not built: its art (scroll/*.cel, map/*.mdl, map/tex/*.txr) is not in the archive
# cdda_sgl is not built: it links src/sdata.o (the sound driver), whose source is not in the archive
