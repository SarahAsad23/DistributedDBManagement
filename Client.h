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

        while(true){
            //read transaction choices from buffer 
            char choice[1024] = {0}; 
            read(clientSocket, choice, sizeof(choice));

            cout << choice << "\n"; 

            // client enters number 
            string userChoice; 
            cin >> userChoice; 

            userChoice += '\n';

            cout << "now sending clients choice\n"; 

            // send that choice to server 
            send(clientSocket, userChoice.c_str(), userChoice.size(), 0); 
            cout << "Choice sent to server " << userChoice << endl;

            //recieve response/ack from server that they recived that choice
            char buffer[1024] = {0}; 
            read(clientSocket, buffer, sizeof(buffer)); 
            cout << "Response from server " << buffer << endl; 

            // then client also sends response to ack which allows the 
            // second hop to happen 
            string res = "OK"; 
            send(clientSocket, res.c_str(), res.size(), 0); 
            cout << "Sending response: " << res << endl; 

            // read second ack 
            read(clientSocket, buffer, sizeof(buffer)); 
            cout << "Response from server " << buffer << endl; 

            //send ok 
            send(clientSocket, res.c_str(), res.size(), 0); 
            cout << "Sending response: " << res << endl;

            
        }

        // close socket connection
        close(clientSocket); 
    }

};