//
// Created by jclangst on 11/21/16.
//

#ifndef CPPNTT_MINIMAX_H
#define CPPNTT_MINIMAX_H


#include "MoveTree.h"

class MiniMax {



public:

    MiniMax(MoveTree* tree);
    static MoveTree* tree;

    void initiate(const GameNode &objective);

    bool runThread(std::shared_ptr<GameNode> node);



};






#endif //CPPNTT_MINIMAX_H
