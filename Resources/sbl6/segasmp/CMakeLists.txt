cmake_minimum_required(VERSION 3.5)

set(CMAKE_SYSTEM_NAME Generic)

set(SEGASMP_DESTINATION segasmp)
set(INSTALL_DESTINATION ${SEGASMP_DESTINATION})
set(SGL_ENABLE $ENV{INSTALL_SGL_LIB})


set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
set(CMAKE_EXE_LINKER_FLAGS "")
set(CMAKE_STATIC_LIBRARY_SUFFIX ".a")
set(CMAKE_STATIC_LIBRARY_PREFIX "")

set(SEGALIB $ENV{SATURN_SBL}/segalib)
set(SEGASGL $ENV{SATURN_SGL})

set(SATURN_COMMON $ENV{SATURN_COMMON})
set(SATURN_CMAKE $ENV{SATURN_CMAKE})

project( segasmp VERSION 1.0
                  DESCRIPTION "Sega Saturn Samples"
                  LANGUAGES C ASM)

set(as_flags )
set(c_flags -O0 -Wall -m2 -MMD)

set(ip_maker_verbose ) # set to -v for debug

add_subdirectory(abs)
add_subdirectory(bup)
add_subdirectory(cof)
add_subdirectory(csh)
add_subdirectory(dbg)
add_subdirectory(dma)
#add_subdirectory(dual)
add_subdirectory(fld)
if(${SGL_ENABLE})
  add_subdirectory(game)
  add_subdirectory(game_cd)
  add_subdirectory(sblsgl)
  add_subdirectory(sgl)
else()
  message(WARNING "SGL is not installed.")
endif()
add_subdirectory(gfs)
add_subdirectory(lib)
add_subdirectory(man)
add_subdirectory(mem)
add_subdirectory(mpg)
add_subdirectory(pcm)
add_subdirectory(per)
add_subdirectory(scl)
add_subdirectory(snd)
add_subdirectory(spr)
add_subdirectory(stm)
add_subdirectory(sys)
add_subdirectory(tim)
add_subdirectory(v_blank)
