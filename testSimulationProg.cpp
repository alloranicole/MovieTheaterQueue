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

bool customerArrival(int tBetweenCArrival);

void updateWaitTime(waitingCustomerQueueType& wCustomers, int& tWait);

void runSimulation();
 
int main(int argc, char* argv[])
{
    //If user inputs a seed it is taken in
    //else -1 is inputted to tell the program to use time(0) instead
    if(argc > 1){
      int x = atoi(argv[1]);
      srand((unsigned)x);  
      runSimulation();
    } else{ 
      srand(time(0));
      runSimulation();
     }
   
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

bool customerArrival(int tBetweenCArrival){
    //Determine ratio for probability function
    double lambda = (double) 1/tBetweenCArrival;
    double rNum;
    //Random number generated to see if customer arrives
    rNum = ((double) rand() / (RAND_MAX)); 
    //If customer arrives return true
    if(rNum > (exp(-1*lambda)))
      return true;
    else
      return false; 
    
}

//Updates tWait so that it also reflects the wait time of customers
//still waiting in the queue.
//WARNING: Queue will be emptied. Make a copy before calling if need be.
void updateWaitTime(waitingCustomerQueueType& wCustomers, int& tWait)
{
     while(!wCustomers.isEmptyQueue())//add front until empty
     {
          tWait = tWait + wCustomers.front().getWaitingTime();
          wCustomers.deleteQueue();
     }
}


void runSimulation()
{
     //Variables
     int sID, tWait = 0, sTime, numServers, transTime, tBetweenArrival;
     double customersQueue, customerNum = 0;//Counts the customers that arrive
     customerType customer;
     waitingCustomerQueueType wCustomers;

     //Gets initial values from user
     setSimulationParameters(sTime, numServers, transTime, tBetweenArrival);
     //Sets up the server list with the number of available servers 
     serverListType servers(numServers);
     //Main clock loop for the simulation
     for(int clock = 1; clock <= sTime; clock++)
     {
          //Update the server list to decrement the transaction time of 
          //busy servers
          //Function changed to take out ostream so output prints to screen
          servers.updateServers();
          //If customers in line, increment waiting time 
          if(!wCustomers.isEmptyQueue())
             wCustomers.updateWaitingQueue();
          //If customer arrives add them to the end of the line
          if(customerArrival(tBetweenArrival))
          {
             customerNum++;
             cout << "Customer " << customerNum << " arrived at time " << clock << endl; 
             customer.setCustomerInfo(customerNum,clock,0,transTime);
             wCustomers.addQueue(customer);
          }
          //If customers in the line and a server is free
          //pair the customer next in line with a free server
          //Check for free server
          sID = servers.getFreeServerID();
          while(sID != -1 && !wCustomers.isEmptyQueue())
          {
               //Get customer from front of line
               customer = wCustomers.front();
               //Add customer waiting time to total
               tWait += customer.getWaitingTime(); 
               wCustomers.deleteQueue();
               //Move customer to server
               servers.setServerBusy(sID, customer, customer.getTransactionTime());
               sID = servers.getFreeServerID();
          }
     }
    
/*
     I believe the way the simulation was already set up does not require this added
     measure after the loop is over or else when it was created within the printf of
     updateServers() the created would have made the time unit displayed be clock - 1.
     Right now if this is added the program will display that the server finished a 
     transaction at time 101 if that were the case, rather than the time 100. So 
     personally I do not believe this is needed.

     //Added feature after discussed in class where the first part of the clock loop
     //is taken care of after the loop ends   
     //Update the server list to decrement the transaction time of 
     //busy servers
     //Function changed to take out ostream so output prints to screen
     servers.updateServers();
     //If customers in line, increment waiting time 
     if(!wCustomers.isEmptyQueue())
        wCustomers.updateWaitingQueue();
     
*/
     
     //update any final output variables that need it
     customersQueue = wCustomers.getCount();  
     updateWaitTime(wCustomers, tWait);
     
     //Output original information.      
     //Note: I like to line up the outputs. If you do not like that,
     //      feel free to change it back.
     cout << endl << endl; 
     cout << "---You supplied:--------------------" << endl;
     cout << "Simulation Time:       " << sTime << endl;
     cout << "Number of Servers:     " << numServers << endl;
     cout << "Transaction Time:      " << transTime << endl;
     cout << "Time Between Arrivals: " << tBetweenArrival << endl;
     //Output result information.
     cout << "---The results are:-----------------" << endl;
     cout << "Total Wait Time:              " << tWait << endl;
     cout << "Completed Transactions:       " << servers.getCompleted() << endl;
     cout << "Customers still with servers: " << servers.getNumberOfBusyServers() << endl;
     cout << "Customers remaining in queue: " << customersQueue << endl;
     cout << "Average Wait Time:            " << tWait / customerNum << endl; 
     cout << endl; 
}


