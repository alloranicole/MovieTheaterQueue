#include <iostream>
#include <string>
#include <cstdlib>
 
#include "simulation.h"
#include "queueAsArray.h"

using namespace std;
 

//*************** customerType ************

void customerType::setCustomerInfo(int customerN, int arrvTime, 
                                   int wTime, int tTime)
{
    customerNumber = customerN;
    arrivalTime = arrvTime;
    waitingTime = wTime;
    transactionTime = tTime;
}

customerType::customerType(int customerN, int arrvTime, 
                           int wTime, int tTime)
{
    setCustomerInfo(customerN, arrvTime, wTime, tTime);
}


int customerType::getWaitingTime() const
{
    return waitingTime;
}

void customerType::incrementWaitingTime()
{
    waitingTime++;
}

void customerType::setWaitingTime(int time)
{
    waitingTime = time;
}

int customerType::getArrivalTime() const
{
    return arrivalTime;
}

int customerType::getTransactionTime() const
{
    return transactionTime;
}

int customerType::getCustomerNumber() const
{
    return customerNumber;
}
//**************** serverType **********

serverType::serverType()
{
    status = "free";
    transactionTime = 0;
}

bool serverType::isFree() const
{
    return (status == "free");
}

void serverType::setBusy()
{
    status = "busy";
}

void serverType::setFree()
{
    status = "free";
}

void serverType::setTransactionTime(int t)
{
    transactionTime = t;
}

void serverType::setTransactionTime()
{
    int time;

    time = currentCustomer.getTransactionTime();

    transactionTime = time;
}

void serverType::decreaseTransactionTime()
{
    transactionTime--;
}

int serverType::getRemainingTransactionTime() const
{
    return transactionTime;
}

void serverType::setCurrentCustomer(customerType cCustomer)
{
    currentCustomer = cCustomer;
}

int serverType::getCurrentCustomerNumber() const
{
    return currentCustomer.getCustomerNumber();
}

int serverType::getCurrentCustomerArrivalTime() const
{
    return currentCustomer.getArrivalTime();
}

int serverType::getCurrentCustomerWaitingTime() const
{
    return currentCustomer.getWaitingTime();
}

int serverType::getCurrentCustomerTransactionTime() const
{
    return currentCustomer.getTransactionTime();
}


//************** serverListType ***********

serverListType::serverListType(int num)
{
    numOfServers = num;
    completed = 0;
    servers = new serverType[num];
}

serverListType::~serverListType()
{
    delete [] servers;
}

int serverListType::getCompleted()
{
    return completed;
}

int serverListType::getFreeServerID() const
{
    int serverID = -1;

    int i;

    for (i = 0; i < numOfServers; i++)
        if (servers[i].isFree())
        {
            serverID = i;
            break;
        }

    return serverID;
}

int serverListType::getNumberOfBusyServers() const
{
    int busyServers = 0;

    int i;

    for (i = 0; i < numOfServers; i++)
        if (!servers[i].isFree())
            busyServers++;

    return busyServers;
}

void serverListType::setServerBusy(int serverID, 
                                   customerType cCustomer, 
                                   int tTime)
{
    servers[serverID].setBusy();
    servers[serverID].setTransactionTime(tTime);
    servers[serverID].setCurrentCustomer(cCustomer);
    cout << "Server " << serverID+1 << " now serving Customer " << cCustomer.getCustomerNumber()
         << " at time " << cCustomer.getArrivalTime() + cCustomer.getWaitingTime()
         << endl; 
}

void serverListType::setServerBusy(int serverID, 
                                   customerType cCustomer)
{
    int time;

    time = cCustomer.getTransactionTime();

    servers[serverID].setBusy();
    servers[serverID].setTransactionTime(time);
    servers[serverID].setCurrentCustomer(cCustomer);
}

//void serverListType::updateServers(ostream& outFile)
void serverListType::updateServers()
{
    int i;

    for (i = 0; i < numOfServers; i++)
        if (!servers[i].isFree())
        {
            servers[i].decreaseTransactionTime();

            if (servers[i].getRemainingTransactionTime() == 0)
            {
                cout << "From Server " << (i + 1) 
                        << " Customer "
                        << servers[i].getCurrentCustomerNumber()
                        << "\n     departed at time "
                        << servers[i].
                              getCurrentCustomerArrivalTime()
                           + servers[i].
                              getCurrentCustomerWaitingTime() 
                           + servers[i].
                              getCurrentCustomerTransactionTime()
                        << endl;
                completed++;
                servers[i].setFree();
            }
        }
}

//*************** waitQueue ************


waitingCustomerQueueType::waitingCustomerQueueType(int size)
                          :queueType<customerType>(size)
{
}

void waitingCustomerQueueType::updateWaitingQueue()
{
    customerType cust;

    cust.setWaitingTime(-1);  
    int wTime = 0;
	
	addQueue(cust);

    while (wTime != -1)
    {
        cust = front();
        deleteQueue();

        wTime = cust.getWaitingTime();
        if (wTime == -1)
            break;
        cust.incrementWaitingTime();
        addQueue(cust);
	}
}
