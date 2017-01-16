cmake_minimum_required(VERSION 3.3)
project(cppNTT)

set(SOURCE_FILES src/GameNode.cpp include/GameNode.h src/main.cpp src/MoveTree.cpp include/MoveTree.h src/MiniMax.cpp include/MiniMax.h src/UI.cpp include/UI.h)

find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH} ../local/lib/bdb/)
link_directories(${MPI_LINK_PATH} ../local/lib/bdb/)

add_executable(cppNTT ${SOURCE_FILES})
target_link_libraries(cppNTT ${MPI_LIBRARIES} db_cxx boost_system boost_filesystem)

if(MPI_COMPILE_FLAGS)
    set_target_properties(cppNTT PROPERTIES
            COMPILE_FLAGS "${MPI_COMPILE_FLAGS} ")
endif()

if(MPI_LINK_FLAGS)
    set_target_properties(cppNTT PROPERTIES
            LINK_FLAGS "${MPI_LINK_FLAGS}")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Ofast -fopenmp")


