//
// Created by jclangst on 9/3/2016.
//

#ifndef CPPNTT_MOVETREE_H
#define CPPNTT_MOVETREE_H


#include <rocksdb/db.h>
#include <libcuckoo/city_hasher.hh>
#include <libcuckoo/cuckoohash_map.hh>
#include "GameNode.h"

class MoveTree {

    public:
    rocksdb::DB* resolved;
    cuckoohash_map<unsigned long long, char, CityHasher<unsigned long long> >* locks;

    MoveTree(char n);
    MoveTree(rocksdb::DB& movetree);

    void lock(const GameNode &board);
    void unlock(const GameNode &board);
    bool isLocked(const GameNode &board);

    void put(const GameNode &node, char i);
    char get(const GameNode &board);

    std::shared_ptr<GameNode> getStart(const GameNode &node);

    void open(char n);
    void close();
};


#endif //CPPNTT_MOVETREE_H
