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
    int node; 
    int port; 

    // function to process the clients request
    string ProcessClientRequest(const string& request){
        stringstream ss(request); 
        string operation, column; 
        int ID; 

        ss >> operation >> ID >> column; 

        if (operation == "READ"){
            string response = nodeManager.Read(ID, node, column); 
            return response.empty() ? "ERROR: Record not found" : "SUCCESS" + response + "\n"; 
        }
        else{ // want to wrtie 
            return "write"; 
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
            }

            char buffer[1024] = {0}; 
            read(clientSocket, buffer, sizeof(buffer)); 
            cout << "Recieved" << buffer << "\n"; 

            // process the client request and get a response
            //string response = ProcessClientRequest(buffer); 

            string response = "HELLO FROM SERVER\n"; 

            // sent the response back to the cleint 
            send(clientSocket, response.c_str(), response.size(), 0); 

            //close the socket
            close(clientSocket); 

        }
    }

  


    


    

    

};