cmake_minimum_required(VERSION 3.15)

include(cmake/Functions.cmake)

project(openGL_show_case)

add_subdirectory(thirdparty)

add_executable(${PROJECT_NAME} main.cpp)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} imgui)


