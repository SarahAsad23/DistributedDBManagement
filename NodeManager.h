/*
* This file will contain the methods to read and write to the database 
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio>

using namespace std;

class NodeManager{

private: 
    // get the filename we want to operate on 
    // removes redundancy from all the below methods 
    string getFileName(int node){
        switch (node){
        case 1: return "Node1/Projects.csv";
        case 2: return "Node2/Tasks.csv";
        case 3: return "Node3/Employees.csv";        
        default: 
            cerr << "Invalid FIle Access";
            return "";
        }
    }

public: 
    // This will help us find which row we want to operate on (read/write) 
    int NodeManager::GetRecord(int ID, int node){
        // first get the file we are operating on 
        string fileName = getFileName(node); 
        
        //if filename is empty then error
        if(fileName.empty()){
            return -1; 
        }

        // if file does not open the also error 
        ifstream inFile(fileName);
        if (!inFile){
            cerr << "Error Opening File"; 
            return -1; 
        }

        // then we need to get the ID of the record 
        



    }

    // reading from a node/file
    string NodeManager::Read(){
    
    }

    // writitng to a node/file 
    void NodeManager::Write(int mode, int node, int ID){
        // adding a new entry into file 
        if (mode == 1){

        }
        //rewriting a file (editing existing entry)
        else{

        }

    }
};
