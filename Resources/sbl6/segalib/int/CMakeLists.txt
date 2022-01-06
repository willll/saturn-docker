find_package(Boost 1.78.0)

set(SOURCES
    patch_int.c
    int.c
    )

add_compile_definitions(_SH MODEL_S)

add_library( sega_int STATIC
             ${SOURCES}
             )

if(Boost_FOUND)
  include_directories(
                      ${PROJECT_SOURCE_DIR}/include
                      ${Boost_INCLUDE_DIRS}
                      )
else()
  include_directories(
                      ${PROJECT_SOURCE_DIR}/include
                      $ENV{BOOST_INCLUDEDIR}
                      )
endif()

SET_TARGET_PROPERTIES(sega_int PROPERTIES PREFIX "")

install(TARGETS sega_int DESTINATION ${SEGALIB_DESTINATION_LIB})
