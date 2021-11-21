cmake_minimum_required(VERSION 3.0)

SET(CMAKE_SYSTEM_NAME "SEGA SATURN")

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
set(CMAKE_EXE_LINKER_FLAGS "")
set(CMAKE_STATIC_LIBRARY_SUFFIX ".a")
set(CMAKE_STATIC_LIBRARY_PREFIX "")


set(SEGALIB $ENV{SATURN_SBL}/segalib)

project( segasmp VERSION 1.0
                  DESCRIPTION "Sega Saturn Samples"
                  LANGUAGES C)

#add_compile_options(-Wall -Wpointer-sign -m2 -O2)

add_subdirectory(bup)


#install(DIRECTORY include DESTINATION segalib/include)
