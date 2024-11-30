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
         //T2: look up employee with ID 300(to get the Project_Name)
         //update coresponding Task_ID in Task.csv to 0 (indicates that it is in high priority)

         t.op.push_back({"R", 300, "", "Employee_ID", "", 3}); 
         t.op.push_back({"R", 10, "8", "Task_ID", "0", 2})
      }
      else if(stoi(clientChoice) == 3){
         //T3: look up employee with ID 300(to get the Project_ID)
         //add a new task of it in Task.cvs(the project is really important and has more tasks to do)

         t.op.push_back({"R", 300, "", "Employee_ID", "", 3}); 
         t.op.push_back({"W", })

      }
      else if(stoi(clientChoice) == 4){
         //T4: read Task_Name="client feedback analysis"(get the info and find it in a hurry and not done yet)
         //delete Employee_ID==500 in Employee.csv(the company choose to fire another manager due to tighter budget)

         t.op.push_back({"R", })
         t.op.push_back({"W", })

      }

      return t; 
    }

   // need to have a function here that checks for cycles
   // dont really need to check for predefined transactions 
   // but need to check for dynamic transactions 
   bool hasScCycle(transaction t){
      
   }
};