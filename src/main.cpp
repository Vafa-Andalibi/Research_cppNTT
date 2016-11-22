//
// Created by jclangst on 9/3/2016.
//
#include "rocksdb/db.h"
#include <iostream>
#include <ctime>
#include "GameNode.h"
#include "MoveTree.h"
#include "MiniMax.h"
#include "mpi/mpi.h"
#include "UI.h"

#define N 5

int main(int argc, char *argv[]) {

    int id;
    int ierr;
    int p;

    //initialize MPI
    ierr = MPI_Init(&argc, &argv);

    //get the number of processes
    ierr = MPI_Comm_size(MPI_COMM_WORLD, &p);

    //get the specific process ID
    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &id);

    //UI ui;
    unsigned char n;
   // n = ui.getN();


    std::shared_ptr<MoveTree> moves(new MoveTree(N));
    MiniMax search = MiniMax(moves);


    std::clock_t begin = clock();




    std::cout << "Iniating Process " << id << std::endl;

    std::cout << id << ": SEARCH - " << &search << std::endl;
    GameNode node = GameNode(N);
    moves->get(node);


    std::cout << id << " NODE - " << &node << std::endl;
    search.initiate(node, id);

    std::cout << id << " DONE" << std::endl;
    //terminate process
    MPI_Finalize();



    //ui.explore(std::shared_ptr<GameNode>(new GameNode(N)), *moves);


    if(id == 0){
        std::clock_t end = clock();
        std::cout << (end - begin)/CLOCKS_PER_SEC << std::endl;
        moves->close();
    }

    return 0;
}





//    // Structure that defines hashing and comparison operations for user's type.
//    struct MyHashCompare {
//        static size_t hash( const string& x ) {
//            size_t h = 0;
//            for( const char* s = x.c_str(); *s; ++s )
//                h = (h*17)^*s;
//            return h;
//        }
//        //! True if strings are equal
//        static bool equal( const string& x, const string& y ) {
//            return x==y;
//        }
//    };
//
//    // A concurrent hash table that maps strings to ints.
//    typedef concurrent_hash_map<string,int> StringTable;
//
//    StringTable::accessor a;
//    StringTable tbl;
//    tbl.insert(a, "jack");
//    a->second = (int)2;
//
//    a.release();
//
//    StringTable::accessor b;
//
//    tbl.find(b, "jack");
//    cout << b->second << endl;
//
//    delete db;
//}