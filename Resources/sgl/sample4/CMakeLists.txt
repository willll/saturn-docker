cmake_minimum_required(VERSION 3.0)

SET(CMAKE_SYSTEM_NAME Generic)

set(SEGASGL_DESTINATION samples/sample4)
set(INSTALL_DESTINATION ${SEGASGL_DESTINATION})

set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
set(CMAKE_EXE_LINKER_FLAGS "")
set(CMAKE_STATIC_LIBRARY_SUFFIX ".a")
set(CMAKE_STATIC_LIBRARY_PREFIX "")

set(SEGALIB $ENV{SATURN_SBL}/segalib)
set(SEGASGL $ENV{SATURN_SGL})

set(SATURN_COMMON $ENV{SATURN_COMMON})
set(SATURN_CMAKE $ENV{SATURN_CMAKE})

project( sample VERSION 1.0
                  DESCRIPTION "Sega Saturn SGL samples from SGL 3.20"
                  LANGUAGES C ASM)

set(as_flags )
set(c_flags -Wall -m2 -MMD)

add_subdirectory(bitmap)
add_subdirectory(cdda)
add_subdirectory(coloff)
add_subdirectory(expccalc)
add_subdirectory(grad)
add_subdirectory(linecol)
add_subdirectory(linscl/truecol)
add_subdirectory(pcm)
add_subdirectory(rotatebg)
add_subdirectory(shadow)
add_subdirectory(sound)
add_subdirectory(spccalc)
add_subdirectory(spprio)
add_subdirectory(sprcc2bg)
add_subdirectory(window/sprwin)
