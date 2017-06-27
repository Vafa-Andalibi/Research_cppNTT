/***************************************
 * MoveTree
 * ------------------------------
 * Provides all of the persistence mechanisms
 * for the game states and locks.
 ***************************************/
#include <limits.h> /* PATH_MAX */
#include <sys/stat.h>
#include <iostream>
#include <queue>
#include "../include/MoveTree.h"
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <linux/limits.h>

//#include "boost/filesystem/operations.hpp"

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
    char resolved_path[PATH_MAX];
    realpath(this->dir, resolved_path);
    std::string stringDir(resolved_path);
//    std::cout << "hellllooooo" + stringDir << std::endl;
    //pull in the relevant directories
//    std::string stringDir(this->dir);
    std::string stringDataDir = stringDir + "/data/";
    std::string stringEnvDir = stringDir + "/env/";
    std::string dbName = std::to_string(n) + ".db";

    //creates the data directory
  /*  boost::filesystem::path dataDir = boost::filesystem::absolute(boost::filesystem::path(stringDataDir.c_str()));
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
    }*/
//    std::cout << "folder path to create: " + stringDir << std::endl;
    if (mkdir(stringDir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
    {
        if( errno == EEXIST ) {
            // alredy exists
    //        std::cout << "Data folder already exists" << std::endl;
        } else {
            // something else
            std::cout << "cannot create Data folder, sessionnamefolder error:" << std::strerror(errno) << std::endl;
            throw std::runtime_error( std::strerror(errno) );
        }
    }

//    std::cout << "folder path to create: " + stringDataDir << std::endl;
    if (mkdir(stringDataDir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
    {
        if( errno == EEXIST ) {
            // alredy exists
  //          std::cout << "Data folder already exists" << std::endl;
        } else {
            // something else
            std::cout << "cannot create Data folder, sessionnamefolder error:" << std::strerror(errno) << std::endl;
            throw std::runtime_error( std::strerror(errno) );
        }
    }

    if (mkdir(stringEnvDir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
    {
        if( errno == EEXIST ) {
            // alredy exists
      //      std::cout << "Env folder already exists" << std::endl;
        } else {
            // something else
            std::cout << "cannot create Env folder, sessionnamefolder error:" << std::strerror(errno) << std::endl;
            throw std::runtime_error( std::strerror(errno) );
        }
    }



 //           std::cout << "V1) going to create DbEnv " << std::strerror(errno) << std::endl;
this->resEnv = new DbEnv(0u);
    resEnv->set_cachesize(4,0,1);
//    resEnv->add_data_dir(dataDir.c_str());
    resEnv->set_data_dir(stringDataDir.c_str());
//    resEnv->open(envDir.c_str(), DB_INIT_LOCK | DB_INIT_MPOOL | DB_THREAD| DB_CREATE, 0);
 //   std::cout << stringEnvDir.c_str() << std::endl;
    resEnv->open(stringEnvDir.c_str(), DB_INIT_LOCK | DB_INIT_MPOOL | DB_THREAD| DB_CREATE, 0);

    std::cout << "Opening DB" << std::endl;
    //todo do BTree calcs
    this->resolved = new Db(resEnv, 0);
//    std::cout << "V3) after new Db" << std::endl;
    resolved->set_pagesize(512*16);

   // std::cout << "V4) page size set, going to open db" << std::endl;
//    resolved->set_create_dir(dataDir.c_str());
//    resolved->set_create_dir(stringDataDir.c_str());

    resolved->open(NULL,
                   dbName.c_str(),
                   NULL,
                   DB_BTREE,
                   (DB_CREATE | DB_THREAD),
                   0);

    //std::cout << " db openned " << std::endl;
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
//    boost::filesystem::path removeDir = boost::filesystem::absolute(boost::filesystem::path(fullPath.c_str()));
//    if(boost::filesystem::exists(removeDir)) boost::filesystem::remove(removeDir);
    std::string remove_str = "rm -r " + stringDataDir;
    std::cout << "Folder removed: " + stringDataDir << std::endl;
    system(remove_str.c_str());

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
    this->resolved->put(NULL, &key, &value, DB_NOOVERWRITE);
}

//gets resolution of game state or returns -1 if not yet resolved
char MoveTree::get(std::shared_ptr<GameNode> board){

    //initialize hash

  //  std::cout << "M1) Initialize hash "  << std::endl;
    unsigned long long hash = board->hash;
  //  std::cout << "M2) hash built "  << std::endl;
    Dbt key;
   // std::cout << "M3) Key created, setting data"  << std::endl;
   // memset(&key, 0, sizeof(key));
    key.set_data(&hash);
   // std::cout << "M4) data set "  << std::endl;
    key.set_size(sizeof(unsigned long long));

    //initialize return value
    char returnVal;
    Dbt value;
  //  std::cout << "M5) Value created"  << std::endl;
    key.set_size(sizeof(unsigned long long));
   // memset(&value, 0, sizeof(value));
   // std::cout << "M6) Value set"  << std::endl;
    value.set_data(&returnVal);
    //std::cout << "M7) retValue set"  << std::endl;
    value.set_ulen(sizeof(char));
    value.set_flags(DB_DBT_USERMEM);

  //  std::cout << "M8) flag set"  << std::endl;
    //get value
    if(this->resolved->get(NULL, &key, &value, 0) == 0){
        
  //  	std::cout << "M9) value found"  << std::endl;
	return returnVal;
    }else{
//    	std::cout << "M9) value NOT found"  << std::endl;
        //return -1 if not found
        return -1;
    }

}





