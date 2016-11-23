//
// Created by jclangst on 11/21/16.
//

#include "MiniMax.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <omp.h>


MiniMax::MiniMax(std::shared_ptr<MoveTree> tree){
    this->tree = tree;
    std::cout << "Made it" << std::endl;
}


void MiniMax::initiate(GameNode &node, int process, int threads){
    std::cout << process << ": Entering" << std::endl;
    std::shared_ptr<GameNode> objective(new GameNode(node));

    omp_set_num_threads(threads);
    #pragma omp parallel
    {
        std::cout << process << ": Checking Tree" << std::endl;
       #pragma omp for schedule(dynamic)
       for(int i = 0; i < 50000; i++) {

            if(MiniMax::tree->get(objective) == -1) {

                std::cout << process << ": Starting" << std::endl;
                runThread(objective);
                std::cout << process << ": Finishing" << std::endl;

            }else{
                #pragma omp cancel for
            }
            #pragma omp cancellation point for
        }
    }

    std::cout << "Finished " << static_cast<unsigned>(MiniMax::tree->get(objective)) << std::endl;
}




bool MiniMax::runThread(std::shared_ptr<GameNode> node){

    //node->printInfo();
    char dp = MiniMax::tree->get(node);
    if(dp != -1){
        //std::cout << "RETURN DP " << (dp == 1) << " FOR " << std::endl;
        //node->printInfo();
        return dp == 1;
    }


    char status = 1;
    std::vector<std::shared_ptr<GameNode> > children = (*node).succ();
    std::random_shuffle(children.begin(), children.end());
    std::vector<std::shared_ptr<GameNode> >::iterator child = children.begin();
    while(child != children.end()){
        if (runThread(*child)) {
            status = 0;
        }
        child++;
    }

    //std::cout << "RETURN " << (status == 1) << " FOR: " << std::endl;
    if(status == 1){

        MiniMax::tree->put(node, 1);
    }else{
        MiniMax::tree->put(node, 0);
    }

    //node->printInfo();
    return status == 1;

}