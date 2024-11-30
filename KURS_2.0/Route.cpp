#include <iostream>
#include <vector>
#include <string>
#include <locale>
#include <string.h>
#include <Windows.h>
#include <fstream>
#include "OtherClasses.h"

using namespace std;

class Route {
private:
    RouteType routeType;
    TrainType trainType;
    string routeName; //��������

    string departureTime; //����� �����������
    string arrivalTime; //����� ��������
    vector<Stop> stops; //���������
    vector<Vagon> vagons; //������
    string trainName;

public:
    int routeNumber; //�����

    Route(RouteType routeType, TrainType trainType, const string& routeName, int routeNumber, const string& departureTime,
        const string& arrivalTime, const string& trainName)
        : routeName(routeName), routeNumber(routeNumber),
        departureTime(departureTime), arrivalTime(arrivalTime), trainName(trainName) {}

    Route() : routeType(RouteType::CITYLINES), trainType(TrainType::ELECTRIC),
        routeName(""), routeNumber(0), departureTime(""), arrivalTime(""),
        trainName("") {}

    ~Route() {

    }

    void addStop(const Stop& stop) {
        stops.push_back(stop);
    }

    void addVagon(const Vagon& vagon) {
        vagons.push_back(vagon);
    }

    const string& getRouteName() const {
        return routeName;
    }

    int getRouteNumber() const {
        return routeNumber;
    }

    const vector<Stop>& getStops() const {
        return stops;
    }

    bool passesThrough(const string& stationName) const {
        for (const auto& stop : stops) {
            if (stop.name == stationName) {
                return true;
            }
        }
        return false;
    }

    char inputDay() const {
        char day;
        cout << "������� ����: \n1- ����������� \n2- ������� \n3- ����� \n4- ������� \n5- ������� \n6- ������� \n7- �����������\n";
        do {
            cin >> day;
            if (!(day == '1' || day == '2' || day == '3' || day == '4' || day == '5' || day == '6' || day == '7'))
                cout << "������������ ����. ������� ����� �� 1 �� 7";
        } while (!(day == '1' || day == '2' || day == '3' || day == '4' || day == '5' || day == '6' || day == '7'));

        return day;
    }

    bool isDayTrue(Stop stop) const {
        char inputtingDay = inputDay();
        int flag = 0;
        for (int i = 0; stop.daysOfTheWeek[i] != '\0'; i++)
        {
            if (inputtingDay == stop.daysOfTheWeek[i])//�� �������� ��������� ����� ���!!!
            {
                flag = 1;
                break;
            }
            else
                flag = 0;
        }
        if (flag)
            return true;
        else
            return false;
    }

    bool passesThroughForDay(const string& stationName) const {
        for (const auto& stop : stops) {
            if (stop.name == stationName && (isDayTrue(stop))) { //��������� ������ ���� � ��� ������������
                return true;
            }
        }
        return false;
    }

    void printRouteType(RouteType routeType) const {
        switch (routeType)
        {
        case RouteType::CITYLINES:
            cout << "����� ��������� �����\n";
            break;
        case RouteType::REGIONALLINESECONOMY:
            cout << "����� ������������ ����� ������������\n";
            break;
        case RouteType::REGIONALLINESBIZNES:
            cout << "����� ������������ ����� ������ ������\n";
            break;
        case RouteType::INTERCITYLINESECONOMY:
            cout << "����� ��������������� ����� ������������n";
            break;
        case RouteType::INTERCITYLINESBIZNES:
            cout << "����� ��������������� ����� ������ ������\n";
            break;
        case RouteType::INTERCOUNTRIES:
            cout << "����� ������������� �����\n";
            break;
        }
    }

    void printTrainType(TrainType trainType) const {
        switch (trainType)
        {
        case TrainType::ELECTRIC:
            cout << "����������\n";
            break;
        case TrainType::DIESEL:
            cout << "������ �����\n";
            break;
        case TrainType::OTHER:
            cout << "�����\n";
            break;
        }
    }

    void printVagonType(TypeVagon typeVagon) const {
        switch (typeVagon)
        {
        case TypeVagon::PLAC:
            cout << "����������� � ";
            break;
        case TypeVagon::KUPE:
            cout << "�������� � ";
            break;
        case TypeVagon::SEAT:
            cout << "������� � ";
            break;
        case TypeVagon::MAIN:
            cout << "����� � ";
            break;
        case TypeVagon::SV:
            cout << "�� � ";
            break;
        }
    }

    void printRouteShortInfo() const {//����� �������� ������� ���������� ��� ����������������� ����

    }

    void printRouteInfo() const { //����� �������� ��� ���������� �� ���������� ������������
        printRouteType(routeType);
        cout << "��������: " << routeName << "\n"
            << "����� �: " << routeNumber << "\n"
            << "����� ������������: " << departureTime
            << " - " << arrivalTime << "\n";
        printTrainType(trainType);
        cout << trainName << "\n"
            << "��������� � �������:\n";
        int i = 0;
        for (const auto& stop : stops) {

            cout << "  - " << stop.name
                << " (: " << stop.arrivalTime
                << " - " << stop.departureTime << ")\n";
            i++;
        }
        cout << "����� ��������� " << i;
        i = 0;
        for (const auto& vagon : vagons) {
            cout << "\n�����";
            printVagonType(vagon.typeVagon);
            cout << "1 - " << vagon.number;
            cout << "\n ����: " << vagon.numberOfSeats
                << "\n ��������: " << vagon.description << ")\n";
            i++;
        }
        cout << "����� ������� " << i;

    }

    void save(ofstream& ofs) const {
        ofs << static_cast<int>(routeType) << "\n"
            << static_cast<int>(trainType) << "\n"
            << routeName << "\n"
            << routeNumber << "\n"
            << departureTime << "\n"
            << arrivalTime << "\n";

        // ��������� ���������� ���������
        ofs << stops.size() << "\n";
        for (const auto& stop : stops) {
            stop.save(ofs);
        }

        // ��������� ���������� �������
        ofs << vagons.size() << "\n";
        for (const auto& vagon : vagons) {
            vagon.save(ofs);
        }

        ofs << trainName << "\n";
    }

    void load(ifstream& ifs) {
        int type;
        ifs >> type;
        routeType = static_cast<RouteType>(type);

        ifs >> type;
        trainType = static_cast<TrainType>(type);

        ifs.ignore(); // ���������� ������ ����� ������ ����� �����
        getline(ifs, routeName);

        ifs >> routeNumber;
        ifs.ignore(); // ���������� ������ ����� ������ ����� �����
        getline(ifs, departureTime);
        getline(ifs, arrivalTime);

        // ��������� ���������
        size_t stopsCount;
        ifs >> stopsCount;
        stops.resize(stopsCount);
        for (size_t i = 0; i < stopsCount; ++i) {
            stops[i].load(ifs);
        }

        // ��������� ������
        size_t vagonsCount;
        ifs >> vagonsCount;
        vagons.resize(vagonsCount);
        for (size_t i = 0; i < vagonsCount; ++i) {
            vagons[i].load(ifs);
        }

        ifs.ignore(); // ���������� ������ ����� ������ ����� �����
        getline(ifs, trainName);
    }
};