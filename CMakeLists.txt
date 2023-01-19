cmake_minimum_required(VERSION 3.0)

project(RedBlackTree)

include_directories(${CMAKE_SOURCE_DIR}/include)

aux_source_directory(${CMAKE_SOURCE_DIR}/include/ Sources)

add_executable(main ${CMAKE_SOURCE_DIR}/src/main.cpp ${Sources})