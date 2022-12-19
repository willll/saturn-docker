
function(GetItRight resultVar)
  set(result)
  foreach(ITR ${ARGN})  # ARGN holds all arguments to function after last named one
    if(ITR MATCHES "(.*)=(.*)")
      list(APPEND result ${ITR})
    endif()
  endforeach()
  set(${resultVar} ${result} PARENT_SCOPE)
endfunction()

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.bin
          DESTINATION ${INSTALL_DESTINATION}/${TARGET_PATH}/CD
          RENAME 0.bin )

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/IP.BIN
              DESTINATION ${INSTALL_DESTINATION}/${TARGET_PATH}/CD )
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_NAME}.cue DESTINATION ${INSTALL_DESTINATION}/${TARGET_PATH} )

list(TRANSFORM CD_FILES PREPEND "${CMAKE_INSTALL_PREFIX}/${INSTALL_DESTINATION}/${TARGET_PATH}/CD/")

# Not very nice, it is match the pathspec file grammar (e.g. output=input syntax)
set(files_to_write)
foreach(ITR ${CD_FILES})  # ARGN holds all arguments to function after last named one
  if(ITR MATCHES "(.*)=(.*)")
    list(APPEND files_to_write "${CMAKE_MATCH_2}=${CMAKE_MATCH_1}")
  else()
      list(APPEND files_to_write ${ITR})
  endif()
endforeach()
list(TRANSFORM files_to_write APPEND "\\n" )

set (filename ${CMAKE_INSTALL_PREFIX}/${INSTALL_DESTINATION}/${TARGET_PATH}/CD/FILES.txt)
install(CODE "file(WRITE ${filename} ${files_to_write})")

set(MAKE_CMD $ENV{SATURN_CD}/create_cd.sh
    "${TARGET_NAME}" "\"SBL\"" "\"SBL\"" IP.BIN
    "${TARGET_NAME}.iso" "${CMAKE_INSTALL_PREFIX}/${INSTALL_DESTINATION}/${TARGET_PATH}/")

install(CODE "execute_process(COMMAND
                                ${MAKE_CMD}
                                WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/${INSTALL_DESTINATION}/${TARGET_PATH}/CD)")
