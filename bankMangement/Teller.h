#pragma once
#include "Customer.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
class Teller
{
private:
    int ID;
    int NextFreeTime;
    Customer* CurrentCustomer;
    ofstream* OutFile;

public:
    static int Counter;

    Teller(ofstream* _OutFile) {
        ID = Counter;
        Counter++;

        NextFreeTime = 0;

        OutFile = _OutFile;
    }

    int GetID() { return ID; }

    int GetNextFreeTime() { return NextFreeTime; }
    void SetNextFreeTime(int _NextFreeTime) { NextFreeTime = _NextFreeTime; }

    void SetCurrentCustomer(Customer* _CurrentCustomer) {
        CurrentCustomer = _CurrentCustomer;
    }

    void ServeCustomer() {
        CurrentCustomer->SetWaitingTime(
            NextFreeTime - CurrentCustomer->GetArrivalTime() > 0
            ? NextFreeTime - CurrentCustomer->GetArrivalTime()
            : 0);

        CurrentCustomer->SetServingTime(CurrentCustomer->GetArrivalTime() +
            CurrentCustomer->GetWaitingTime());

        CurrentCustomer->SetDepartureTime(CurrentCustomer->GetServingTime() +
            CurrentCustomer->GetServiceDuration());

        NextFreeTime = CurrentCustomer->GetDepartureTime();

        *OutFile << CurrentCustomer->GetID() << "," << ID << ","
            << CurrentCustomer->GetPriority() << ","
            << CurrentCustomer->GetArrivalTime() << ","
            << CurrentCustomer->GetServiceDuration() << ","
            << CurrentCustomer->GetWaitingTime() << ","
            << CurrentCustomer->GetServingTime() << ","
            << CurrentCustomer->GetDepartureTime() << endl;
    }
};

