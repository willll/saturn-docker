cmake_minimum_required(VERSION 3.0)

SET(CMAKE_SYSTEM_NAME Generic)

set(SEGASMP_DESTINATION segasmp)

set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
set(CMAKE_EXE_LINKER_FLAGS "")
set(CMAKE_STATIC_LIBRARY_SUFFIX ".a")
set(CMAKE_STATIC_LIBRARY_PREFIX "")

set(SEGALIB $ENV{SATURN_SBL}/segalib)
set(SEGASGL $ENV{SATURN_SGL})

set(SATURN_COMMON $ENV{SATURN_COMMON})

project( segasmp VERSION 1.0
                  DESCRIPTION "Sega Saturn Samples"
                  LANGUAGES C ASM)

add_subdirectory(abs)
add_subdirectory(bup)
add_subdirectory(cof)
add_subdirectory(csh)
add_subdirectory(dbg)
add_subdirectory(dma)
#add_subdirectory(dual)
add_subdirectory(mem)
add_subdirectory(mpg)
add_subdirectory(pcm)
add_subdirectory(sblsgl)
add_subdirectory(snd)
add_subdirectory(spr)
add_subdirectory(sys)
