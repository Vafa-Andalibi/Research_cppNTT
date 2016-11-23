/***************************************
 * MoveTree
 * ------------------------------
 * Provides all of the persistence mechanisms
 * for the game states and locks.
 ***************************************/


#include <iostream>
#include <queue>
#include "MoveTree.h"


/***************************************
 * Constructor
 ***************************************/

MoveTree::MoveTree(char n, char* dir){
    this->dir = dir;
    this->open(n);
}


/***************************************
 * Database Utilities
 ***************************************/

//opens the lock and resolution databases
void MoveTree::open(char n){

    //pull in the relevant directories
    std::string stringDir(this->dir);
    std::string dataDir = stringDir + "data/";
    std::string locksDir = stringDir + "locks/";
    std::string dbName = std::to_string(n) + ".db";
    std::string locksName = std::to_string(n) + ".ldb";

    this->resEnv = new DbEnv(0);
    resEnv->set_cachesize(3,0,1);
    resEnv->add_data_dir(dataDir.c_str());
    resEnv->open(stringDir.c_str(), DB_INIT_LOCK | DB_INIT_MPOOL | DB_CREATE, 0);

    //todo do BTree calcs
    this->resolved = new Db(resEnv, 0);
    resolved->set_pagesize(512*8);

    resolved->set_create_dir(dataDir.c_str());
    //resolved->remove("test.db",NULL,0);
    resolved->open(NULL,
                   dbName.c_str(),
                   NULL,
                   DB_BTREE,
                   DB_CREATE,
                   0);


    this->lockEnv = new DbEnv(0);
    lockEnv->set_cachesize(0,std::pow(2,27),0);
    lockEnv->add_data_dir(locksDir.c_str());
    lockEnv->open(stringDir.c_str(), DB_INIT_LOCK | DB_INIT_MPOOL | DB_CREATE, 0);


    //todo do BTree calcs
    this->locks = new Db(lockEnv, 0);
    locks->set_pagesize(512);

    locks->set_create_dir(locksDir.c_str());
    //locks->remove(locksName.c_str(), NULL, 0);
    locks->open(NULL,
                   locksName.c_str(),
                   NULL,
                   DB_BTREE,
                   DB_CREATE,
                   0);


}

//closes the lock and resolution databases
void MoveTree::close(){
    this->resolved->close(0);
    this->resEnv->close(0);
    delete this->resolved;
    delete this->resEnv;

    this->locks->close(0);
    this->lockEnv->close(0);
    delete this->locks;
    delete this->lockEnv;
}


/***************************************
 * Locking Mechanisms
 ***************************************/

//adds lock to database
void MoveTree::lock(GameNode &board){
    //initialize lock
    Dbt key(&board.hash, sizeof(unsigned long long));
    char temp = 1;
    Dbt value(&temp, sizeof(char));

    //store lock
    this->locks->put(NULL, &key, &value, 0);
}

//removes lock from database
void MoveTree::unlock(GameNode &board){
    //initialize key
    Dbt key(&board.hash, sizeof(unsigned long long));

    //remove lock
    this->locks->del(NULL, &key, 0);
}

//checks to see if the board is locked
bool MoveTree::isLocked(GameNode &board){
    //initialize key
    Dbt key(&board.hash, sizeof(unsigned long long));

    //check lock's existence
    return DB_KEYEMPTY == this->locks->exists(NULL, &key, 0);
}


/***************************************
 * Resolution Mechanisms
 ***************************************/

//saves resolution of game state
void MoveTree::put(GameNode &board, char i){
    //initialize key and value
    Dbt key(&board.hash, sizeof(unsigned long long));
    Dbt value(&i, sizeof(char));

    //store in database
    this->resolved->put(NULL, &key, &value, 0);
}

//gets resolution of game state or returns -1 if not yet resolved
char MoveTree::get(GameNode &board){

    //initialize hash
    Dbt key(&board.hash, sizeof(unsigned long long));

    //initialize return value
    char returnVal;
    Dbt value;
    value.set_data(&returnVal);
    value.set_ulen(sizeof(char));
    value.set_flags(DB_DBT_USERMEM);

    //get value
    if(this->resolved->get(NULL, &key, &value, 0) == 0){
        return returnVal;
    }else{
        //return -1 if not found
        return -1;
    }

}


/***************************************
 * Start Heuristic
 ***************************************/

//returns the most appropriate place to begin searching the game tree
std::shared_ptr<GameNode> MoveTree::getStart(GameNode &node){
    if(isLocked(node)){
        std::vector<std::shared_ptr<GameNode> > children = node.succ();

        //load inital q
        std::queue<std::shared_ptr<GameNode> > q;
        for(std::vector<std::shared_ptr<GameNode> >::iterator child = children.begin(); child != children.end(); ++child){
            q.push(child.operator*());
        }

        while(!q.empty()) {
            for (std::vector<std::shared_ptr<GameNode> >::iterator child = children.begin();
                 child != children.end(); ++child) {
                if (get(**child) == -1) {
                    if (!isLocked(**child)) {
                        return child.operator*();
                    } else {

                        q.push(child.operator*());
                    }
                }
            }

            //get the next non-resolved element
            std::shared_ptr<GameNode> next;
            do {
                next = q.front();
                q.pop();
                if(get(*next) != -1){
                    children = (*next).succ();
                    break;
                }
            }while(!q.empty());
        }

        return nullptr;
    }else{
        return std::make_shared<GameNode>(node);
    }
}




