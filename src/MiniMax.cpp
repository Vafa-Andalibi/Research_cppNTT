//
// Created by jclangst on 11/21/16.
//

#include "MiniMax.h"
#include <vector>
#include <iostream>

MoveTree* MiniMax::tree = nullptr;

MiniMax::MiniMax(MoveTree* tree){
    MiniMax::tree = tree;
    std::cout << "Made it" << std::endl;
}


void MiniMax::initiate(const GameNode &objective){

    #pragma omp parallel num_threads(8)
    {
        #pragma omp for schedule(dynamic)
        for(int i = 0; i < 50000; i++) {
            if(MiniMax::tree->get(objective) == -1) {
                std::shared_ptr<GameNode> start;
                #pragma omp critical(start)
                {
                    start = MiniMax::tree->getStart(objective);
                    if(start) MiniMax::tree->lock(*start);

                }

                if(start) {
                    std::cout << "Starting!" << std::endl;
                    start->printInfo();
                    runThread(start);
                    std::cout << "Done with thread!" << std::endl;
                }

            }else{
                #pragma omp cancel for
            }
            #pragma omp cancellation point for
        }
    }

    std::cout << "Finished" << std::endl;
}




bool MiniMax::runThread(std::shared_ptr<GameNode> node){

    char dp = MiniMax::tree->get(*node);
    if(dp != -1){
        MiniMax::tree->unlock(*node);
        return dp == 1;
    }


    char status = 0;
    std::vector<std::shared_ptr<GameNode> > children = (*node).succ();
    while(!children.empty()) {
        std::vector<std::shared_ptr<GameNode> >::iterator child = children.begin();
        while(child != children.end()){
            if (!MiniMax::tree->isLocked(**child)) {
                MiniMax::tree->lock(*node);
                if (!runThread(*child)) {
                    status = 1;
                }
                child = children.erase(child);
            }else{
                child++;
            }
        }
    }

    if(status == 1){
        MiniMax::tree->put(*node, 1);
    }else{
        MiniMax::tree->put(*node, 0);
    }

    MiniMax::tree->unlock(*node);
    return status == 1;

}