//
// Created by jclangst on 9/3/2016.
//
#include "../libs/rocksdb/include/rocksdb/db.h"
#include "../libs/rocksdb/include/rocksdb/status.h"
#include "../libs/rocksdb/include/rocksdb/options.h"
#include <iostream>
#include <cassert>
#include "GameNode.h"

int main() {
    GameNode node = GameNode(3);
    std::array<char, 2> test = {2, 0};
    std::cout << node.rBoard(1);
    rocksdb::DB* db;
    rocksdb::Options options;
    options.create_if_missing = true;
    rocksdb::Status status = rocksdb::DB::Open(options, "/tmp/testdb", &db);

    delete db;
}