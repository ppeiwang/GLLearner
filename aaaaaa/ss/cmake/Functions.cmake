macro(INCLUDE_THIRD_PARTY Scope)
    file(GLOB_RECURSE all_headers ${CMAKE_CURRENT_LIST_DIR}/include/*.h)
    file(GLOB_RECURSE all_imps ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
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