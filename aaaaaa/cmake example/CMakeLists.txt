cmake_minimum_required(VERSION 3.15)

project(cmake_show)

macro(INCLUDE_THIRD_PARTY Scope)
    if(Scope STREQUAL INTERFACE)
        add_library(${PROJECT_NAME} INTERFACE)
    else()
        add_library(${PROJECT_NAME})       
    endif()
    file(GLOB_RECURSE all_files ${CMAKE_CURRENT_LIST_DIR}/include/*.h ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
    target_sources(${PROJECT_NAME} ${Scope} ${all_files})
    source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES ${all_files})
    target_include_directories(${PROJECT_NAME} ${Scope} ${CMAKE_CURRENT_LIST_DIR}/include/) 
endmacro(INCLUDE_THIRD_PARTY)

add_subdirectory(thirdparty)

add_executable(${PROJECT_NAME} main.cpp)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} imgui)


