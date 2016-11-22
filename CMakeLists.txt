cmake_minimum_required(VERSION 3.6)
project(cppNTT)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Ofast -fopenmp")
link_directories(/usr/local/lib)
set(SOURCE_FILES src/GameNode.cpp src/GameNode.h src/main.cpp src/MoveTree.cpp src/MoveTree.h src/MiniMax.cpp src/MiniMax.h)
add_executable(cppNTT ${SOURCE_FILES})
target_link_libraries(cppNTT rocksdb gflags bz2 snappy pthread z tbb cityhash)