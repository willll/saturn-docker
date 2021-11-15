cmake_minimum_required(VERSION 3.0)

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

project( segalib )

add_compile_options(-Wall -Wpointer-sign -m2 -O2)

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

#install(TARGETS include DESTINATION include)
