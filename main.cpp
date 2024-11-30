
#include <string>
#include <iostream>
#include <cstring>
//#include "NodeServer.h"
//#include "Client.h"
#include "NodeManager.h"

using namespace std; 

/*

int main(int argc, char* argv[]){

    // Check if enough arguments are provided
    if (argc < 3) {
        cerr << "Usage: Server port\n";
        cerr << "Client ServerIP port\n";
        return 1; // Exit with error
    }

    cout << "Usage: Server port\n";
    cout << "Client ServerIP port\n"; 

    if (strcmp(argv[1], "Server") == 0) {
        if (argc < 3) {
            cerr << "Server requires a port number.\n";
            return 1;
        }

        int serverPort = atoi(argv[2]); 
        if (serverPort > 0) {
            NodeServer nodeServer; 
            nodeServer.StartServer(serverPort); 
        } else {
            cerr << "Invalid port number.\n";
            return 1;
        }
    } 
    else if (strcmp(argv[1], "Client") == 0) {
        if (argc < 4) {
            cerr << "Client requires ServerIP and port arguments.\n";
            return 1;
        }

        int serverPort = atoi(argv[3]);
        if (serverPort > 0) {
            Client client; 
            client.clientConnect(argv[2], serverPort); 
        } else {
            cerr << "Invalid port number.\n";
            return 1;
        }
    } 
    else {
        cerr << "Invalid argument. Usage: Server port or Client ServerIP port\n";
        return 1;
    }

    return 0;
}

*/

int main(){
      // This is where we will define the transactions 
    NodeManager n; 
    n.Write("Nebula Shift", "Project_Name", "NEW PROJECT", 1);
}


  
    


    
