//
// Created by jclangst on 9/3/2016.
//

#ifndef CPPNTT_MOVETREE_H
#define CPPNTT_MOVETREE_H


#include <libcuckoo/city_hasher.hh>
#include <libcuckoo/cuckoohash_map.hh>
#include <db_cxx.h>
#include "GameNode.h"

class MoveTree {

    public:
    Db* resolved;
    DbEnv* resEnv;
    char* dir;

    MoveTree(char n, char* dir);


    void put(std::shared_ptr<GameNode> board, char i);
    char get(std::shared_ptr<GameNode> board);

    static void removeDb(char n, char* dir);
    void open(char n);
    void close();
};


#endif //CPPNTT_MOVETREE_H
