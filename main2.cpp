#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <map>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

// Dummy NodeManager class for the example
class NodeManager {
public:
    string Read(int node) {
        return "Read operation completed on node " + to_string(node);
    }
    
    string InsertEmployee(int node) {
        return "Employee inserted on node " + to_string(node);
    }
    
    string InsertTask(int node) {
        return "Task inserted on node " + to_string(node);
    }
    
    string Delete(int node) {
        return "Deleted on node " + to_string(node);
    }
};

struct operations {
    string type; // R, W, IT, IE, D
    int node; // table we want to operate on 
};

struct transaction {
    long long timeStamp; 
    vector<operations> op; // vector of operations (like a chain)
};

class NodeServer {
private:
    NodeManager nodeManager;
    map<int, queue<operations>> nodeQueues;
    map<int, thread> nodeWorkers;  // Threads for each node to process its queue
    mutex coutMutex;  // Mutex to handle console output from multiple threads
    
    void ProcessOperation(const operations& op) {
        string response;
        if (op.type == "R") {
            response = nodeManager.Read(op.node);
        } else if (op.type == "IE") {
            response = nodeManager.InsertEmployee(op.node);
        } else if (op.type == "IT") {
            response = nodeManager.InsertTask(op.node);
        } else if (op.type == "D") {
            response = nodeManager.Delete(op.node);
        }

        // Output the result of the operation (simulating sending a response)
        {
            lock_guard<mutex> lock(coutMutex);
            cout << response << endl;
        }
    }

    void NodeWorker(int nodeId) {
        while (true) {
            operations op;
            {
                unique_lock<mutex> lock(nodeMutexes[nodeId]);
                if (nodeQueues[nodeId].empty()) continue;
                op = nodeQueues[nodeId].front();
                nodeQueues[nodeId].pop();
            }

            // Process the operation
            ProcessOperation(op);
        }
    }

    void ProcessTransaction(const transaction& t) {
        // Enqueue each operation in the appropriate node queue
        for (const operations& op : t.op) {
            {
                lock_guard<mutex> lock(nodeMutexes[op.node]);
                nodeQueues[op.node].push(op);
            }
        }

        // Ensure each node has a worker thread
        for (auto& [nodeId, queue] : nodeQueues) {
            if (nodeWorkers.find(nodeId) == nodeWorkers.end()) {
                nodeWorkers[nodeId] = thread(&NodeServer::NodeWorker, this, nodeId);
            }
        }

        // Wait for all threads to finish
        for (auto& [nodeId, worker] : nodeWorkers) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }

public:
    map<int, mutex> nodeMutexes;  // Mutexes for each node to ensure thread-safety when accessing queues

    void Execute() {
        // Example transaction
        transaction t;
        t.timeStamp = chrono::duration_cast<chrono::milliseconds>(
            chrono::system_clock::now().time_since_epoch()).count();

        // Simulating a few operations in a transaction
        t.op.push_back({"R", 1});
        t.op.push_back({"IE", 2});
        t.op.push_back({"IT", 1});
        t.op.push_back({"D", 3});

        // Process the transaction
        ProcessTransaction(t);
    }
};

int main() {
    NodeServer server;
    server.Execute();  // Start the operation process directly in main
    return 0;
}
