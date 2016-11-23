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
    Db* locks;
    DbEnv* lockEnv;
    char* dir;

    MoveTree(char n, char* dir);


    void lock(GameNode &board);
    void unlock(GameNode &board);
    bool isLocked(GameNode &board);

    void put(GameNode &node, char i);
    char get(GameNode &board);

    std::shared_ptr<GameNode> getStart(GameNode &node);

    void open(char n);
    void close();
};


#endif //CPPNTT_MOVETREE_H
