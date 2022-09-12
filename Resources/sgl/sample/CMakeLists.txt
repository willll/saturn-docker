cmake_minimum_required(VERSION 3.0)

SET(CMAKE_SYSTEM_NAME Generic)

set(SEGASGL_DESTINATION samples/sample1)
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

add_subdirectory(akira)
add_subdirectory(akira3)
add_subdirectory(demo_a)
add_subdirectory(demo_b)
add_subdirectory(demo_c)
add_subdirectory(demo_d)
add_subdirectory(demogun)
add_subdirectory(design1)
add_subdirectory(design2)
add_subdirectory(design3)
add_subdirectory(driving)
add_subdirectory(driving2)
add_subdirectory(fighting)
add_subdirectory(s_2_2)
add_subdirectory(s_3_2)
add_subdirectory(s_4_2)
add_subdirectory(s_4_3_1)
