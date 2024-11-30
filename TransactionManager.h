#pragma once

#include <vector>
#include <string>
#include <chrono>

using namespace std; 

// we will set it up so that the tranasction manager has the ability to connect to the servers 
// also through a socket. so we can have many servers and many clients. 

struct operations{
    string type; // R or W
    int IDNum; // record ID to operate on 
    string IDString; 
    string column; // column name 
    string newValue; // new value for write operations 
    int node; // table we want to operate on 
};

struct transaction{
    long long timeStamp; 
    vector<operations> op; // vector of operations (sort of like a chain) 
     
};

class TransactionManager{
public: 
   // this will allow us to get the surrent time to do timestamp ordering
   long long getTimeStamp(){
      return chrono::duration_cast<chrono::milliseconds>(
         chrono::system_clock::now().time_since_epoch()).count();
   }

   transaction pickTransaction(const string& clientChoice){
      transaction t; // create the transaction 
      t.timeStamp = getTimeStamp(); //get the timestamp 

      if(stoi(clientChoice) == 1){
         // T1: look up employee with ID 300 and update that 
         // corresponding employees project name 

         t.op.push_back({"R", 300, "", "Employee_ID", "", 3}); 
         t.op.push_back({"W", 300, "Nebula Shift", "Project_Name", "New Name", 1}); 
      }
      else if(stoi(clientChoice) == 2){
         //T2: 
         
      }
      else if(stoi(clientChoice) == 3){
         //T3: 

      }
      else if(stoi(clientChoice) == 4){
         //T4: 

      }

      return t; 
    }

   // need to have a function here that checks for cycles
   // dont really need to check for predefined transactions 
   // but need to check for dynamic transactions 
   bool hasScCycle(transaction t){
      
   }
};