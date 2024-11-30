#include <iostream>
#include <vector>
#include <string>
#include <locale>
#include <string.h>
#include <Windows.h>
#include <fstream>
#include "OtherClasses.h"
#include "Route.h"
using namespace std;

void saveRoutes(const vector<Route>& routes, const string& filename) {
    ofstream ofs(filename);
    if (!ofs.is_open()) {
        cerr << "������ �������� ����� ��� ������!" << endl;
        return;
    }

    ofs << routes.size() << "\n"; // ��������� ���������� ���������
    for (const auto& route : routes) {
        route.save(ofs);
    }

    ofs.close();
}

void loadRoutes(vector<Route>& routes, const string& filename) {
    ifstream ifs(filename);
    if (!ifs.is_open()) {
        cerr << "������ �������� ����� ��� ������!" << endl;
        return;
    }

    size_t routesCount;
    ifs >> routesCount;
    routes.resize(routesCount);

    for (size_t i = 0; i < routesCount; ++i) {
        routes[i].load(ifs);
    }

    ifs.close();
}

RouteType chooseRoute()
{
    RouteType routeType;
    int choise = 1;

    cout << "�������� ��� ��������\n";
    cout << "1 - ��������� �����\n";
    cout << "2 - ������������ ����� ������������\n";
    cout << "3 - ������������ ����� ������ ������\n";
    cout << "4 - ��������������� ����� ������������\n";
    cout << "5 - ��������������� ����� ������ ������\n";
    cout << "6 - ������������� �����\n";

    do
    {
        cin >> choise;
        if (choise < 1 || choise > 6)
            cout << "������������ ����, ����� ������ ����� �� 1 �� 6\n";
    } while (choise < 1 || choise > 6);

    switch (choise)
    {
    case 1:
        routeType = RouteType::CITYLINES;
        break;
    case 2:
        routeType = RouteType::REGIONALLINESECONOMY;
        break;
    case 3:
        routeType = RouteType::REGIONALLINESBIZNES;
        break;
    case 4:
        routeType = RouteType::INTERCITYLINESECONOMY;
        break;
    case 5:
        routeType = RouteType::INTERCITYLINESBIZNES;
        break;
    case 6:
        routeType = RouteType::INTERCOUNTRIES;
        break;
    }
    return routeType;
}

TrainType chooseTrainType() {
    TrainType trainType;
    int choise = 1;

    cout << "�������� ��� ���� ������\n";
    cout << "1 - �������������\n";
    cout << "2 - ���������\n";
    cout << "3 - ������ (����� + ���������)\n";

    do
    {
        cin >> choise;
        if (choise < 1 || choise > 3)
            cout << "������������ ����, ����� ������ ����� �� 1 �� 3\n";
    } while (choise < 1 || choise > 3);

    switch (choise)
    {
    case 1:
        trainType = TrainType::ELECTRIC;
        break;
    case 2:
        trainType = TrainType::DIESEL;
        break;
    case 3:
        trainType = TrainType::OTHER;
        break;
    }
    return trainType;
}

TypeVagon ChooseTypeOfVagon() {
    TypeVagon VagonType;
    int choise = 1;

    cout << "�������� ��� ������\n";
    cout << "1 - ��������\n";
    cout << "2 - ����\n";
    cout << "3 - ������� (��� ������� � ���������� ������ ���)\n";
    cout << "4 - �����\n";
    cout << "5 - ��\n";

    do
    {
        cin >> choise;
        if (choise < 1 || choise > 5)
            cout << "������������ ����, ����� ������ ����� �� 1 �� 5\n";
    } while (choise < 1 || choise > 5);

    switch (choise)
    {
    case 1:
        VagonType = TypeVagon::PLAC;
        break;
    case 2:
        VagonType = TypeVagon::KUPE;
        break;
    case 3:
        VagonType = TypeVagon::SEAT;
        break;
    case 4:
        VagonType = TypeVagon::MAIN;
        break;
    case 5:
        VagonType = TypeVagon::SV;
        break;
    }
    return VagonType;
}

void addRoute(std::vector<Route>& routes) {
    std::string routeName, departureTime, arrivalTime, trainName;
    int routeNumber;
    RouteType routeType = chooseRoute();
    TrainType trainType = chooseTrainType();
    cout << "������� �������� �������� (<�������1> - <�������2>): ";
    cin >> routeName;
    cout << "������� ����� ��������: ";
    cin >> routeNumber;
    cout << "������� ����� ����������� �� ������ ������� (HH:MM): ";
    cin >> departureTime;
    cout << "������� ����� �������� �� �������� ������� (HH:MM): ";
    cin >> arrivalTime;
    cout << "������� �������� ������: ";
    cin >> trainName;

    Route newRoute(routeType, trainType, routeName, routeNumber, departureTime, arrivalTime, trainName);

    char addMoreStops;
    do {
        string stopName, arrivalTime, departureTime, daysOfTheWeek;
        cout << "������� �������� �������: ";
        cin >> stopName;
        cout << "������� ����� �������� �� �������. ���� ������� ������ - ��������������� ����� ������ ������ (HH:MM): ";
        cin >> arrivalTime;
        cout << "������� ����� ����������� �� �������. ���� ��� ��������� - ��������������� ����� ���������� �������� ������� ���������� (HH:MM): ";
        cin >> departureTime;
        cout << "������� ��� ������, ����� ���� ��������� � �������: 1234567 (������ ����), 23 (�� ��������� � ������)  ";
        cin >> daysOfTheWeek; //�������� ��������� ������������ ������, ������� ������������ � ���� ���� � ��������� � ������, � ����� ���� ����� ������������

        newRoute.addStop(Stop(stopName, arrivalTime, departureTime, daysOfTheWeek));

        cout << "Add another stop? (y/n): ";
        cin >> addMoreStops;
    } while (addMoreStops == 'y' || addMoreStops == 'Y');

    char addMoreVagons;
    do {
        int number, numberOfSeats;
        string description;
        TypeVagon typeVagon = ChooseTypeOfVagon();
        cout << "������� ����� ������: ";
        cin >> number;
        cout << "������� ���������� ����: ";
        cin >> numberOfSeats;
        cout << "������� �������� ������: ";
        cin >> description;
        newRoute.addVagon(Vagon(number, typeVagon, numberOfSeats, description));

        cout << "�������� ��� �����? (y/n): ";
        cin >> addMoreVagons;
    } while (addMoreVagons == 'y' || addMoreVagons == 'Y');

    routes.push_back(newRoute);
}

bool chooseTypeOfFinding()
{
    int choise;
    cout << "0 - ������ ��� ����������� ���\n";
    cout << "1 - ������ �� ��� ���\n";
    do
    {
        cin >> choise;
        if (choise < 0 || choise > 1)
            cout << "������������ ����, ����� ������ 0 ��� 1\n";
    } while (choise < 0 || choise > 1);

    switch (choise)
    {
    case 0:
        return 0;
        break;
    case 1:
        return 1;
        break;
    }

}

void bubbleSort(vector<Route>& routes) {
    bool swapped;
    int n = routes.size();

    do {
        swapped = false;
        for (int i = 0; i < n - 1; ++i) {
            if (routes[i].getRouteNumber() > routes[i + 1].getRouteNumber()) {
                std::swap(routes[i], routes[i + 1]);
                swapped = true;
            }
        }
        --n; // ��������� n, ��� ��� ��������� ������� ��� ������������
    } while (swapped);
}

using namespace std;

void viewRoutesAtStation(const std::vector<Route>& routes) {
    string stationToCheck;
    vector<Route> routesToShow; // ������ ��������� ��������� 
    cout << "������� ������� ��� ������: ";
    cin >> stationToCheck;
    bool isAllDaysOrForOneDay = chooseTypeOfFinding(); // 0 - ���������� ����, 1 - ��� ��� 

    bool found = false;
    int count = 1;
    if (isAllDaysOrForOneDay) {
        for (const auto& route : routes) {
            if (route.passesThrough(stationToCheck)) {
                cout << "�������" << count;
                route.printRouteShortInfo(); // ����� ������� ����������
                routesToShow.push_back(route); // ���������� � ��������� ������
                std::cout << "\n";
                found = true;
            }
            count++;
        }
    }
    else {
        for (const auto& route : routes) {
            if (route.passesThroughForDay(stationToCheck)) {
                cout << "�������" << count;
                route.printRouteShortInfo(); // ����� ������� ����������
                routesToShow.push_back(route); // ���������� � ��������� ������
                std::cout << "\n";
                found = true;
            }
            count++;
        }
    }

    if (!found) {
        cout << "No routes found passing through " << stationToCheck << ".\n";
    }
    else {
        // ������������� ������������ ����������� ������� ������� ��� ��������� ��������� ����������
        int choice;
        cout << "�������� ����� �������� ��� ��������� ��������� ���������� (0 - " << (routesToShow.size() - 1) << "): ";
        cin >> choice;

        if (choice >= 0 && choice < routesToShow.size()) {
            routesToShow[choice].printRouteInfo(); // ����� ��������� ���������� � ��������� ��������
        }
        else {
            cout << "������������ ����� ��������.\n";
        }
    }
}

void deleteRouteByTrainNumber(vector<Route>& routes, int trainNumber) {
    auto it = remove_if(routes.begin(), routes.end(), [trainNumber](const Route& route) {
        return route.routeNumber == trainNumber;
        });

    if (it != routes.end()) {
        routes.erase(it, routes.end());
        cout << "������� � ������� ������ " << trainNumber << " ������� ������." << endl;
    }
    else {
        cout << "������� � ������� ������ " << trainNumber << " �� ������." << endl;
    }
}

void chooseOptionForAdmin(vector<Route>& routes)
{
    int choise;
    do {
        cout << "�������� ��������:\n";
        cout << "1 - �������� ������� � ����\n";
        cout << "2 - ������� ������� �� ������:\n";
        cout << "3 - �����\n";
        cin >> choise;

        switch (choise) {
        case 1:
            addRoute(routes);
            bubbleSort(routes);
            break;
        case 2:
            int trainNumber;
            cout << "������� ����� �������� ��� ��������: ";
            cin >> trainNumber;
            deleteRouteByTrainNumber(routes, trainNumber);
            break;
        case 3:
            cout << "����������� � ����\n";
            break;
        default:
            cout << "������� ����� �� 1 �� 3.\n";
            break;
        }
    } while (choise != 3);
}
