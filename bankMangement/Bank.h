#pragma once
#include "Teller.h"
#include <iostream>
#include <queue>
#include <random>
#include <vector>
#include <fstream>
using namespace std;
class Bank
{
private:
    priority_queue<Customer> CustomersQueue;
    vector<Teller> TellersVector;
    vector<Teller> TellersServiceTimeVector;
    int MinServiceDuration, MaxServiceDuration;
    int LastCustomerArrival; // Used to validate that new arrival is later than or
    // at same time of last arrival
    int AddExtraTellerThresholdPerCustomers;
    int RemoveExceededTellerThreshold;
    int RemoveExceededTellerCounter;
    int InitialTellerCount;
    ofstream OutFile, outFileActions, outFileStatistics;

#pragma region Statistics Variables
    double AverageQueueLength_Counter;
    double AverageQueueLength;

    double AverageWaitingTime_Counter;
    double AverageWaitingTime;

    double TotalSystemTime;
    double NumberOfTellers;
    double AverageServicesTime_Counter;
    double TotalServicesTime;
#pragma endregion

    void AddTeller() { TellersVector.push_back(Teller(&OutFile)); }

    void RemoveTeller() { TellersVector.pop_back(); }

    void CheckIfAddExtraTeller() {
        int CheckValue = AddExtraTellerThresholdPerCustomers * TellersVector.size();
        if (CustomersQueue.size() > CheckValue) {
            AddTeller();
            NumberOfTellers++;
            outFileActions << "+ ADD  :: Teller with ID ("
                << TellersVector.back().GetID()
                << ") was added as extra teller." << endl;
        }
    }

    void CheckIfRemoveExceededTeller() {
        int CheckValue = AddExtraTellerThresholdPerCustomers * TellersVector.size();
        if (CustomersQueue.size() <= CheckValue) {
            RemoveExceededTellerCounter++;
        }
        else {
            RemoveExceededTellerCounter = 0;
        }

        if (TellersVector.size() > InitialTellerCount && TellersVector.size() > 0) {
            if (RemoveExceededTellerCounter >= RemoveExceededTellerThreshold) {
                outFileActions << "- REM  :: Teller with ID ("
                    << TellersVector.back().GetID()
                    << ") was removed as exceeded teller." << endl;
                RemoveTeller();
                RemoveExceededTellerCounter = 0;
            }
        }
    }

    int GetIndexOfNearestFreeTeller() {
        int minIndex = 0;

        for (int i = 1; i < TellersVector.size(); i++) {
            if (TellersVector.at(minIndex).GetNextFreeTime() >
                TellersVector.at(i).GetNextFreeTime()) {
                minIndex = i;
            }
        }

        return minIndex;
    }

    int ConvertTimeToSecond(string time) {
        int seconds = 0;
        auto TimeSegments = CustomSplit(time, ':');

        if (TimeSegments.size() == 1) {
            seconds = stoi(TimeSegments[0]);
        }
        else if (TimeSegments.size() == 2) {
            seconds = stoi(TimeSegments[0]) * 60 * 60 + stoi(TimeSegments[1]) * 60;
        }
        else if (TimeSegments.size() == 3) {
            seconds = stoi(TimeSegments[0]) * 60 * 60 + stoi(TimeSegments[1]) * 60 +
                stoi(TimeSegments[2]);
        }

        return seconds;
    }

    bool ValidateArrivalTime(int ArrivalTime) {

        return ArrivalTime >= LastCustomerArrival;
    }

    int GenerateRandomServiceDuration() {
        return rand() % (MaxServiceDuration - MinServiceDuration + 1) +
            MinServiceDuration;
    }

    void ServeNextCustomerInQueue() {
        if (TellersVector.size() < 1) {
            return;
        }

        Customer CurrentCustomer = CustomersQueue.top();
        int TellerIndex = GetIndexOfNearestFreeTeller();

        if (LastCustomerArrival < TellersVector.at(TellerIndex).GetNextFreeTime()) {
            return;
        }

        TellersVector.at(TellerIndex).SetCurrentCustomer(&CurrentCustomer);

        TellersVector.at(TellerIndex).ServeCustomer();

        double AverageWaitingTime_Top =
            (AverageWaitingTime * AverageWaitingTime_Counter) +
            CurrentCustomer.GetWaitingTime();
        AverageWaitingTime_Counter++;
        AverageWaitingTime = AverageWaitingTime_Top / AverageWaitingTime_Counter;

        TotalSystemTime = CurrentCustomer.GetDepartureTime();

        AverageServicesTime_Counter++;

        CustomersQueue.pop();
    }

public:
    vector<string> CustomSplit(string str, char separator) {
        vector<string> strings;
        int startIndex = 0, endIndex = 0;
        for (int i = 0; i <= str.size(); i++) {
            if (str[i] == separator || i == str.size()) {
                endIndex = i;
                string temp;
                temp.append(str, startIndex, endIndex - startIndex);
                strings.push_back(temp);
                startIndex = endIndex + 1;
            }
        }
        return strings;
    }

    void EmptyBank() {
        while (!CustomersQueue.empty()) {
            Customer CurrentCustomer = CustomersQueue.top();

            int TellerIndex = GetIndexOfNearestFreeTeller();

            TellersVector.at(TellerIndex).SetCurrentCustomer(&CurrentCustomer);

            TellersVector.at(TellerIndex).ServeCustomer();

            TotalSystemTime = CurrentCustomer.GetDepartureTime();
            TotalServicesTime += CurrentCustomer.GetServiceDuration();

            CustomersQueue.pop();
        }

        outFileActions << "Simulation Has been Ended!" << endl;

        outFileStatistics << "Average Queue Length: " << AverageQueueLength << endl;
        outFileStatistics << "Average Waiting Time: " << AverageWaitingTime << endl;
        outFileStatistics << "Total System Time: " << TotalSystemTime << endl;
        double TellerUtilization =
            (NumberOfTellers * TotalSystemTime) == 0
            ? 0
            : TotalServicesTime / (NumberOfTellers * TotalSystemTime);
        outFileStatistics << "Average Service Time: "
            << TotalSystemTime / AverageServicesTime_Counter << endl;
        outFileStatistics << "Teller Utilization: " << TellerUtilization << endl;

        OutFile.close();
        outFileActions.close();
        outFileStatistics.close();
    }

    Bank(int TellersCount, int _MinServiceDuration, int _MaxServiceDuration,
        int _AddExtraTellerThresholdPerCustomers,
        int _RemoveExceededTellerThreshold) {

        InitialTellerCount = TellersCount;

        MinServiceDuration = _MinServiceDuration;
        MaxServiceDuration = _MaxServiceDuration;
        AddExtraTellerThresholdPerCustomers = _AddExtraTellerThresholdPerCustomers;
        RemoveExceededTellerThreshold = _RemoveExceededTellerThreshold;

        OutFile.open("IO Files/output.csv");
        outFileActions.open("IO Files/actions.txt");
        outFileStatistics.open("IO Files/statistics.txt");

        outFileActions << "Simulation Has been Started!" << endl;

        for (int i = 0; i < TellersCount; i++) {
            TellersVector.push_back(Teller(&OutFile));
            outFileActions << "* INIT :: Teller with ID (" << TellersVector[i].GetID()
                << ") was added as initial teller." << endl;
        }

        LastCustomerArrival = 0;
        RemoveExceededTellerCounter = 0;

        OutFile << "Customer ID,Teller ID,Priority,Arrival Time,Service "
            "Duration,Waiting Time,Served Time,Departure Time"
            << endl;

        AverageQueueLength = 1;
        AverageQueueLength_Counter = 1;

        AverageWaitingTime = 1;
        AverageWaitingTime_Counter = 1;

        TotalSystemTime = 0;
        NumberOfTellers = 0;
        TotalServicesTime = 0;

        AverageServicesTime_Counter = 1;
    }

    void Arrival(string ArrivalTime, int Priority) {
        int seconds = ConvertTimeToSecond(ArrivalTime);

        if (!ValidateArrivalTime(seconds)) {
            cout << "ERROR: New arrival is earlier than last arrival : " <<LastCustomerArrival << endl;
            return;
        }

        LastCustomerArrival = seconds;

        Customer ArrivedCustomer =
            Customer(seconds, Priority, GenerateRandomServiceDuration());

        CustomersQueue.push(ArrivedCustomer);

        double AverageQueueLength_Top =
            (AverageQueueLength * AverageQueueLength_Counter) +
            CustomersQueue.size();
        AverageQueueLength_Counter++;
        AverageQueueLength = AverageQueueLength_Top / AverageQueueLength_Counter;

        CheckIfAddExtraTeller();

        CheckIfRemoveExceededTeller();

        ServeNextCustomerInQueue();
    }
};

