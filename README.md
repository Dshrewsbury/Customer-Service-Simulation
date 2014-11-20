Customer Service Simulation
----------------------------

A minute-by-minute customer service simulation written in C++, simulating a day  for a number of servers in a customer service oriented job. 
Customers arrive randomly, are served at varying rates and queues are built up while all the servers are occupied.  Involves a struct to represent the customer, 
an overloaded operator-less-than so service events can be stored in a priority queue based on their event time. Also includes a dynamic array of queues to represent 
the wait queues for each server, and a vector to store customer objects after the service is completed.


More documentation to follow....