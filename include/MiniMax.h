//
// Created by jclangst on 11/21/16.
//

#ifndef CPPNTT_MINIMAX_H
#define CPPNTT_MINIMAX_H


#include "MoveTree.h"

class MiniMax {



public:

    MiniMax(std::shared_ptr<MoveTree> tree);
    std::shared_ptr<MoveTree> tree;

    void initiate(GameNode &objective, int process, int threads);

    bool runThread(std::shared_ptr<GameNode> node);



};






#endif //CPPNTT_MINIMAX_H
