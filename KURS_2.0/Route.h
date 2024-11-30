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
    string routeName; //название

    string departureTime; //время отправления
    string arrivalTime; //время прибытия
    vector<Stop> stops; //остановки
    vector<Vagon> vagons; //вагоны
    string trainName;

public:
    int routeNumber; //номер

    Route(RouteType routeType, TrainType trainType, const string& routeName, int routeNumber, const string& departureTime,
        const string& arrivalTime, const string& trainName)
        : routeName(routeName), routeNumber(routeNumber),
        departureTime(departureTime), arrivalTime(arrivalTime), trainName(trainName) {}

    Route() : routeType(RouteType::CITYLINES), trainType(TrainType::ELECTRIC),
        routeName(""), routeNumber(0), departureTime(""), arrivalTime(""),
        trainName("") {}

    ~Route() {

    }

    void addStop(const Stop& stop);

    void addVagon(const Vagon& vagon);

    const string& getRouteName();


    int getRouteNumber();

    const vector<Stop>& getStops();

    bool passesThrough(const string& stationName);

    char inputDay();

    bool isDayTrue(Stop stop);

    bool passesThroughForDay(const string& stationName);

    void printRouteType(RouteType routeType);

    void printTrainType(TrainType trainType);

    void printVagonType(TypeVagon typeVagon);

    void printRouteShortInfo();

    void printRouteInfo();

    void save(ofstream& ofs);

    void load(ifstream& ifs);
};
