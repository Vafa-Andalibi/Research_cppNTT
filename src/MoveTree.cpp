/***************************************
 * MoveTree
 * ------------------------------
 * Provides all of the persistence mechanisms
 * for the game states and locks.
 ***************************************/


#include <iostream>
#include <queue>
#include "MoveTree.h"
#include "boost/filesystem/operations.hpp"

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

    int err;

    //pull in the relevant directories
    std::string stringDir(this->dir);
    std::string stringDataDir = stringDir + "data/";
    std::string stringEnvDir = stringDir + "env/";
    std::string dbName = std::to_string(n) + ".db";

    //creates the data directory
    boost::filesystem::path dataDir = boost::filesystem::absolute(boost::filesystem::path(stringDataDir.c_str()));
    if(!(boost::filesystem::exists(dataDir))){
        std::cout<<"Data Directory Doesn't Exists"<<std::endl;

        if (boost::filesystem::create_directories(dataDir))
            std::cout << "....Successfully Created !" << std::endl;
    }

    //creates the env directory
    boost::filesystem::path envDir = boost::filesystem::absolute(boost::filesystem::path(stringEnvDir.c_str()));
    if(!(boost::filesystem::exists(envDir))){
        std::cout<<"Environment Directory Doesn't Exists"<<std::endl;

        if (boost::filesystem::create_directories(envDir))
            std::cout << "....Successfully Created !" << std::endl;
    }

    this->resEnv = new DbEnv(0);
    resEnv->set_cachesize(3,0,1);
    resEnv->add_data_dir(dataDir.c_str());
    resEnv->open(envDir.c_str(), DB_INIT_LOCK | DB_INIT_MPOOL | DB_THREAD| DB_CREATE, 0);

    std::cout << "Opening DB" << std::endl;
    //todo do BTree calcs
    this->resolved = new Db(resEnv, 0);
    resolved->set_pagesize(512*8);

    resolved->set_create_dir(dataDir.c_str());

    resolved->open(NULL,
                   dbName.c_str(),
                   NULL,
                   DB_BTREE,
                   (DB_CREATE | DB_THREAD),
                   0);
    //err = resolved->set_flags(DB_THREAD | DB_MULTIVERSION);

    //std::cout << err << std::endl;
}

//closes the lock and resolution databases
void MoveTree::close(){
    this->resolved->close(0);
    this->resEnv->close(0);
    delete this->resolved;
    delete this->resEnv;
}

void MoveTree::removeDb(char n, char* dir){
    //pull in the relevant directories
    std::string stringDir(dir);
    std::string stringDataDir = stringDir + "data/";
    std::string dbName = std::to_string(n) + ".db";
    std::string fullPath = stringDataDir + dbName;
    boost::filesystem::path removeDir = boost::filesystem::absolute(boost::filesystem::path(fullPath.c_str()));
    if(boost::filesystem::exists(removeDir)) boost::filesystem::remove(removeDir);

}


/***************************************
 * Resolution Mechanisms
 ***************************************/

//saves resolution of game state
void MoveTree::put(std::shared_ptr<GameNode> board, char i){
    //initialize key and value
    unsigned long long hash = board->hash;
    Dbt key;
   // memset(&key, 0, sizeof(key));
    key.set_data(&hash);
    key.set_size(sizeof(unsigned long long));

    //initialize return value
    Dbt value;
   // memset(&value, 0, sizeof(value));
    value.set_data(&i);
    value.set_size(sizeof(char));
    value.set_flags(DB_DBT_REALLOC);

    //store in database
    this->resolved->put(NULL, &key, &value, 0);
}

//gets resolution of game state or returns -1 if not yet resolved
char MoveTree::get(std::shared_ptr<GameNode> board){

    //initialize hash
    unsigned long long hash = board->hash;
    Dbt key;
   // memset(&key, 0, sizeof(key));
    key.set_data(&hash);
    key.set_size(sizeof(unsigned long long));

    //initialize return value
    char returnVal;
    Dbt value;
   // memset(&value, 0, sizeof(value));
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





