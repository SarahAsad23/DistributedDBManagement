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

    
    // function to process the clients request
    string ProcessClientRequest(int clientSocket, transaction t){
        // to read, need Type, ID, node, column

        string ack; 

        for(int i = 0; i < t.op.size(); i++){
            string response; 

            if(t.op[i].type == "R"){
                response = nodeManager.Read(t.op[i].IDNum, t.op[i].node, t.op[i].column);
            }
            else if(t.op[i].type == "W"){
                response = nodeManager.Write(t.op[i].IDString, t.op[i].column, t.op[i].newValue, t.op[i].node);    
            }

            ack = "ACK for operation " + to_string(i + 1) + ": " + response + "\n";
            send(clientSocket, ack.c_str(), ack.size(), 0); 


            //wait for client response before moving to next operation 
            char buffer[1024] = {0}; 
            read(clientSocket, buffer, sizeof(buffer)); 
        }

        return "Transaction Complete"; 
      
    }



    void HandleClient(int clientSocket){
        while(true){
            string transactions = "Choose a Transaction to run \n"
                                "1) Update Project name for Employee with ID 300 \n"
                                "2) Update teask decription for project with ID 20 (read from same project T1 wrote to)\n" 
                                "3) ANOTHER TRANSACTION\n" 
                                "4) ANOTHER TRANSACTION\n";
            send(clientSocket, transactions.c_str(), transactions.size(), 0); 

            char buffer[1024] = {0}; 
            read(clientSocket, buffer, sizeof(buffer));
        
            string clientChoice(buffer); 
            clientChoice.erase(clientChoice.find_last_not_of("\n\r\t") + 1); 
            cout << "Client selected transaction " << clientChoice << "\n"; 

            transaction t = transactionManager.pickTransaction(clientChoice); 

            ProcessClientRequest(clientSocket, t); 
        } 
    }

public: 

    // starting the server 
    void StartServer(int port) {
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
            int clientSocket = accept(serverSocket, nullptr, nullptr); 
            if (clientSocket < 0){
                cerr << "Unable to Accept Client Connection\n"; 
            }
            else{
                cout << "Successfully connected\n";
                // Create a thread for each client 
                thread clinetThread(&NodeServer::HandleClient, this, clientSocket); 
                clinetThread.detach(); // detach thread to handle client independantly 
            }    
        }

        //close the socket
        close(serverSocket);
        
    }

};