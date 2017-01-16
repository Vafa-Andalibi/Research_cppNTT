#!/bin/bash

#This is for Big Red II compilation
module unload PrgEnv-cray
module load PrgEnv-gnu
module load openmpi
module load boost
mpicxx -std=c++11 -Ofast -fopenmp -o ../search ~/cppNTT/src/GameNode.cpp ~/cppNTT/src/main.cpp ~/cppNTT/src/MoveTree.cpp ~/cppNTT/src/MiniMax.cpp ~/cppNTT/src/UI.cpp -I../local/lib/bdb -lboost_system -lboost_filesystem -L../local/lib/bdb -ldb_cxx -I/N/soft/cle4/openmpi/gnu/1.8.4/include -pthread -Wl,-rpath -Wl,/N/soft/cle4/openmpi/gnu/1.8.4/lib -Wl,--enable-new-dtags -L/N/soft/cle4/openmpi/gnu/1.8.4/lib -lmpi_cxx -lmpi
