#pragma once

#include <vector>
#include <string>
#include <chrono>

using namespace std; 
 

struct operations{
    string type; // R, W, IT, IE, D
    int node; // table we want to operate on 
    int clientSocket; 
};

struct transaction{
    long long timeStamp; 
    vector<operations> op; // vector of operations (sort of like a chain) 
     
};

class TransactionManager{

private: 
   // this will allow us to get the surrent time to do timestamp ordering
   long long getTimeStamp(){
      return chrono::duration_cast<chrono::milliseconds>(
         chrono::system_clock::now().time_since_epoch()).count();
   }

public: 


   transaction pickTransaction(const string& clientChoice){
      transaction t; // create the transaction 
      t.timeStamp = getTimeStamp(); //get the timestamp 

      if(stoi(clientChoice) == 1){
         // T1: Insert employee corresponding to a project 
         // hop 1: read from project table 
         // hop 2: insert to emplyees table 

         t.op.push_back({"R", 3}); 
         t.op.push_back({"IE", 1}); 
      }
      else if(stoi(clientChoice) == 2){
         // T2: add an employee to a project and insert corresponding task 
         // hop 1: read from projects 
         // hop 2: insert task 
         // hop 3: insert employee

         t.op.push_back({"R", 1}); 
         t.op.push_back({"IT", 2});
         t.op.push_back({"IE", 1}); 
      }
      else if(stoi(clientChoice) == 3){
         //T3: delete a task corresponding to a project (task complete)
         // hop 1: read from projects 
         // hop 2: delte a task (change to update)
         t.op.push_back({"R", 1}); 
         t.op.push_back({"D", 2}); 
      }
      else if(stoi(clientChoice) == 4){
         //T4: fire an employee corresponding to a project 
         // hop 1: read from projects 
         // hop 2: delte an employee 
         t.op.push_back({"R", 1}); 
         t.op.push_back({"D", 3});

      }

      return t; 
    }
};