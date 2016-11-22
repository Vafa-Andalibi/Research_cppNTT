//
// Created by jclangst on 9/3/2016.
//
#include "rocksdb/db.h"
#include "tbb/concurrent_hash_map.h"
#include <iostream>
#include <ctime>
#include "GameNode.h"
#include "MoveTree.h"
#include "MiniMax.h"
#include "libcuckoo/cuckoohash_map.hh"
#include "libcuckoo/city_hasher.hh"


int main() {
    MoveTree moves = MoveTree(5);
    MiniMax search = MiniMax(&moves);
    GameNode node = GameNode(5);


    std::clock_t begin = clock();
    search.initiate(node);

    std::clock_t end = clock();

    std::cout << (end - begin)/CLOCKS_PER_SEC << std::endl;

    moves.close();


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