cmake_minimum_required(VERSION 3.15)

include(cmake/Functions.cmake)

project(openGL_show_case)

add_subdirectory(thirdparty)

add_executable(${PROJECT_NAME})

file(GLOB_RECURSE srcs ${CMAKE_CURRENT_LIST_DIR}/include/*.h ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

target_sources(${PROJECT_NAME} PUBLIC ${srcs})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include/)

source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES ${srcs})

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Bin)

target_link_libraries(${PROJECT_NAME} imgui)
target_link_libraries(${PROJECT_NAME} glm)
target_link_libraries(${PROJECT_NAME} glad)
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} stb_image)
target_link_libraries(${PROJECT_NAME} opengl32.lib)
target_link_libraries(${PROJECT_NAME} assimp)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME}) # set the default project of our visual studio solution
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

# copy the assets folder to Binary directory
#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND}
#    -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets)

get_property(ASSIMP_DEBUG_LIB TARGET assimp PROPERTY IMPORTED_LOCATION_DEBUG)
get_property(ASSIMP_RELEASE_LIB TARGET assimp PROPERTY IMPORTED_LOCATION_RELEASE)
set(ASSIMP_SHARED_LIB $<IF:$<CONFIG:Debug>, ${ASSIMP_DEBUG_LIB}, ${ASSIMP_RELEASE_LIB}>)

#message("Current output path: ${ASSIMP_DEBUG_LIB}")

#add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND}
#    -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets)	

#PRE_COPY_FILE(${ASSIMP_SHARED_LIB} $<TARGET_FILE_DIR:${PROJECT_NAME}>)

POST_COPY_DIR($<$<CONFIG:Debug> "${CMAKE_SOURCE_DIR}/assets" > "${CMAKE_BINARY_DIR}/assets")


