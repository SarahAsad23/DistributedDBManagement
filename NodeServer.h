#pragma once

#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include "NodeManager.h"
#include <vector>
#include <sys/types.h>		//socket, bind
#include <sys/socket.h>		//coket, bind, listen, inet_ntoa
#include <netinet/in.h>		//hton1, htons, inet_ntoa
#include <unistd.h>
#include <map>
#include "TransactionManager.h"



using namespace std; 

// we will use sockets to set up the server and  will 
    // route it to operate on only each table/node table. 

    // each node runs as a server responsible for 
        // listening for requests from the transaction manager (client)
        // performing read and write operations on csv file 
        // returning ack to the client after each hop

class NodeServer{

private: 

    NodeManager nodeManager; 
    TransactionManager transactionManager; 
    int node; 
    int port; 

    map<int, mutex> nodeLocks; 

    
    // function to process the clients request
    string ProcessClientRequest(int clientSocket, transaction t){
        // to read, need Type, ID, node, column

        string ack; 

        for(int i = 0; i < t.op.size(); i++){
            string response; 

            // Get the node we are going to be operating on 
            int targetNode = t.op[i].node; 
            nodeLocks[targetNode].lock(); // put a lock on the table being operated on 
            try{
                // read from table
                if(t.op[i].type == "R"){
                    response = nodeManager.Read(t.op[i].node);
                } 
                else if(t.op[i].type == "IE"){
                    response = nodeManager.InsertEmployee(t.op[i].node); 
                }
                // insert into task table 
                else if(t.op[i].type == "IT"){
                    response = nodeManager.InsertTask(t.op[i].node); 
                }
                // delete from table 
                else if(t.op[i].type == "D"){
                    response = nodeManager.Delete(t.op[i].node);
                }
            } catch (const exception& e){
                response = "Error: " + string(e.what()); //catch error
            }

            // Release the lock 
            nodeLocks[targetNode].unlock(); 

            ack = "ACK for operation " + to_string(i + 1) + ": " + response + "\n";
            send(clientSocket, ack.c_str(), ack.size(), 0); 


            //wait for client response before moving to next operation 
            char buffer[1024] = {0}; 
            read(clientSocket, buffer, sizeof(buffer)); 
        }

        string complete = "Transaction Complete\n"; 
        send(clientSocket, complete.c_str(), complete.size(), 0); 

        return complete; 
      
    }


    // This is the function that is threaded (server/client communication) 
    void HandleClient(int clientSocket){
        while(true){
            string transactions = "Choose a Transaction to run \n"
                                "1) Insert an Employee working on specific Project \n"
                                "       hop 1: Read from Project\n"
                                "       hop 2: Insert Employee\n"
                                "2) Insert an Employee for a Project and add corresponding task\n"
                                "       hop 1: read a project\n"
                                "       hop 2: Insert Task\n"
                                "       hop 3: Insert Employee\n" 
                                "3) Update a Task Corresponding to a Project \n"
                                "       hop 1: read a project\n"
                                "       hop 2: Update Task\n" 
                                "4) Fire an Employee working on a specific project\n"
                                "       hop 1: Read project\n"
                                "       hop 2: Delete employee\n";
            send(clientSocket, transactions.c_str(), transactions.size(), 0); 

            char buffer[1024] = {0}; 
            read(clientSocket, buffer, sizeof(buffer));
        
            string clientChoice(buffer); 
            clientChoice.erase(clientChoice.find_last_not_of("\n\r\t") + 1); 
            cout << "Client selected transaction " << clientChoice << "\n"; 

            transaction t = transactionManager.pickTransaction(clientChoice); 

            ProcessClientRequest(clientSocket, t); 
        } 

        close(clientSocket); 
    }

public: 

    // starting the server 
    void StartServer(int port) {

        // initialize locks dor nodes 
        for(int i = 1; i < 3; i++){
            nodeLocks[i]; 
        }

        // IPv4 and TCP 
        int serverSocket = socket(AF_INET, SOCK_STREAM, 0); 
        if(serverSocket == 0){
            cerr << "Unable to Create Socket\n"; 
        }

        // set up server structure
        sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET; 
        serverAddress.sin_addr.s_addr = INADDR_ANY;  
        serverAddress.sin_port = htons(port); 

        // then bind the socket to the port 
        if(bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0){
            cerr<< "Error Binding\n"; 
        }

        // then listen for client connections 
        if (listen(serverSocket, 3) < 0){
            cerr << "Unable to Listen\n"; 
        }

        cout << "Server now listening on port \n" << port << "\n"; 

        while(true){
            // Accept client connecions 
            int clientSocket = accept(serverSocket, nullptr, nullptr); 
            if (clientSocket < 0){
                cerr << "Unable to Accept Client Connection\n"; 
            }
            else{
                cout << "Successfully connected\n";
                // Create a thread for each client 
                thread clientThread(&NodeServer::HandleClient, this, clientSocket); 
                clientThread.detach(); // detach thread to handle client independantly 

            }    
        }

        //close the socket
        close(serverSocket);
        
    }

};