cmake_minimum_required(VERSION 3.5)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/smpspr2)
if(${SGL_ENABLE})
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/smpspr7)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/smpspr8)
endif()
