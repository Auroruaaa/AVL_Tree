#include <string>
#include <iostream>
#include <fstream>

#include "json.hpp"
#include "AVL.h"


int main(int argc, char** argv) {

    // check the accuracy of the input
    if (argc != 2){ // if wrong input
        std::cerr << "USAGE: AVLCommands.cxx TestCase.json" << std::endl;   // print the error
        exit(0);    // then exit the program
    }
    nlohmann::json jsonObject;  // json variable to store the json file
    std::string file_name = argv[1];    // get the input filename
    std::ifstream json_file(file_name); // read the json file
    if(json_file.is_open()){    // store the json file into jsonObject
        json_file >> jsonObject;
    }
    jsonObject.erase("metadata");   // pop information in the key "metedata"
    AVL AVLObject;  // assign an AVL tree
    for (auto itr = jsonObject.begin(); itr != jsonObject.end(); ++itr){    //read through the left information in the json file
        int key;    //memorize the key
        if((*itr)["key"] != nullptr){   // if it is not empty
            key = (*itr)["key"];    // set key
            AVLObject.Insert(key);  // then read the data, and insert them into the AVL tree
        }
    }
    std::cout << AVLObject.JSON() << std::endl; // output the result
}