//
// Created by jclangst on 9/3/2016.
//

#include "GameNode.h"
#include <array>
#include <vector>
#include <iostream>
#include <bits/shared_ptr.h>

unsigned char GameNode::n = 0;



GameNode::GameNode(unsigned char n){
    GameNode::n = n;
    this->board = 0;
    this->hash = 0;
    this->possibleMoves = std::unordered_set<unsigned char>();
    for(int i = 0; i < (n * n); i++){
        this->possibleMoves.insert(i);
    }
}

GameNode::GameNode(const GameNode &node){
    this->board = node.board;
    this->hash = node.hash;
    this->possibleMoves = std::unordered_set<unsigned char>(node.possibleMoves);
}



/*****************************************
//Making Moves
******************************************/

//makes a move and updates the child's possible moves and hash
void GameNode::makeMove(unsigned char move){
    this->board |= 1 << move;
    this->possibleMoves.erase(move);
    this->updateHash();
    prune(move);
}

//updates the hash
void GameNode::updateHash(){

    unsigned char sym_type = 0;
    unsigned long long hash = this->board;
    unsigned long long temp;

    //check r3
    temp = this->r3Board(board);
    if (temp < hash) {
        sym_type = 1;
        hash = temp;
    }

    //check r2
    temp = this->r2Board(board);
    if (temp < hash) {
        sym_type = 2;
        hash = temp;
    }

    //check r
    temp = this->rBoard(board);
    if (temp < hash) {
        sym_type = 3;
        hash = temp;
    }

    //check f
    temp = this->fBoard(board);
    if (temp < hash) {
        sym_type = 4;
        hash = temp;
    }

    //check rf
    temp = this->rfBoard(board);
    if (temp < hash) {
        sym_type = 5;
        hash = temp;
    }

    //check r2f
    temp = this->r2fBoard(board);
    if (temp < hash) {
        sym_type = 6;
        hash = temp;
    }

    //check r3f
    temp = this->r3fBoard(board);
    if (temp < hash) {
        sym_type = 7;
        hash = temp;
    }

    this->hash = hash;
    this->sym_type = sym_type;
};

//returns whole board transformation
unsigned long long GameNode::rBoard(unsigned long long board){
    unsigned long long output = 0;
    for(unsigned char i = 0; i < GameNode::n * GameNode::n; i++){
        if ((board >> i) & 1) {
            output |= 1 << rMove(i);
        }
    }
    return output;
}
unsigned long long GameNode::r2Board(unsigned long long board){
    unsigned long long output = 0;
    for(unsigned char i = 0; i < GameNode::n * GameNode::n; i++){
        if ((board >> i) & 1) {
            output |= 1 << r2Move(i);
        }
    }
    return output;
}
unsigned long long GameNode::r3Board(unsigned long long board){
    unsigned long long output = 0;
    for(unsigned char i = 0; i < GameNode::n * GameNode::n; i++){
        if ((board >> i) & 1) {
            output |= 1 << r3Move(i);
        }
    }
    return output;
}
unsigned long long GameNode::fBoard(unsigned long long board){
    unsigned long long output = 0;
    for(unsigned char i = 0; i < GameNode::n * GameNode::n; i++){
        if ((board >> i) & 1) {
            output |= 1 << fMove(i);
        }
    }
    return output;
}
unsigned long long GameNode::rfBoard(unsigned long long board){
    unsigned long long output = 0;
    for(unsigned char i = 0; i < GameNode::n * GameNode::n; i++){
        if ((board >> i) & 1) {
            output |= 1 << rfMove(i);
        }
    }
    return output;
}
unsigned long long GameNode::r2fBoard(unsigned long long board){
    unsigned long long output = 0;
    for(unsigned char i = 0; i < GameNode::n * GameNode::n; i++){
        if ((board >> i) & 1) {
            output |= 1 << r2fMove(i);
        }
    }
    return output;
}
unsigned long long GameNode::r3fBoard(unsigned long long board){
    unsigned long long output = 0;
    for(unsigned char i = 0; i < GameNode::n * GameNode::n; i++){
        if ((board >> i) & 1) {
            output |= 1 << r3fMove(i);
        }
    }
    return output;
}

//returns transformation of single move
unsigned char GameNode::rMove(unsigned char move) {
    unsigned char n = GameNode::n;
    return n * (move % n + 1) - (move / n) - 1;
}
unsigned char GameNode::r2Move(unsigned char move) {
    unsigned char n = GameNode::n;
    return n * (GameNode:: n - (move / GameNode:: n)) - (move % n) - 1;
}
unsigned char GameNode::r3Move(unsigned char move) {
    unsigned char n = GameNode::n;
    return (n - (move % n) - 1) * n + (move / n);
}
unsigned char GameNode::fMove(unsigned char move) {
    unsigned char n = GameNode::n;
    return (move / n) * n + n - (move % n) - 1;
}
unsigned char GameNode::rfMove(unsigned char move) {
    unsigned char n = GameNode::n;
    return n * (n - (move % n)) - (move / n) - 1;
}
unsigned char GameNode::r2fMove(unsigned char move) {
    unsigned char n = GameNode::n;
    return n * (n - move / n - 1) + (move % n);
}
unsigned char GameNode::r3fMove(unsigned char move) {
    unsigned char n = GameNode::n;
    return n * (move % n) + (move / n);
}


//prunes possible moves to only valid moves
void GameNode::prune(unsigned char lastMove){
    unsigned char col = lastMove % GameNode::n;
    unsigned char moveAtRowStart = lastMove - col;
    unsigned char row = lastMove / GameNode::n;
    unsigned char colLM = columnLosingMove(col);
    unsigned char rowLM = rowLosingMove(moveAtRowStart);
    unsigned char rdLM = rightDiagLosingMove(row, col);
    unsigned char ldLM = leftDiagLosingMove(row, col);

    if(colLM != 255){
        this->possibleMoves.erase(colLM);
    }
    if(rowLM != 255){
        this->possibleMoves.erase(rowLM);
    }
    if(rdLM != 255){
        this->possibleMoves.erase(rdLM);
    }
    if(ldLM != 255){
        this->possibleMoves.erase(ldLM);
    }
}
unsigned char GameNode::columnLosingMove(unsigned char start){
    bool primed = false;
    unsigned char output = 255;
    unsigned char curMove;
    for(unsigned char i = 0; i < GameNode::n; i++){
        curMove = start + (i * GameNode::n);
        if(!((this->board >> curMove) & 1)){
            if(primed){
                return 255;
            }else{
                primed = true;
                output = curMove;
            }
        }
    }
    return output;
}
unsigned char GameNode::rowLosingMove(unsigned char start){
    bool primed = false;
    unsigned char output = 255;
    unsigned char curMove;
    for(unsigned char i = 0; i < GameNode::n; i++){
        curMove = start + i;
        if(!((this->board >> curMove) & 1)){
            if(primed){
                return 255;
            }else{
                primed = true;
                output = curMove;
            }
        }
    }
    return output;
}
unsigned char GameNode::rightDiagLosingMove(unsigned char movei, unsigned char movej){

    if(movei == movej) {
        bool primed = false;
        unsigned char output = 255;
        unsigned char curMove;
        for (unsigned char i = 0; i < GameNode::n; i++) {
            curMove = getMove(std::array<unsigned char, 2>{i, i});
            if (!((this->board >> curMove) & 1)) {
                if (primed) {
                    return 255;
                } else {
                    primed = true;
                    output = curMove;
                }
            }
        }
        return output;
    }else{
        return 255;
    }
}
unsigned char GameNode::leftDiagLosingMove(unsigned char movei, unsigned char movej){

    if(movei + movej + 1 == GameNode::n) {
        bool primed = false;
        unsigned char output = 255;
        unsigned char curMove;
        for (unsigned char i = 0; i < GameNode::n; i++) {
            curMove = getMove(std::array<unsigned char, 2>{i, (GameNode::n - 1 - i)});
            if (!((this->board >> curMove) & 1)) {
                if (primed) {
                    return 255;
                } else {
                    primed = true;
                    output = curMove;
                }
            }
        }
        return output;
    }else{
        return 255;
    }
}



/*****************************************
//Successor Logic
******************************************/

//returns the unique children of the node
std::vector<std::shared_ptr<GameNode> > GameNode::succ() const{
    std::unordered_set<unsigned long long> hashset;
    std::vector<std::shared_ptr<GameNode> >  children;
    children.reserve(GameNode::n * GameNode::n);

    //iterate through possible moves
    for(const auto& move : this->possibleMoves){
        //create node with move made
        std::shared_ptr<GameNode> child(new GameNode(*this));
        child->makeMove(move);

        //check to see if child shares the hash as another child (i.e., same node)
        std::unordered_set<unsigned long long>::const_iterator got = hashset.find(child->hash);
        if(got == hashset.end()){
            hashset.insert(child->hash);
            children.push_back(child);
        }
    }
    return children;
}



/*****************************************
//utility functions
******************************************/

//converts a move number into a (row, column) pair
std::array<unsigned char, 2> GameNode::getRC(unsigned char move){
    std::array<unsigned char, 2> RC = {(move / GameNode::n), (move % GameNode::n)};
    return RC;
}

//converts a (row,column) into a move number
unsigned char GameNode::getMove(std::array<unsigned char, 2> RC){
    return ((GameNode::n * RC[0]) + RC[1]);
}


//prints out information about the node
void GameNode::printInfo(){

    //gets number of moves made by counting the set bits
    unsigned long long value = this->hash;
    unsigned int count = 0;
    while (value > 0) {
        if ((value & 1) == 1)
            count++;
        value >>= 1;
    }

    std::cout << "Num Moves: " << count;
    std::cout << " Board: " << this->board;
    std::cout << " Hash: " << this->hash;
    std::cout << " Next Moves: ";
    for(auto& move : this->possibleMoves){
        std::cout << static_cast<unsigned>(move) << ", ";
    }
    std::cout << std::endl;
}