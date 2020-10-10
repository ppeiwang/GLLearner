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


get_property(ASSIMP_DEBUG_LIB TARGET assimp PROPERTY IMPORTED_LOCATION_DEBUG)
get_property(ASSIMP_RELEASE_LIB TARGET assimp PROPERTY IMPORTED_LOCATION_RELEASE)


# *This section would get a wrong file name which has a '>' suffix*
#set(ASSIMP_SHARED_LIB_PATH $<IF:$<CONFIG:Debug>,${ASSIMP_DEBUG_LIB},${ASSIMP_RELEASE_LIB}>)
#get_filename_component(FILENAME "${ASSIMP_SHARED_LIB_PATH}" NAME)
#file(GENERATE OUTPUT filename CONTENT "${FILENAME}")

# *The same to the below one
#set(MY_Lib $<$<CONFIG:Debug>:${ASSIMP_DEBUG_LIB}>$<$<CONFIG:Release>:${ASSIMP_RELEASE_LIB}>)
#get_filename_component(FILENAME "${MY_Lib}" NAME)
#file(GENERATE OUTPUT filename CONTENT "${FILENAME}") 

# *And this one would always jump into the else branch
#IF($<CONFIG:Debug>)
#get_property(ASSIMP_LIB_PATH TARGET assimp PROPERTY IMPORTED_LOCATION_DEBUG)
#ELSE()
#get_property(ASSIMP_LIB_PATH TARGET assimp PROPERTY IMPORTED_LOCATION_RELEASE)
#ENDIF()

#Customized function which is defined in Functions.cmake

PRE_COPY_FILE(${ASSIMP_DEBUG_LIB} "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
PRE_COPY_FILE(${ASSIMP_RELEASE_LIB} "$<TARGET_FILE_DIR:${PROJECT_NAME}>")

POST_COPY_DIR("${CMAKE_SOURCE_DIR}/assets" "${CMAKE_BINARY_DIR}/assets")


