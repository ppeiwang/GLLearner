macro(INCLUDE_THIRD_PARTY Scope)
    file(GLOB_RECURSE all_headers ${CMAKE_CURRENT_LIST_DIR}/include/*.h)
    file(GLOB_RECURSE all_imps ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp ${CMAKE_CURRENT_LIST_DIR}/src/*.c)
    #MESSAGE(STATUS "${PROJECT_NAME} : ${all_imps}")
    if(${Scope} STREQUAL INTERFACE)
        add_library(${PROJECT_NAME} INTERFACE)
        target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include/)
        target_sources(${PROJECT_NAME} INTERFACE ${all_headers} INTERFACE ${all_imps})
    else()
        add_library(${PROJECT_NAME})
        target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include/)
        target_sources(${PROJECT_NAME} PRIVATE ${all_headers} PRIVATE ${all_imps})
    endif()
    source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES ${all_headers} FILES ${all_imps})
endmacro(INCLUDE_THIRD_PARTY)


function (COPY_FILE_CONF FILE DEST CONFIG)
  get_filename_component(FILENAME "${FILE}" NAME)
  add_custom_command(TARGET ${PROJECT_NAME} ${CONFIG} COMMAND ${CMAKE_COMMAND} -E copy_if_different "${FILE}" "${DEST}/${FILENAME}")
endfunction (COPY_FILE_CONF)

function (POST_COPY_FILE FILE DEST)
    COPY_FILE_CONF(${FILE} ${DEST} ${COND} POST_BUILD)
endfunction (POST_COPY_FILE)

function (PRE_COPY_FILE FILE DEST)
    COPY_FILE_CONF(${FILE} ${DEST} ${COND} PRE_BUILD)
endfunction (PRE_COPY_FILE)

function (COPY_FILE_COND FILE DEST COND CONFIG)
  get_filename_component(FILENAME "${FILE}" NAME)
  add_custom_command(TARGET ${PROJECT_NAME} ${CONFIG} COMMAND ${CMAKE_COMMAND} -E $<${COND}:echo> $<${COND}:"omitted execute "> copy_if_different "${FILE}" "${DEST}/${FILENAME}")
endfunction (COPY_FILE_COND)

function (POST_COPY_FILE_COND FILE DEST COND)
    COPY_FILE_COND(${FILE} ${DEST} ${COND} POST_BUILD)
endfunction (POST_COPY_FILE_COND)

function (PRE_COPY_FILE_COND FILE DEST COND)
    COPY_FILE_COND(${FILE} ${DEST} ${COND} PRE_BUILD)
endfunction (PRE_COPY_FILE_COND)

function (POST_COPY_FILE_TARGET PROJ FILE DEST)
  get_filename_component(FILENAME "${FILE}" NAME)
  add_custom_command(TARGET ${PROJ} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${FILE}" "${DEST}/${FILENAME}")
endfunction (POST_COPY_FILE_TARGET)

function (PRE_COPY_DIR DIR DEST)
  add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${DIR}" "${DEST}")
endfunction (PRE_COPY_DIR)

function (POST_COPY_DIR DIR DEST)
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${DIR}" "${DEST}")
endfunction (POST_COPY_DIR)

function (POST_COPY_DIR_TARGET PROJ DIR DEST)
  add_custom_command(TARGET ${PROJ} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${DIR}" "${DEST}")
endfunction (POST_COPY_DIR_TARGET)