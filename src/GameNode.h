//
// Created by jclangst on 9/3/2016.
//

#include <unordered_set>
#include <bitset>

#ifndef CPPNTT_GAMENODE_H
#define CPPNTT_GAMENODE_H


class GameNode {

    public:
        static unsigned char n;
        std::unordered_set<int> possibleMoves;
        unsigned char lastMove;
        unsigned long long board;

        //constructors
        GameNode(unsigned char n);
        GameNode(const GameNode&);

        unsigned char rMove(unsigned char move);
        unsigned char r2Move(unsigned char move);
        unsigned char r3Move(unsigned char move);
        unsigned char fMove(unsigned char move);
        unsigned char rfMove(unsigned char move);
        unsigned char r2fMove(unsigned char move);
        unsigned char r3fMove(unsigned char move);

        unsigned long long rBoard(unsigned long long board);
    unsigned long long r2Board(unsigned long long board);
    unsigned long long r3Board(unsigned long long board);
    unsigned long long fBoard(unsigned long long board);
    unsigned long long rfBoard(unsigned long long board);
    unsigned long long r2fBoard(unsigned long long board);
    unsigned long long r3fBoard(unsigned long long board);

        std::array< unsigned char, 2> getRC(unsigned char move);
       unsigned char getMove(std::array<unsigned char, 2>);

    void makeMove(unsigned char move);
    void propagate();
    unsigned char columnLosingMove(unsigned char start);
    unsigned char rowLosingMove(unsigned char start);
    unsigned char rightDiagLosingMove(unsigned char mover, unsigned char movec);
    unsigned char leftDiagLosingMove(unsigned char mover, unsigned char movec);
};

#endif //CPPNTT_GAMENODE_H
