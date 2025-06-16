# DistributedDBManagement
Transaction Processing and Distributed Database management

This project implements a transaction processing system inspired
by the principles outlined in the paper "Transaction Chains: Achieving
Serializability with Low Latency in Geo-Distributed Storage Systems." 
The primary goal of this system is to emulate a geo-distributed
database environment and demonstrate how transaction chains
can be effectively utilized to achieve serializability with higher
concurrency and lower latency. Our system simulates a distributed
database environment using a local setup that spans across three vir-
tual nodes, each represented as a CSV-based database table, thereby
mimicking the behavior of a real-world geo-distributed storage
system of a project management application of a company. Several
transactions are also defined to simulate real-world use cases of the
application, showcasing how the system handles various transac-
tional scenarios in the distributed environment. The transactions
are predefined with a corresponding SC-graph created. Four of them
are SC-Cycles free, but an example dynamic transaction is created
to introduce SC-cycles to the graph and based on that we propose
a method to handle the specific case. The system uses a Server-
Client implementation and threading to simulate concurrent client
connections and concurrent transaction requests. It also facilitates
transaction chains where each transaction is chopped and executed
across multiple nodes as an independent local transaction, ensuring
consistency and adherence to hop-based communication. Aimed at
achieving serializability and avoiding race conditions, the system
incorporates locking mechanisms that ensure mutual exclusion on
shared resources to prevent conflicts of different transactions and
maintains a consistent execution order for transactions originating
from different clients, ensuring correctness in a distributed set
