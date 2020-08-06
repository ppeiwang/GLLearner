cmake_minimum_required(VERSION 3.15)

project(imgui)

INCLUDE_THIRD_PARTY(PUBLIC)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glad)
