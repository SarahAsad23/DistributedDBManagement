/*
* This file will contain the methods to read and write to the database 
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio>
#include <vector>

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
    int GetRecord(int ID, int node, string column){
        // first get the file we are operating on 
        string fileName = getFileName(node); 
        
        //vif filename is empty then error
        if(fileName.empty()){
            cerr << "Invalid node"; 
            return -1; 
        }

        // open the file for reading 
        ifstream inFile(fileName);

        // if file does not open then also error
        if (!inFile){
            cerr << "Error Opening File"; 
            return -1; 
        }
 
        string line; // string to hold each line of the file 
        vector<string> headers; // to hold column names (headers)

        // we want to read the headers first to know which ID we 
        // are looking for cause there are multiple ID's 
        if(getline(inFile, line)){
            // create a stringstream to process the line 
            stringstream ss(line); 
            string header; 

            // split the header line by the commas and 
            // store the column names 
            while (getline(ss, header, ',')){
                headers.push_back(header); 
            }
        }

        // then we need to find the index of the passed in column 
        int colIndex = -1; 
        for(int i = 0; i < headers.size(); i++){
            if(headers[i] == column){
                colIndex = i; 
                break;
            }
        }

        // now we need to search for the row with the specified column ID
        int rowNum = 2; 

        while(getline(inFile, line)){
            stringstream ss(line); 
            vector<string> rowValues; 
            string value; 

            while(getline(ss, value, ',')){
                rowValues.push_back(value);
            }

            if(colIndex < rowValues.size() && stoi(rowValues[colIndex]) == ID){
                cout << "Row number Found:" << rowNum; 
                return rowNum;
            }
            
            rowNum++;
        }

        cerr << "row with that ID NOT FOUND";
        return -1; 


    }

    // reading from a node/file
    string Read(int ID, int node, string column){
        int row = GetRecord(ID, node, column);

        if(row == -1){
            cerr << "Error finding record"; 
            return ""; 
        }

        string filename = getFileName(node); 
        ifstream inFile(filename); 

        if(!inFile){
            cerr << "error opening file"; 
            return ""; 
        }

        string line; 
        int currentRow = 0; 

        while(getline(inFile, line)){
            currentRow++; 
            if(currentRow == row){
                cout << line; 
                return line; 
            }
        }

        cerr << "Error: Row not Found"; 
        return "";
        
    }

    // writitng to a node/file 
    void Write(int mode, int node, int ID){
        // adding a new entry into file 
        if (mode == 1){

        }
        //rewriting a file (editing existing entry)
        else{

        }

    }
};
