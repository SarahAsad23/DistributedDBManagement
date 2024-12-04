#pragma once

#include <vector>
#include <string>
#include <chrono>

using namespace std; 
 

struct operations{
    string type; // R or W
    int IDNum; // record ID to operate on 
    string IDString; 
    string column; // column name 
    string newValue; // new value for write operations 
    int node; // table we want to operate on 

   // these are for adding a new task to table 
    string TaskID; 
    string projectID; 
    string taskName;
    string taskDescription; 
};

struct transaction{
    long long timeStamp; 
    vector<operations> op; // vector of operations (sort of like a chain) 
     
};

class TransactionManager{

private: 


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
         //T2: look up Project with Project_Manager_ID == 300 
         //add a new corresponding(Project_ID == 20) task in 
         //Task.csv with Task_Id == 0, Task_Name == prepare annual budget report, 
         //Task_Discription == estimate cost of the project 

         t.op.push_back({"R", 300, "", "Project_Manager_ID", "", 1}); 
         t.op.push_back({"A", 0, "", "", "", 2, "0", "300", "prepare annual budget report", "estimate cost of the project"});
      }
      else if(stoi(clientChoice) == 3){
         //T3: look up Project with Project ID== 20(to get corresponding Project_ID)
         //add a new corresponding(Project_ID == 20)  task in Task.cvs with Task_id == 14, 
         //Task_Name == place an advertisement, Task_Discription == find ADs sponsor
         t.op.push_back({"R", 20, "", "Project_ID", "", 1}); 
         t.op.push_back({"A", 0, "", "", "", 2, "14", "20", "place an advertisement", "find ADs sponsor"}); 
      }
      else if(stoi(clientChoice) == 4){
         //T4: Empty for now. 

      }

      return t; 
    }
};