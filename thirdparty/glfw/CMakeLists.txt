project(glfw)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include/)

set(glfw_lib_path ${CMAKE_CURRENT_LIST_DIR}/lib)

#target_link_directories(${PROJECT_NAME} INTERFACE ${glfw_lib_path})

target_link_libraries(${PROJECT_NAME} INTERFACE debug ${glfw_lib_path}/glfw3d.lib INTERFACE optimized ${glfw_lib_path}/glfw3.lib)