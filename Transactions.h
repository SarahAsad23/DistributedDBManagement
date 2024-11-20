using namespace std; 

class Transactions{


    /*
    Transaction 1: look for project with ID 5 and update the 
    corresponding project managers role to Manager 

    Operations
    1) 1st hop: read project manager ID from projects table 
       where the project with ID is 5 

    2) 2nd hop: write to employees table with employee ID, ID 
       from above and change ther role to "Manager"
    */

    /*
    Transaction 2: add a Project for Employee with ID 300

    Operations: 
    1) 1st hop: read from employees table to ensure that 
       employee with ID 300 exists 

    2) 2nd hop: in projects table, add a new project with 
       Project_Manager_ID set to 300 
    */

   /*
   Transaction 3: for a project with ID 15, update the corresponding 
   task description for that project 

   Operations: 
   1) 1st hop: read from projects table to ensure that the project 
      with ID 15 exists 

   2) 2nd hop: find project task with ID 15 to update the 
      corresponding task descrption 
   */

    /*
    Transaction 4: 

    Operations: 
    1) 1st hop: 

    2) 2nd hop: 
    */
  
};