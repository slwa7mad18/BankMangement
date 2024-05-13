#include <iostream>
#include<string>
#include <conio.h>
#include <queue>
#include <vector>
#include "Customer.h"
#include "Bank.h"
#include "Teller.h"
#include <cctype>
#include <windows.h>


using namespace std;

int Customer::Counter = 0;
int Teller::Counter = 0;
int getIntFromUser();

int main()
{
    // Get Configurations from User
    int TellersCount;
    system("color f5");
    cout << "Enter initial number of tellers: ";
    
    TellersCount = getIntFromUser();
    cout << endl;

    int AddExtraTellerThresholdPerCustomers;
    cout << "Enter the threshold to add new teller: ";
    AddExtraTellerThresholdPerCustomers = getIntFromUser();;
    cout << endl;

    int RemoveExceededTellerThreshold;
    cout << "Enter the threshold to remove exceeded teller: ";
    RemoveExceededTellerThreshold = getIntFromUser();
    cout << endl;

    int MinServiceDuration=1, MaxServiceDuration=0;
    while (MinServiceDuration > MaxServiceDuration)
    {
        cout << "Enter Minimum Service Duration: ";
        MinServiceDuration = getIntFromUser();
        cout << endl;
        cout << "Enter Maximum Service Duration: ";
        MaxServiceDuration = getIntFromUser();
        cout << endl;
        if (MinServiceDuration > MaxServiceDuration)
        {
            cout << "Error !! Minimum Service Duration Must be smaller than Maximum Service Duration..\n" << endl;
        }
    }

    Bank bank(TellersCount, MinServiceDuration, MaxServiceDuration,
        AddExtraTellerThresholdPerCustomers, RemoveExceededTellerThreshold);

    bool ReadFromFileFlag;
    char ReadFromFileUserInput;
    cout << "Do you want to read from file? [y/n] ";
    cin >> ReadFromFileUserInput;
    cout << endl;
    switch ((char)tolower(ReadFromFileUserInput)) {
    case 'y':
        ReadFromFileFlag = true;
        break;
    default:
        ReadFromFileFlag = false;
        break;
    }

    if (ReadFromFileFlag) {
        string Line;
        ifstream InFile("IO Files/input.csv");
        if (InFile.is_open()) {
            while (getline(InFile, Line)) {
                vector<string> LineSplitted = bank.CustomSplit(Line, ',');
                bank.Arrival(LineSplitted[0], stoi(LineSplitted[1]));
            }
            InFile.close();
        }
        else {
            cout << "ERROR: Unable to open file.\n";
        }
    }

    bool EnterNewCustomerFlag;
    char EnterNewCustomerUserInput;
    string ArrivalTime;
    int Priority;
    while (true) {
        cout << "Do you want to enter new customer? [y/n] ";
        cin >> EnterNewCustomerUserInput;
        cout << endl;
        switch ((char)tolower(EnterNewCustomerUserInput)) {
        case 'y':
            EnterNewCustomerFlag = true;
            break;
        case 'n':
            EnterNewCustomerFlag = false;
            break;
        default:
            continue;
        }

        if (!EnterNewCustomerFlag) {
            bank.EmptyBank();
            break;
        }
        else {
            cout << "Enter Arrival Time (hh:mm:ss): ";
            cin >> ArrivalTime;
            cout << endl;

            cout << "Enter Priority (0 for Regular, 1 for VIP): ";
            Priority = getIntFromUser();
            cout << endl;

            bank.Arrival(ArrivalTime, Priority);
        }
    }
}

int getIntFromUser()
{
    int option;
    bool flag = true;
    cin.exceptions(std::istream::failbit);
    do {
        try {

            cin >> option;
            if (option<0)
            {
                throw  std::ios_base::failure("");
            }
            flag = true;
        }
        catch (std::ios_base::failure& fail) {
            flag = false;
            cout << "Error !! Please enter a valid Intger number : ";
            cin.clear();
            std::string tmp;
            getline(cin, tmp);
        }
    } while (flag == false);
    
    return option;
}