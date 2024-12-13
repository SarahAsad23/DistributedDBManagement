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
#include <chrono>
#include <thread>


using namespace std; 
using namespace chrono; 

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

        srand(time(0)); 

        // chrnon used to see how long execution took 
        auto start = high_resolution_clock::now(); 
        for(int i = 0; i < 50; i++){
            //read transaction choices from buffer 
            char choice[1024] = {0}; 
            read(clientSocket, choice, sizeof(choice));
            cout << choice << "\n"; 

            //client will send a random transaction between 1 - 4
            int randomNumber = rand() % 4 + 1; 
            string userChoice = to_string(randomNumber) + '\n';
            cout << "now sending clients choice\n"; 
            // send that choice to server 
            send(clientSocket, userChoice.c_str(), userChoice.size(), 0); 
            cout << "Choice sent to server " << userChoice << endl;

            //this_thread::sleep_for(chrono::seconds(1)); 

            while(true){

                char buffer[1024] = {0};
                //recieve response/ack from server that they recived that choice 
                read(clientSocket, buffer, sizeof(buffer)); 
                cout << "Response from server " << buffer << endl; 

                // If transaction complete then break 
                if(strstr(buffer, "Transaction Complete\n")){
                    break;
                }
                else{
                    // then client also sends response to ack which allows the 
                    // second hop to happen 
                    string res = "OK\n"; 
                    send(clientSocket, res.c_str(), res.size(), 0); 
                    cout << "Sending response: " << res << endl; 
                } 
            }
        }

        auto end = high_resolution_clock::now(); 
        auto duration = chrono::duration_cast<chrono::microseconds>(end-start);

        cout << "Time taken: " << duration.count() << " Milliseconds \n";

        // close socket connection
        close(clientSocket); 
    }
};


