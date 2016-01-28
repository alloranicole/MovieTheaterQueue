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

void runSimulation();
 
int main()
{
    runSimulation();
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
    double lambda = (double) 1/tBetweenCArrival;
    double rNum;
    customerType C;
    static int CN = 1;
    rNum = ((double) rand() / (RAND_MAX)); 
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
     if(!wCustomers.isEmptyQueue())//make sure queue isn't empty
     {
          while(!wCustomers.isEmptyQueue())//add front until empty
          {
                tWait = tWait + wCustomers.front().getWaitingTime();
                wCustomers.deleteQueue();
          }
     }
}

void runSimulation()
{
     int CustomersComplete = 0, sID, tWait = 0, sTime, numServers, transTime,
         tBetweenArrival;
     double totalCustomers = 0;
     customerType D;
     waitingCustomerQueueType wCustomers;
     // ofstream outfile;
     // outfile.open("TheaterSimulation")
     setSimulationParameters(sTime, numServers, transTime, tBetweenArrival); 
     serverListType Servers(numServers);
     srand(time(0));
     for(int clock = 1; clock <= sTime; clock++){
         // Servers.updateServers(outfile);
         Servers.updateServers();
         if(!wCustomers.isEmptyQueue())
             wCustomers.updateWaitingQueue();
         CustomerArrival(wCustomers,tBetweenArrival,clock,transTime);
         if(!wCustomers.isEmptyQueue()){
            sID = Servers.getFreeServerID();
            if(sID != -1){
               D = wCustomers.front();
               tWait += D.getWaitingTime(); 
               wCustomers.deleteQueue();
               Servers.setServerBusy(sID, D, D.getTransactionTime());
            }
         }
     }

     //update any final output variables that need it
     updateWaitTime(wCustomers, tWait);
     totalCustomers = Servers.getCompleted() + Servers.getNumberOfBusyServers() + 
                      wCustomers.getCount();
     
     //Output original information.      
     //Note: I like to line up the outputs. If you do not like that,
     //      feel free to change it back.
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
     cout << "Customers remaining in queue: " << wCustomers.getCount() << endl;
     cout << "Average Wait Time:            " << tWait / totalCustomers << endl;
     //  outfile.close(); 
}


