set(SOURCES dma_cpu0.c dma_cpu0.h dma_cpu1.c dma_cpu2.c dma_cpu3.c dma_cpu4.c
    dma_cpu5.c dma_cpum.c dma_cpum.h dma_scu0.c dma_scu0.h dma_scu1.c dma_scu1.h
    dma_xpt.h )

add_compile_definitions(_SH MODEL_S)

add_library( sega_dma STATIC
             ${SOURCES} )

include_directories(${PROJECT_SOURCE_DIR}/include)

SET_TARGET_PROPERTIES(sega_dma PROPERTIES PREFIX "")

install(TARGETS sega_dma DESTINATION ${SEGALIB_DESTINATION_LIB})
