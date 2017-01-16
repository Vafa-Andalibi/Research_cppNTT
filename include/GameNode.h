//
// Created by jclangst on 9/3/2016.
//

#include <unordered_set>
#include <memory>
#include <vector>

#ifndef CPPNTT_GAMENODE_H
#define CPPNTT_GAMENODE_H


class GameNode {

    public:
    static unsigned char n;
    std::unordered_set<unsigned char> possibleMoves;
    unsigned long long board;
    unsigned long long hash;
    char sym_type;

    //constructors
    GameNode(unsigned char n);
    GameNode(const GameNode &node);


    void updateHash();
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


    void printInfo();

    std::vector<std::shared_ptr<GameNode> > succ() const;

    void makeMove(unsigned char move);
    void prune(unsigned char lastMove);
    unsigned char columnLosingMove(unsigned char start);
    unsigned char rowLosingMove(unsigned char start);
    unsigned char rightDiagLosingMove(unsigned char mover, unsigned char movec);
    unsigned char leftDiagLosingMove(unsigned char mover, unsigned char movec);
};

#endif //CPPNTT_GAMENODE_H
