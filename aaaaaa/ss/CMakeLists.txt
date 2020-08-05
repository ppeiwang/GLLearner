cmake_minimum_required(VERSION 3.15)

include(cmake/Functions.cmake)

project(openGL_show_case)

add_subdirectory(thirdparty)

add_executable(${PROJECT_NAME})

file(GLOB_RECURSE srcs ${CMAKE_CURRENT_LIST_DIR}/include/*.h ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

target_sources(${PROJECT_NAME} PUBLIC ${srcs})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include/)

source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES ${srcs})

target_link_libraries(${PROJECT_NAME} imgui)
target_link_libraries(${PROJECT_NAME} glm)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})


