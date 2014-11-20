
// Programmer: Daniel Shrewsbury
// Editor(s) used: XP Notepad
// Compiler(s) used: VC++ 2010 Express
//

#include <ctime>
#include <fstream>
#include <iostream>
#include <queue>
#include <stack>
using namespace std;

struct custo
{
  double arriTime;
  double servTime;
  double comTime;
  char ID;
};

struct serviceEvent
{
  double eveTime;
  int servNum;
  bool operator<(const serviceEvent&) const;
};

bool serviceEvent::operator<(const serviceEvent& a) const
{
  if(a.eveTime < eveTime)
  {
    return true;
  }
  else
  {
    return false;
  }
}

// requires cmath and cstdlib (and srand and rand to seed the random number generator)
int getRandomNumberOfServiceRequests(double averageRequestsPerTimeStep)
{
  int requests = 0;
  double probOfnRequests = exp(-averageRequestsPerTimeStep);
  for (double randomValue = (double)rand() / RAND_MAX;
    (randomValue -= probOfnRequests) > 0.0;
    probOfnRequests *= averageRequestsPerTimeStep / (double)++requests);
  return requests;
}


int main()
{
  srand(1);
  rand();
  int numServers;
  double arrivRate;
  int maxWait;
  int minTime;
  int maxTime;
  int endTime;
  serviceEvent temp;
  int tempInt;
  int n = 0;
  int turnedAway = 0;

   cout << "Programmer: Daniel Shrewsbury\n";
   cout << "Description: Does a simulation of a customer service event. " << endl;

  // read the input values from a text file, one per line, in the order specified above.
  // open the input file
  ifstream fin;
  fin.open("simulation.txt");
  if (!fin.good()) throw "I/O error";

  fin >> numServers;
  fin >> arrivRate;
  fin >> maxWait;
  fin >> minTime;
  fin >> maxTime;
  fin >> endTime;


  cout << "number of servers:     " << numServers << endl;
  cout << "customer arrival rate: " << arrivRate <<  "per minute, for " << endTime << " minutes" << endl;
  cout << "maximum queue length:  " << maxWait << endl;
  cout << "minimum service time:  " << minTime << endl;
  cout << "maximum service time:  " << maxTime << endl;
  cout << endl;

  fin.close();
  // declare and create and assign arrays and queues to be used in the solution
  queue<custo>* servQueue = new queue<custo>[numServers];    //wait queues for the servers. The queues should store customer objects
  std::priority_queue<serviceEvent> servEvents;    //service events
  stack<custo> afterService;                //customer objects after service
  custo* custObj = new custo[numServers];    //server array, dynamic array of customer objects to represent the customers being served by each server

  bool* compArray = new bool[numServers];    //corresponding dynamic array of boolean values, set to true if the customer object at that index position is being served, false otherwise
  for(int j = 0; j < numServers; j++)
  {
    compArray[j] = false;
  }

  // the time step management loop
  for (int time = 0;; time++)
  {
    // handle all service events scheduled to complete in this time step
    //while (event queue is not empty AND time of the top event equals the current time)
    while(servEvents.empty() == false && servEvents.top().eveTime == time)
    {
       serviceEvent temp = servEvents.top();
      //remove the top service event from its queue
      servEvents.pop();

      //determine the server associated with the service event
      int tempInt = temp.servNum;

     // set this server's current customer completion time to the current time
      custObj[tempInt].comTime = time;

     // copy this server's current customer to the served customers queue
      afterService.push(custObj[tempInt]);

    //  set this server to idle (even if there are customers in that server's queue!)
      compArray[tempInt] = false;

      custo blank;
      blank.ID = ' ';
      custObj[tempInt] = blank;
     }


    // handle new arrivals -- can be turned away if all queues are at maximum length!
    //if time is less than the time at which new arrivals stop
    if(time < endTime)
    {
      //get the #of of arrivals from the Poisson process (a whole number >= 0) -- see the lecture topic notes!
      int numArrivals = getRandomNumberOfServiceRequests(arrivRate);
      //for each new arrival
      for(int i = 0; i < numArrivals; i++)
      {
          //create a new customer object with the arrival time equal to the current time

          int emptyQ;
          custo bob;
        bob.ID = (char)('A' + (n++ % 26));
       // cout << bob.ID << endl;
        bob.arriTime = time;


       // get the index of the next idle server -- set to a sentinel value if none are idle
         int j;
         for(j = 0; j < numServers; j++)
         {
            if(compArray[j] == false)
            {
             break;
            }
         }

       // get the index of the shortest queue -- in case of a tie, YOU DECIDE WHAT TO DO
        int shortestQ = 0;
       for(int i = 0; i < numServers;i++)
       {
          if(servQueue[shortestQ].size() > servQueue[i].size())
          {
            shortestQ = i;
          }
       }

      // check if all queues are full -- save as true or false
      bool test3 = false;
      for(int i = 0; i < numServers;i++)
       {
          if((int)servQueue[i].size() != maxWait)
          {
            test3 = true;
            break;
          }
       }

       // if there is an idle server AND his wait queue is empty, copy the new customer to that server's wait queue
       // else if all wait queues are full, "turn away the customer" and add one to the count of turned away customers
      if(j != numServers && servQueue[j].empty())
      {
         servQueue[j].push(bob);
      }
      else if(test3 == false)
      {
        turnedAway++;
      }
      else //else copy the new customer to the shortest queue
      {

        servQueue[shortestQ].push(bob);
        //possibly increment total customers
      }
    }
  }

    custo tempCusto;
    // for idle servers, move customer from wait queue and begin service
    //for each server
     for(int i = 0; i < numServers; i++)
     {

      //if (server is idle AND server's wait queue is not empty)
       if(compArray[i] == false && servQueue[i].empty() == false)
       {
          

        // remove top customer from wait queue
 
        // copy it to the server array
          custObj[i] = servQueue[i].front();

           
        servQueue[i].pop();


         //  set the customer's service time to the current time
          custObj[i].servTime = time;  

         //use random number generation to determing the amount of time it will take to complete the service
          custObj[i].comTime = minTime + time + (rand() % (maxTime - minTime + 1));

        // create a new service event for the time that service will be completed for this server and add to priority queue
          serviceEvent yay;
          yay.eveTime = custObj[i].comTime;
          yay.servNum = i;
          servEvents.push(yay);
  

     

           
         compArray[i] = true;
        }
      }

    // print the summary
    //print the current time
    cout << "Time: " << time << endl;
    cout << "--------------------------------" << endl;
    cout << "line now serving waiting queue" << endl;
    cout << "---- ----------- ---------------" << endl;
    //print a heading for the visual prepresentation of the servers and wait queues
    //for each server

    //queue<custo> servQueue2;
    for(int i = 0; i < numServers; i++)
    {
      //show the ID of the customer being served (or blank if idle)
        cout << "  " << i << "       ";

      if(compArray[i] == true)
        cout << custObj[i].ID;

      cout << "       ";

      //show the IDs of customers in the wait queue
      for(queue<custo> servQueue2 = servQueue[i]; !servQueue2.empty(); servQueue2.pop())
      {
        //cout << servQueue2.size();
        cout << servQueue2.front().ID;       
      }
      cout << endl;
    }

    // summarize performance
    //iterate over the served customers to get the average time between arrival and completion
    double count = 0;
    double avegTime = 0;

    stack<custo> copy;
    for(copy = afterService; !copy.empty();copy.pop())
    {
      custo temp3 = copy.top();
      avegTime = avegTime + (temp3.comTime - temp3.arriTime);
      count++;
      //and if any, print the average of their "completion time" minus their "arrival time"
    }

    if(count != 0)
      avegTime = avegTime / count;

    cout << "Avg time: " << avegTime << ". Turned away per minute: ";

    if(time != 0)
    {
      cout << (double)turnedAway / time << endl;
     }
    else
      cout << "0" << endl;


     // if the user wants to quit, break out of this loop
    char exitPlz;
    cout << "X, or x to exit, anything else to continue....." << endl;
    cin >> exitPlz;
    if(exitPlz == 'X' || exitPlz == 'x') break;
  }

}