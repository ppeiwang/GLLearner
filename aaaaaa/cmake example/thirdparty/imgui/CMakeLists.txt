cmake_minimum_required(VERSION 3.15)

project(imgui)

INCLUDE_THIRD_PARTY(PUBLIC)

#add_library(imgui)

#file(GLOB_RECURSE srcs
#${CMAKE_CURRENT_LIST_DIR}/include/*.h
#${CMAKE_CURRENT_LIST_DIR}/src/*.cpp
#)

#target_sources(imgui
#PUBLIC
#${srcs}
#)

#source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES ${srcs})

#target_include_directories(imgui INTERFACE
#    ${CMAKE_CURRENT_LIST_DIR}/include/
#)