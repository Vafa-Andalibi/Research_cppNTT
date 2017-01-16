//
// Created by jclangst on 11/22/16.
//

#ifndef CPPNTT_UI_H
#define CPPNTT_UI_H


#include <iostream>
#include <sstream>
#include "GameNode.h"
#include "MoveTree.h"

class UI {

public:
    unsigned char getN();
    int getExplorationChoice(int turn, int numMoves);
    void explore(std::shared_ptr<GameNode> board, MoveTree tree);
    std::vector<char> printChoices(MoveTree moves, std::shared_ptr<GameNode> board);
};


#endif //CPPNTT_UI_H
