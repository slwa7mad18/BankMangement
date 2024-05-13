#pragma once
#include <iostream>
#include <vector>
using namespace std;
class Customer
{
private:
    int ID;
    int Priority;
    int ArrivalTime;
    int WaitingTime;
    int DepartureTime;
    int ServingTime;
    int ServiceDuration;

public:
    static int Counter;

    Customer(int _ArrivalTime, int _Priority, int _ServiceDuration) {
        ID = Counter;
        Counter++;

        ArrivalTime = _ArrivalTime;
        Priority = _Priority;
        ServiceDuration = _ServiceDuration;
    }

    int GetID() const { return ID; }
    int GetPriority() const { return Priority; }
    int GetServiceDuration() const { return ServiceDuration; }
    int GetArrivalTime() const { return ArrivalTime; }

    int GetWaitingTime() const { return WaitingTime; }
    void SetWaitingTime(int _WaitingTime) { WaitingTime = _WaitingTime; }

    int GetDepartureTime() const { return DepartureTime; }
    void SetDepartureTime(int _DepartureTime) { DepartureTime = _DepartureTime; }

    int GetServingTime() const { return ServingTime; }
    void SetServingTime(int _ServingTime) { ServingTime = _ServingTime; }

    bool operator<(const Customer& c) const {
        return this->GetPriority() < c.GetPriority();
    }
};

