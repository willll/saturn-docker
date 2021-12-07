cmake_minimum_required(VERSION 3.0)

SET(CMAKE_SYSTEM_NAME "SEGA SATURN")

set(SAMPLES_DESTINATION Samples)

set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
set(CMAKE_EXE_LINKER_FLAGS "")
set(CMAKE_STATIC_LIBRARY_SUFFIX ".a")
set(CMAKE_STATIC_LIBRARY_PREFIX "")

set(SAMPLES $ENV{SATURN_SAMPLES})

set(SATURN_SAMPLE_COMMON $ENV{SATURN_SAMPLES}/Saturn-SDK-Sample-Common)

project( Samples VERSION 1.0
                  DESCRIPTION "Sega Saturn Samples"
                  LANGUAGES C ASM)

add_subdirectory(Saturn-SDK-Sample-00-Basic)
add_subdirectory(Saturn-SDK-Sample-01-Sprite)
add_subdirectory(Saturn-SDK-Sample-02-TexturedSprite)
#add_subdirectory(Saturn-SDK-Sample-13-CDBrowser)
