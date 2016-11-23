#!/bin/bash

#This is for Big Red II compilation
module unload PrgEnv-cray
module load PrgEnv-gnu
module load openmpi
module load boost
mpicxx -std=c++11 -Ofast -fopenmp -o ../search src/GameNode.cpp src/GameNode.h src/main.cpp src/MoveTree.cpp src/MoveTree.h src/MiniMax.cpp src/MiniMax.h src/UI.cpp src/UI.h -I../local/lib/bdb -lboost_system -lboost_filesystem -L../local/lib/bdb -ldb_cxx
