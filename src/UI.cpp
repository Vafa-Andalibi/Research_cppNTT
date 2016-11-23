//
// Created by jclangst on 11/22/16.
//

#include "UI.h"


const std::string RESET = "\u001B[0m";
const std::string GREEN = "\u001B[32m";
const std::string RED = "\u001B[31m";

//todo figure out why this is not returning the correct number
unsigned char UI::getN(){
    std::string input = "";
    unsigned char n;
    std::cout << "What size of n x n gameboard would you like to analyze?" << std::endl;
    std::getline(std::cin, input);

    std::stringstream myStream(input);
    myStream >> n;
    return n - 48;
}


//color codes


//explores the full tree
void UI::explore(std::shared_ptr<GameNode> board, MoveTree tree){

    //trackers
    std::vector<std::shared_ptr<GameNode>> path;
    int player = 1;
    int turn = 1;

    //state variables
    int choice;
    std::vector<char> choices;


    //print initial commands
    std::cout <<"\nCOMMANDS"<< std::endl;
    std::cout <<"n  --> make the move n indicated on the gameboard"<< std::endl;
    std::cout <<"bn --> go backwards n moves"<< std::endl;
    std::cout <<"q  --> quit"<< std::endl;
    std::cout <<"\nGAMEBOARD KEY"<< std::endl;
    std::cout <<GREEN + "#" + RESET + " indicates a winning move"<< std::endl;
    std::cout <<RED + "#" + RESET + " indicates a losing move"<< std::endl;
    std::cout <<"X indicates a previous move"<< std::endl;
    std::cout <<"- indicates an invalid move (immediate loss)"<< std::endl;

    std::cout << "\n(enter) to continue"<< std::endl;
    std::cin.ignore();

    while(true){

        //record path
        path.push_back(board);

        //headers
        std::cout << "\n********************************\n"<< std::endl;
        std::cout << "Player " << player<< std::endl;
        std::cout << "Turn: " << turn << std::endl;
        std::cout << "\n" << std::endl;

        //get potential choices
        choices = printChoices(tree, board);
        if(choices.size() == 0){
            std::cout <<  "\nPLAYER " << player << " HAS NO MOVES! LOSES!\n"<< std::endl;
        }

        choice = getExplorationChoice(turn, choices.size());

        //quit
        if(choice == 0){
            break;
        }
            //make move
        else if (choice > 0){
            board = std::shared_ptr<GameNode>(new GameNode(*board));
            board->makeMove(choices.at(choice - 1));
            player = std::abs(player - 2) + 1;
            turn++;
        }
            //go backwards in the game path
        else{

            std::vector<std::shared_ptr<GameNode>>::iterator removal = path.end();
            removal--;
            for(int i = 0; i < -choice; i++){
                removal = path.erase(removal);
                removal--;
            }


            board = path.at(path.size() - 1);
            turn += choice;
            if(-choice % 2 == 1){
                player = std::abs(player - 2) + 1;
            }
        }
    }
}



//TODO need to validate
//gets and validates the user command
int UI::getExplorationChoice(int turn, int numMoves){
    int output;
    std::string input;

    while(true){
        std::cout << "Command: " << std::endl;

        std::getline(std::cin, input);
        std::stringstream myStream(input);
        myStream >> output;

        if(output <= numMoves && output > 0){
            break;
        }else{
            std::cout <<"NOT A VALID MOVE!" << std::endl;
            continue;
        }
    }
    return output;
}




//prints the potential choices given the current board and a list of winning children
//returns a mapping of the choices to the game moves
std::vector<char> UI::printChoices(MoveTree moves, std::shared_ptr<GameNode> board){

    std::string output = "";
    std::string row;
    char n = GameNode::n;
    int counter = 1;
    unsigned char move;



    //movemappings
    std::vector<char> moveMapping;

    //print logic
    for(unsigned char i = 0; i < n; i++){
        row = "";
        for(unsigned char j = 0; j < n; j++){
            move = static_cast<unsigned char>(GameNode::n * i + j);
            if (!((board->board >> move) & 1)){
                if(board->possibleMoves.find(move) != board->possibleMoves.end()){
                    std::shared_ptr<GameNode> child(new GameNode(*board));
                    child->makeMove(move);
                    char status = moves.get(*child);
                    if(status == 1){
                        row += GREEN + " " + std::to_string(counter) + RESET;
                        if(counter > 9){
                            row += " ";
                        }else{
                            row += "  ";
                        }
                        moveMapping.push_back(move);
                        counter++;
                    }else if(status == 0) {
                        row += RED + " " + std::to_string(counter) + RESET;
                        if (counter > 9) {
                            row += " ";
                        } else {
                            row += "  ";
                        }
                        moveMapping.push_back(move);
                        counter++;
                    }
                }else{
                    row += " -  ";
                }
                
            }else{
                row += " X  ";
            }
        }

        output += row + "\n";
    }

    std::cout <<"ENDED?" << std::endl;
    std::cout << output << std::endl;
    return moveMapping;
}
    