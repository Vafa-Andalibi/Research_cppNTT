#!/bin/bash

#PBS -l nodes=1:ppn=4
#PBS -l walltime=00:10:00
#PBS -N CompileNoTakTo
#PBS -q debug_cpu
#PBS -l gres=ccm
#PBS -V

ccmrun mpicxx -std=c++11 -Ofast -fopenmp -o ../search src/GameNode.cpp src/GameNode.h src/main.cpp src/MoveTree.cpp src/MoveTree.h src/MiniMax.cpp src/MiniMax.h src/UI.cpp src/UI.h -I../local/lib/bdb -lboost_system -lboost_filesystem -L../local/lib/bdb -ldb_cxx -I/N/soft/cle4/openmpi/gnu/1.8.4/include -pthread -Wl,-rpath -Wl,/N/soft/cle4/openmpi/gnu/1.8.4/lib -Wl,--enable-new-dtags -L/N/soft/cle4/openmpi/gnu/1.8.4/lib -lmpi_cxx -lmpi
