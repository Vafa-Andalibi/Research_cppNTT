//
// Created by jclangst on 9/3/2016.
//
#include <iostream>
#include "GameNode.h"
#include <array>

unsigned char GameNode::n = 0;

GameNode::GameNode(unsigned char n){
    GameNode::n = n;
    lastMove = 255;
    possibleMoves = std::unordered_set<int>();

    for(int i = 0; i < (n * n); i++){
        possibleMoves.insert(i);
    }
}

//move whole board
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

//move single digit
unsigned char GameNode::rMove(unsigned char move) {
    std::array< unsigned char, 2> RC = getRC(move);
    char r = RC[0];
    RC[0] = RC[1];
    RC[1] = GameNode::n - r - (char)1;
    return getMove(RC);
}
unsigned char GameNode::r2Move(unsigned char move) {
    std::array< unsigned char, 2> RC = getRC(move);
    RC[0] = n - RC[0] - (char)1;
    RC[1] = GameNode::n - RC[1] - (char)1;
    return getMove(RC);
}
unsigned char GameNode::r3Move(unsigned char move) {
    std::array< unsigned char, 2> RC = getRC(move);
    char r = RC[0];
    RC[0] = GameNode::n - RC[1] - char(1);
    RC[1] = r;
    return getMove(RC);
}
unsigned char GameNode::fMove(unsigned char move) {
    std::array< unsigned char, 2> RC = getRC(move);
    RC[1] = GameNode::n - RC[1] - (char)1;
    return getMove(RC);
}
unsigned char GameNode::rfMove(unsigned char move) {
    std::array< unsigned char, 2> RC = getRC(move);
    char r = RC[0];
    RC[0] = GameNode::n - RC[1] - char(1);
    RC[1] = GameNode::n - r - char(1);
    return getMove(RC);
}
unsigned char GameNode::r2fMove(unsigned char move) {
    std::array< unsigned char, 2> RC = getRC(move);
    RC[0] = GameNode::n - RC[0] - (char)1;
    return getMove(RC);
}
unsigned char GameNode::r3fMove(unsigned char move) {
    std::array< unsigned char, 2> RC = getRC(move);
    char r = RC[0];
    RC[0] = RC[1];
    RC[1] = r;
    return getMove(RC);
}

//utility functions
std::array<unsigned char, 2> GameNode::getRC(unsigned char move){
    std::array<unsigned char, 2> RC = {move / GameNode::n, move % GameNode::n};
    return RC;
}
unsigned char GameNode::getMove(std::array<unsigned char, 2> RC){
    return ((GameNode::n * RC[0]) + RC[1]);
}

//make move logic
void GameNode::makeMove(unsigned char move){
    this->board |= 1 << move;
    this->lastMove = move;
    this->possibleMoves.erase(move);
    propagate();
}
void GameNode::propagate(){
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
        if(!((this->board >> i) & 1)){
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
        if(!((this->board >> i) & 1)){
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
            if (!((this->board >> i) & 1)) {
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
            curMove = getMove(std::array<unsigned char, 2>{i, GameNode::n - (unsigned char)1 - i});
            if (!((this->board >> i) & 1)) {
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