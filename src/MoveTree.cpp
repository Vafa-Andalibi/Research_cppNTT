//
// Created by jclangst on 9/3/2016.
//

#include <iostream>
#include <queue>
#include "MoveTree.h"

MoveTree::MoveTree(rocksdb::DB &resolved){
    this->resolved = &resolved;
};

MoveTree::MoveTree(char n){
    this->open(n);
    this->locks =  new cuckoohash_map<unsigned long long, char, CityHasher<unsigned long long> > (50 * n * n,0.6,0);
}

void MoveTree::lock(const GameNode &board){
    this->locks->insert(board.hash);
}

void MoveTree::unlock(const GameNode &board){
    this->locks->erase(board.hash);
}

bool MoveTree::isLocked(const GameNode &board){
    char i;
    return this->locks->find(board.hash, i);
}


std::shared_ptr<GameNode> MoveTree::getStart(const GameNode &node){
    if(isLocked(node)){
        std::vector<std::shared_ptr<GameNode> > children = node.succ();

        //load inital q
        std::queue<std::shared_ptr<GameNode> > q;
        for(std::vector<std::shared_ptr<GameNode> >::iterator child = children.begin(); child != children.end(); ++child){
            q.push(child.operator*());
        }

        while(!q.empty()) {
            for (std::vector<std::shared_ptr<GameNode> >::iterator child = children.begin();
                 child != children.end(); ++child) {
                if (get(**child) == -1) {
                    if (!isLocked(**child)) {
                        return child.operator*();
                    } else {
                        q.push(child.operator*());
                    }
                }
            }

            //get the next non-resolved element
            std::shared_ptr<GameNode> next;
            do {
                next = q.front();
                q.pop();
                if(get(*next) != -1){
                    children = (*next).succ();
                    break;
                }
            }while(!q.empty());
        }

        return nullptr;
    }else{
        return std::make_shared<GameNode>(node);
    }
}


void MoveTree::put(const GameNode &board, char i){
    this->resolved->Put(rocksdb::WriteOptions(), std::to_string(board.hash), &i);
}

char MoveTree::get(const GameNode &board){
    std::string value;
    rocksdb::Status s;
    s = this->resolved->Get(rocksdb::ReadOptions(), std::to_string(board.hash), &value);
    if(s.IsNotFound()){
        return -1;
    }else {
        return value[0];
    }
}



void MoveTree::open(char n){
    rocksdb::DB* db;
    rocksdb::Options options;
    options.create_if_missing = true;
    std::string directory = "/home/jclangst/nttDB";
    rocksdb::Status status = rocksdb::DB::Open(options, "/home/jclangst/testing/", &db);
    this->resolved = db;
}
void MoveTree::close(){
    delete this->resolved;
}