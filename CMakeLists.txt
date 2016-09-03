cmake_minimum_required(VERSION 3.6)
project(cppNTT)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/GameNode.cpp src/GameNode.h src/main.cpp src/MoveTree.cpp src/MoveTree.h)
add_executable(cppNTT ${SOURCE_FILES})