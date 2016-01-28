#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <cmath>
#include <ctime>

#include "simulation.h"
#include "queueAsArray.h" 

using namespace std;

void setSimulationParameters(int& sTime, int& numOfServers,
                             int& transTime,
                             int& tBetweenCArrival);

void CustomerArrival(waitingCustomerQueueType& wCustomers, 
                     int tBetweenCArrival, int clock, int transTime);

void updateWaitTime(waitingCustomerQueueType& wCustomers, int& tWait);

void runSimulation(int seed);
 
int main(int argc, char* argv[])
{
    //If user inputs a seed it is taken in
    //else -1 is inputted to tell the program to use time(0) instead
    if(argc > 1){
    int x = atoi(argv[1]);
    runSimulation(x);
    } else 
      runSimulation(-1);
   
    return 0;
}

void setSimulationParameters(int& sTime, int& numOfServers,
							 int& transTime,
							 int& tBetweenCArrival)
{
    cout << "Enter the simulation time: ";
    cin >> sTime;
    cout << endl;

    cout << "Enter the number of servers: ";
    cin >> numOfServers;
    cout << endl;

    cout << "Enter the transaction time: ";
    cin >> transTime;
    cout << endl;

    cout << "Enter the time between customer arrivals: ";
    cin >> tBetweenCArrival;
    cout << endl;
}

void CustomerArrival(waitingCustomerQueueType& wCustomers, int tBetweenCArrival,
                int clock, int transTime){
    //Determine ratio for probability function
    double lambda = (double) 1/tBetweenCArrival;
    double rNum;
    customerType C;
    //Counts the customers that arrive
    static int CN = 1;
    //Random number generated to see if customer arrives
    rNum = ((double) rand() / (RAND_MAX)); 
    //If customer arrives they are added to the back of the line
    if(rNum > (exp(-1*lambda))){
      cout << "Customer " << CN << " arrived at time " << clock << endl; 
      C.setCustomerInfo(CN,clock,0,transTime);
      wCustomers.addQueue(C);
      CN++;
    }
}

//Updates tWait so that it also reflects the wait time of customers
//still waiting in the queue.
void updateWaitTime(waitingCustomerQueueType& wCustomers, int& tWait)
{

     //Isn't this redundant? The while loop will check first before going in the loop
     if(!wCustomers.isEmptyQueue())//make sure queue isn't empty
     {
          while(!wCustomers.isEmptyQueue())//add front until empty
          {
                tWait = tWait + wCustomers.front().getWaitingTime();
                wCustomers.deleteQueue();
          }
     }
}


void runSimulation(int seed)
{
     //Variables
     int sID, tWait = 0, sTime, numServers, transTime, tBetweenArrival;
     double totalCustomers = 0, customersQueue; 
     customerType D;
     waitingCustomerQueueType wCustomers;

     //Gets initial values from user
     setSimulationParameters(sTime, numServers, transTime, tBetweenArrival);
     //Sets up the server list with the number of available servers 
     serverListType Servers(numServers);
     //Checks if the user input a seed to use or if just using the clock
     if(seed == -1)
        srand(time(0));
     else
        srand((unsigned)seed);  
     //Main clock loop for the simulation
     for(int clock = 1; clock <= sTime; clock++){
         //Update the server list to decrement the transaction time of 
         //busy servers
         //Function changed to take out ostream so output prints to screen
         Servers.updateServers();
         //If customers in line, increment waiting time 
         if(!wCustomers.isEmptyQueue())
             wCustomers.updateWaitingQueue();
         //If customer arrives add them to the end of the line
         CustomerArrival(wCustomers,tBetweenArrival,clock,transTime);
         //If customers in the line and a server is free
         //pair the customer next in line with a free server
            //Check for free server
            sID = Servers.getFreeServerID();
            while(sID != -1 && !wCustomers.isEmptyQueue()){
               //Get customer from front of line
               D = wCustomers.front();
               //Add customer waiting time to total
               tWait += D.getWaitingTime(); 
               wCustomers.deleteQueue();
               //Move customer to server
               Servers.setServerBusy(sID, D, D.getTransactionTime());
               sID = Servers.getFreeServerID();
            }
     }

     //update any final output variables that need it
     customersQueue = wCustomers.getCount();  
     totalCustomers = Servers.getCompleted() + Servers.getNumberOfBusyServers() +
                      customersQueue;  
     updateWaitTime(wCustomers, tWait);
     
     //Output original information.      
     //Note: I like to line up the outputs. If you do not like that,
     //      feel free to change it back.
     cout << endl << endl; 
     cout << "---You supplied:--------------------" << endl;
     cout << "Simulation Time:       " << sTime << endl;
     cout << "Number of Servers:     " << numServers << endl;
     cout << "Transaction Time:      " << transTime << endl;
     //Output result information.
     cout << "Time Between Arrivals: " << tBetweenArrival << endl;
     cout << "---The results are:-----------------" << endl;
     cout << "Total Wait Time:              " << tWait << endl;
     cout << "Completed Transactions:       " << Servers.getCompleted() << endl;
     cout << "Customers still with servers: " << Servers.getNumberOfBusyServers() << endl;
     cout << "Customers remaining in queue: " << customersQueue << endl;
     cout << "Average Wait Time:            " << tWait / totalCustomers << endl; 
     cout << endl; 
}


