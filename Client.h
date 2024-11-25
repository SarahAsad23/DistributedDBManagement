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
#include <unistd.h>         // for close()
#include <arpa/inet.h>




class Client{

public: 
    void clientConnect(const char *server, int serverPort){
        int clientSocket = socket(AF_INET, SOCK_STREAM, 0); 
        if(clientSocket < 0){
            cerr << "Client Socket Creation Failed\n"; 
        }

        // set up server structure
        sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET; 
        serverAddress.sin_addr.s_addr = inet_addr(server);  
        serverAddress.sin_port = htons(serverPort); 

        if(connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0){
            cerr << "Connection Failed\n"; 
        }

        string message = "HELLO SERVER\n"; 

        // send request to server 
        send(clientSocket, message.c_str(), message.size(), 0); 
        cout << "Message sent to Server " << message << "\n"; 

        //recieve response from server 
        char buffer[1024] = {0}; 
        read(clientSocket, buffer, sizeof(buffer)); 

        cout << "Response from server " << buffer << "\n"; 


        // close socket connection
        close(clientSocket); 
    }

};