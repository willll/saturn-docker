cmake_minimum_required(VERSION 3.5)

SET(CMAKE_SYSTEM_NAME Generic)

set(SEGASMP_DESTINATION examples)
set(INSTALL_DESTINATION ${SEGASMP_DESTINATION})

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
                  DESCRIPTION "Sega Saturn Examples"
                  LANGUAGES C ASM)

add_subdirectory(backram)
add_subdirectory(devcon96)
# dualcpu is not built: it is the Hitachi compiler build of segasmp/dual (built there), and links slave.lib, a binary Hitachi library without source
#add_subdirectory(dualcpu)
add_subdirectory(gfsdemo)
add_subdirectory(periphs)
# sgl is not built: it is the same as the demos of SGL, built by build-sgl302-samples.sh
#add_subdirectory(sgl)
