cmake_minimum_required(VERSION 3.5)

SET(CMAKE_SYSTEM_NAME Generic)

set(SEGASGL_DESTINATION samples/sample2)
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
                  DESCRIPTION "Sega Saturn SGL samples"
                  LANGUAGES C ASM)

set(as_flags )
set(c_flags -Wall -m2 -MMD)

add_subdirectory(sega2d_1)
add_subdirectory(sega3d_1)
add_subdirectory(sega3d_2)
add_subdirectory(sega3d_3)
