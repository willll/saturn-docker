set(SOURCES
 scl_back.c  scl_func.c  scl_pr04.c  scl_pr10.c  scl_ro00.c  scl_ro10.c
scl_fu01.c  scl_lncl.c  scl_pr05.c  scl_pr11.c  scl_ro01.c  scl_vblv.c
scl_fu02.c  scl_mosa.c  scl_pr06.c  scl_pr12.c  scl_ro02.c  scl_win0.c
scl_fu03.c  scl_pr01.c  scl_pr07.c  scl_pr13.c  scl_ro03.c  scl_win1.c
scl_fu04.c  scl_pr02.c  scl_pr08.c  scl_pr14.c  scl_ro04.c  scl_win2.c
scl_fu05.c  scl_pr03.c  scl_pr09.c  scl_pri.c   scl_ro05.c  scl_win3.c  )

add_compile_definitions(_SH MODEL_S USE_DSP)

ADD_LIBRARY( sega_scl STATIC
             ${SOURCES} )

include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/gfs)

SET_TARGET_PROPERTIES(sega_scl PROPERTIES PREFIX "")

install(TARGETS sega_scl DESTINATION ${SEGALIB_DESTINATION_LIB})
