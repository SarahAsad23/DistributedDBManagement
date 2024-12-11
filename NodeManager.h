#pragma once

/*
* This file will contain the methods to read and write to the database 
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <map>

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

    // helper function for Read and Write to trim whitespace when doing comparison 
    string trim(const string& str) {
        size_t first = str.find_first_not_of(" \t\r\n");
        if (first == string::npos) return ""; // no non-whitespace characters
        size_t last = str.find_last_not_of(" \t\r\n");
        return str.substr(first, (last - first + 1));
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
    string Read(int node){
        string filename = getFileName(node); 
        ifstream inFile(filename); 

        if(!inFile){
            cerr << "error opening file"; 
            return ""; 
        }

        int totalRows = 0; 
        string line; 
        bool firstRowSkipped = false; 

        getline(inFile, line); 
        firstRowSkipped = true; 
   
        // get total number of rows 
        while(getline(inFile, line)){
            totalRows++; 
        }

        srand(time(0)); 
        int randomRow = rand() % totalRows + 1; 

        inFile.clear();
        inFile.seekg(0, ios::beg); 

        getline(inFile, line); 

        int currentRow = 0; 
        while(getline(inFile, line)){
            currentRow++;
            if(currentRow == randomRow){
                return line; 
            }
        }


        cerr << "Error: Row not Found"; 
        return "";
        
    }

    // writing to node/file 
    string Write(string ID, string column, string newValue, int node) {
        // Get the file name for the specified node
        string fileName = getFileName(node);

        if (fileName.empty()) {
            return "Error: Invalid node.";
        }

        // Open the file for reading
        ifstream inFile(fileName);
        if (!inFile) {
            return "Error: Unable to open file for reading.";
        }

        // Read all file data into memory to process changes
        vector<string> fileData;
        string line;

        // Read header first
        if (getline(inFile, line)) {
            fileData.push_back(line); // Add headers back to the fileData
        }

        vector<string> headers;
        stringstream headerStream(line);
        string header;

        // Parse the headers to find the column index
        while (getline(headerStream, header, ',')) {
            headers.push_back(trim(header));
        }

        
        // print headers
        for(int i = 0; i < headers.size(); i ++){
            cout << headers[i] << " "; 
        }
        

        // Find the column index
        int colIndex = -1;
        for (int i = 0; i < headers.size(); i++) {
            if (headers[i] == trim(column)) {
                colIndex = i;
                break;
            }
        }

        cout << "Column index found: " << colIndex << endl;

        if (colIndex == -1) {
            return "Error: Column not found.";
        }

        // Process rows to locate the record
        bool recordFound = false;
        string oldRow, newRow;
        while (getline(inFile, line)) {
            stringstream ss(line);
            vector<string> rowValues;
            string value;

            cout << "ID IS: " << ID << "\n";

            // Split the row into values based on commas
            while (getline(ss, value, ',')) {
                rowValues.push_back(value);
            }

            // Check if the value in the specified column matches the ID

            cout << "rowValues[colIndex]: " << trim(rowValues[colIndex]) << "\n";
            cout << "ID: " << trim(ID) << "\n"; 
            

            if (trim(rowValues[colIndex]) == trim(ID)) {
                oldRow = line; // Save the old row;  
                rowValues[colIndex] = newValue; // Update the value in the target column
                recordFound = true;

                // rebuild the row with the updated value
                newRow.clear(); // Clear any previous data
                for (int i = 0; i < rowValues.size(); i++) {
                    newRow += rowValues[i];
                    if (i != rowValues.size() - 1) {
                        newRow += ",";
                    }
                }

                //cout << "Updated row: " << newRow << endl; 
            }

            // Always add the row (updated or original) to the file data
            if (recordFound && oldRow == line) {
                fileData.push_back(newRow);  // Add updated row
            } else {
                fileData.push_back(line);  // Add original row
            }
        }

        inFile.close();

        if (!recordFound) {
            return "Error: Record with specified ID not found.";
        }

        // write updated data back to the file
        ofstream outFile(fileName);
        if (!outFile) {
            return "Error: Unable to open file for writing.";
        }

        // write all the data including header
        for (const string& dataLine : fileData) {
            outFile << dataLine << "\n";
        }

        outFile.close();

        cout << "OLD Row: " << oldRow << "\nNEW Row: " << newRow << endl;

        return "OLD Row: " + oldRow + "\nNEW Row: " + newRow;
    }

    //inserting a task into Task Table
    string InsertTask(int node){
        map<string, string> tasks; 
        tasks.insert({"code Review", "Review code for errors and performance"}); 
        tasks.insert({"bux Fixes", "identify and resolve issues in software"}); 
        tasks.insert({"Feature Development", "Design and implement features based on requirements"}); 
        tasks.insert({"System Design", "Planning and architecting the structure of the software system"}); 
        tasks.insert({"Unite Testing", "Writing and running tests to ensure individual components work correctly"}); 
        tasks.insert({"Performance Optimization", "Improving the speed and efficiency of the software."}); 
        tasks.insert({"API Integration", " Integrating third-party APIs into the software for extended functionality"}); 
        tasks.insert({"Database Management", "Designing, managing, and optimizing databases for performance."}); 
        tasks.insert({"DevOps Automation", "Automating the deployment and infrastructure management processes"}); 
        tasks.insert({"Security Auditing", "Analyzing and fixing vulnerabilities to ensure software security"}); 

        // Get the file name for the specified node
        string fileName = getFileName(node);

        if (fileName.empty()) {
            return "Error: Invalid node.";
        }

        // Open the file for appending
        ofstream outFile(fileName, ios::app); // Use ios::app to append to the file
        if (!outFile) {
            return "Error: Unable to open file for writing.";
        }
        srand(time(0)); 
        size_t mapSize = tasks.size(); 
        size_t randomIndex = rand() % mapSize; 
        auto it = tasks.begin(); 
        advance(it, randomIndex); 

        string lineToAdd = to_string(rand()) + ", " + to_string(rand()) + ", " + it->first + ", " + it->second;

        // Append the line to the file
        outFile << lineToAdd << endl;

        // Close the file
        outFile.close();

        return "Task added successfully: " + lineToAdd + "\n";

    }

    // deleting a row from a table 
    string Delete(int node){
    string fileName = getFileName(node); 
    ifstream inFile(fileName); 

    vector<string> rows;
    string line;
    while (getline(inFile, line)) {
        rows.push_back(line);
    }

    inFile.close();

    // select a random row
    srand(time(0)); 
    int randomIndex = rand() % rows.size(); // Random index (0-based)

    string deletedRow = rows[randomIndex];

    // remove the random row from the vector
    rows.erase(rows.begin() + randomIndex);

    // Write the updated rows back to the file
    std::ofstream outFile(fileName);
    if (!outFile) {
        std::cerr << "Error writing to file\n";
        return "";
    }

    for (const auto& row : rows) {
        outFile << row << "\n";
    }

    return "Deleted Row: " + deletedRow + "\n"; 

    outFile.close();


    cerr << "Error: Row not Found"; 
    return "";

}

    // inserting an employee into employee table 
    string InsertEmployee(int node){
    vector<string> firstNames = {"Sarah", "Chris", "James", "Olivia", "Daniel", "Sophia", "Ben", "Mia", "Alex", "Lily"}; 
    vector<string> lastNames = {"Brown", "Williams", "Miller", "Moore", "Clark", "Walker", "Lewis", "Hall", "Allen", "Young"}; 
    vector<string> roles = {"Manager", "Developer", "Engineer", "Software Engineer", "UI Developer"}; 
 
    string randFirstName = firstNames[rand() % 10]; 
    string randLastName = lastNames[rand() % 10]; 
    string randRole = roles[rand() % 5]; 

     // Get the file name for the specified node
    string fileName = getFileName(node);

    if (fileName.empty()) {
        return "Error: Invalid node.";
    }

    // Open the file for appending
    ofstream outFile(fileName, ios::app); // Use ios::app to append to the file
    if (!outFile) {
        return "Error: Unable to open file for writing.";
    }

    string lineToAdd = to_string(rand()) + ", " + randFirstName + ", " + randLastName + ", " + randRole + "\n"; 

    // Append the line to the file
    outFile << lineToAdd << endl;

    // Close the file
    outFile.close();

    return "Employee added successfully: " + lineToAdd + "\n";

    

}
};
