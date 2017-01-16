//
// Created by jclangst on 9/3/2016.
//
#include <iostream>
#include <ctime>
#include "../include/GameNode.h"
#include "../include/MoveTree.h"
#include "../include/MiniMax.h"
#include "mpi.h"
#include "../include/UI.h"
#include <cstring>
#include <chrono>

bool getInteractive(int argc, char *argv[]);
unsigned char getN(int argc, char *argv[]);
int getRAM(int argc, char **argv);
int getThreads(int argc, char *argv[]);
char* getDBDirectory(int argc, char *argv[]);
bool getRemove(int argc, char *argv[]);

int main(int argc, char *argv[]) {

    //program variables
    UI ui;
    unsigned char n;
    bool interactive = false;
    int ram;
    int threads;
    char* dir = nullptr;
    int dirLen = 0;

    //MPI variables
    int id;
    int ierr;
    int p;

    //initialize MPI
    ierr = MPI_Init(&argc, &argv);

    //get the number of processes
    ierr = MPI_Comm_size(MPI_COMM_WORLD, &p);

    //get the specific process ID
    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &id);


    //gets critical program parameters
    if(id == 0){
        interactive = getInteractive(argc, argv);
        if(p>1) MPI_Send(&interactive, 1, MPI_CXX_BOOL, 1, 0, MPI_COMM_WORLD);

        //gets the ram
        ram = getRAM(argc, argv);
        if(ram == 0){
            std::cout << "RAM not correctly specified (-ram _)" << std::endl;
            //MPI_Abort(MPI_COMM_WORLD, 1);
        }else{
            if(p > 1) MPI_Send(&ram, 1, MPI_INT, 1, 2, MPI_COMM_WORLD);
        }

        


        //gets the threads per node
        threads = getThreads(argc, argv);
        if(threads == 0){
            std::cout << "Threads not correctly specified (-threads _)" << std::endl;
            MPI_Abort(MPI_COMM_WORLD, 1);
        }else{
            if(p > 1)MPI_Send(&threads, 1, MPI_INT, 1, 3, MPI_COMM_WORLD);
        }

        //gets the database directory
        dir = getDBDirectory(argc, argv);
        dirLen = (int) std::strlen(dir);
        if(dir == nullptr){
            std::cout << "Directory not correctly specified (-dir _)" << std::endl;
            MPI_Abort(MPI_COMM_WORLD, 1);
        }else{
            if(p > 1) MPI_Send(&dirLen, 1, MPI_INT, 1, 6, MPI_COMM_WORLD);
            if(p > 1) MPI_Send(dir, dirLen + 1, MPI_CHAR, 1, 5, MPI_COMM_WORLD);
        }


    }else{
        //receive interactive directive
        MPI_Recv(&interactive, 1, MPI_INT, id-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(&interactive, 1, MPI_CXX_BOOL, (id+1)%p, 0, MPI_COMM_WORLD);
        //receive ram
        MPI_Recv(&ram, 1, MPI_INT, id-1, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(&ram, 1, MPI_INT, (id+1)%p, 2, MPI_COMM_WORLD);
        //receive threads
        MPI_Recv(&threads, 1, MPI_INT, id-1, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(&threads, 1, MPI_INT, (id+1) %p, 3, MPI_COMM_WORLD);
        //receive directory
        MPI_Recv(&dirLen, 1, MPI_INT,id-1,6,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        MPI_Send(&dirLen, 1, MPI_INT, (id+1)%p, 6, MPI_COMM_WORLD);
        dir = new char[dirLen];
        MPI_Recv(dir, dirLen + 1, MPI_CHAR, id-1, 5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(dir, dirLen + 1, MPI_CHAR, (id+1) %p, 5, MPI_COMM_WORLD);
    }


    //interactive mode
    if(interactive && id == 0){

        n = ui.getN();
        std::shared_ptr<MoveTree> moves(new MoveTree(n, dir));
        ui.explore(std::shared_ptr<GameNode>(new GameNode(n)), *moves);

    //headless mode
    }else if(!interactive) {

        //gets the relevant values
        if (id == 0) {

            //gets the correct n
            n = getN(argc, argv);

            //deletes the existing database if necessary
            if (getRemove(argc, argv)) {
                std::cout << "Removing DB" << std::endl;
                MoveTree::removeDb(n, dir);
            }

            //sends n
            if (n == 0) {
                std::cout << "N not correctly specified (-n _)" << std::endl;
                MPI_Abort(MPI_COMM_WORLD, 1);
            } else {
                if (p > 1) MPI_Send(&n, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
            }


        } else {
            //receive n
            MPI_Recv(&n, 1, MPI_INT, id - 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(&n, 1, MPI_INT, (id + 1) % p, 1, MPI_COMM_WORLD);
        }

        auto begin = std::chrono::high_resolution_clock::now();
        //searching algorithm
        std::shared_ptr<MoveTree> moves(new MoveTree(n, dir));
        MiniMax search = MiniMax(moves);
        GameNode node(n);
        search.initiate(node, id, threads);


        if (id == 0) {
            auto end = std::chrono::high_resolution_clock::now();
            std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() << "ns" << std::endl;
        }
    }

    std::cout << id << " DONE" << std::endl;
    MPI_Finalize();


    return 0;
}


//checks arguments for interactive mode or not
bool getInteractive(int argc, char *argv[]){
    for(int i = 0; i < argc; i++){
        if(std::strcmp(argv[i], "-i") == 0){
            return true;
        }
    }
    return false;
}

//checks arguments for remove existing database
bool getRemove(int argc, char *argv[]){
    for(int i = 0; i < argc; i++){
        if(std::strcmp(argv[i], "-r") == 0){
            return true;
        }
    }
    return false;
}

//checks the arguments for n
unsigned char getN(int argc, char *argv[]){
    for(int i = 0; i < argc; i++){
        if(std::strcmp(argv[i], "-n") == 0){
            return (unsigned char) std::atoi(argv[i+1]);
        }
    }
    return 0;
}

//checks the arguments for ram
int getRAM(int argc, char **argv){
    for(int i = 0; i < argc; i++){
        if(std::strcmp(argv[i], "-ram") == 0){
            return std::atoi(argv[i+1]);
        }
    }
    return 0;
}

//checks the arguments for threads
int getThreads(int argc, char *argv[]){
    for(int i = 0; i < argc; i++){
        if(std::strcmp(argv[i], "-threads") == 0){
            return std::atoi(argv[i+1]);
        }
    }
    return 0;
}

//gets the directory for the database
char* getDBDirectory(int argc, char *argv[]){
    for(int i = 0; i < argc; i++){
        if(std::strcmp(argv[i], "-dir") == 0){
            return argv[i+1];
        }
    }
    return nullptr;
}

