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
        cout << "Введите день: \n1- Понедельник \n2- Вторник \n3- Среда \n4- Четверг \n5- Пятница \n6- Суббота \n7- Воскресенье\n";
        do {
            cin >> day;
            if (!(day == '1' || day == '2' || day == '3' || day == '4' || day == '5' || day == '6' || day == '7'))
                cout << "Неправильный ввод. Введите цифру от 1 до 7";
        } while (!(day == '1' || day == '2' || day == '3' || day == '4' || day == '5' || day == '6' || day == '7'));

        return day;
    }

    bool isDayTrue(Stop stop) const {
        char inputtingDay = inputDay();
        int flag = 0;
        for (int i = 0; stop.daysOfTheWeek[i] != '\0'; i++)
        {
            if (inputtingDay == stop.daysOfTheWeek[i])//не работает исправить через чат!!!
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
            if (stop.name == stationName && (isDayTrue(stop))) { //Совпадает нужный день с днём курсирования
                return true;
            }
        }
        return false;
    }

    void printRouteType(RouteType routeType) const {
        switch (routeType)
        {
        case RouteType::CITYLINES:
            cout << "Поезд городских линий\n";
            break;
        case RouteType::REGIONALLINESECONOMY:
            cout << "Поезд региональных линий экономкласса\n";
            break;
        case RouteType::REGIONALLINESBIZNES:
            cout << "Поезд региональных линий бизнес класса\n";
            break;
        case RouteType::INTERCITYLINESECONOMY:
            cout << "Поезд межрегиональных линий экономклассаn";
            break;
        case RouteType::INTERCITYLINESBIZNES:
            cout << "Поезд межрегиональных линий бизнес класса\n";
            break;
        case RouteType::INTERCOUNTRIES:
            cout << "Поезд международных линий\n";
            break;
        }
    }

    void printTrainType(TrainType trainType) const {
        switch (trainType)
        {
        case TrainType::ELECTRIC:
            cout << "Электричка\n";
            break;
        case TrainType::DIESEL:
            cout << "Дизель поезд\n";
            break;
        case TrainType::OTHER:
            cout << "Поезд\n";
            break;
        }
    }

    void printVagonType(TypeVagon typeVagon) const {
        switch (typeVagon)
        {
        case TypeVagon::PLAC:
            cout << "Плацкартный № ";
            break;
        case TypeVagon::KUPE:
            cout << "Купейный № ";
            break;
        case TypeVagon::SEAT:
            cout << "Сидячий № ";
            break;
        case TypeVagon::MAIN:
            cout << "Общий № ";
            break;
        case TypeVagon::SV:
            cout << "СВ № ";
            break;
        }
    }

    void printRouteShortInfo() const {//Будет выводить краткую информацию для незагромождённости поля

    }

    void printRouteInfo() const { //Будет выводить всю информацию по требованию пользователя
        printRouteType(routeType);
        cout << "НАЗВАНИЕ: " << routeName << "\n"
            << "ПОЕЗД №: " << routeNumber << "\n"
            << "Время курсирования: " << departureTime
            << " - " << arrivalTime << "\n";
        printTrainType(trainType);
        cout << trainName << "\n"
            << "ОСТАНОВКИ И СТАНЦИИ:\n";
        int i = 0;
        for (const auto& stop : stops) {

            cout << "  - " << stop.name
                << " (: " << stop.arrivalTime
                << " - " << stop.departureTime << ")\n";
            i++;
        }
        cout << "Всего остановок " << i;
        i = 0;
        for (const auto& vagon : vagons) {
            cout << "\nВагон";
            printVagonType(vagon.typeVagon);
            cout << "1 - " << vagon.number;
            cout << "\n Мест: " << vagon.numberOfSeats
                << "\n Описание: " << vagon.description << ")\n";
            i++;
        }
        cout << "Всего вагонов " << i;

    }

    void save(ofstream& ofs) const {
        ofs << static_cast<int>(routeType) << "\n"
            << static_cast<int>(trainType) << "\n"
            << routeName << "\n"
            << routeNumber << "\n"
            << departureTime << "\n"
            << arrivalTime << "\n";

        // Сохраняем количество остановок
        ofs << stops.size() << "\n";
        for (const auto& stop : stops) {
            stop.save(ofs);
        }

        // Сохраняем количество вагонов
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

        ifs.ignore(); // Игнорируем символ новой строки после числа
        getline(ifs, routeName);

        ifs >> routeNumber;
        ifs.ignore(); // Игнорируем символ новой строки после числа
        getline(ifs, departureTime);
        getline(ifs, arrivalTime);

        // Загружаем остановки
        size_t stopsCount;
        ifs >> stopsCount;
        stops.resize(stopsCount);
        for (size_t i = 0; i < stopsCount; ++i) {
            stops[i].load(ifs);
        }

        // Загружаем вагоны
        size_t vagonsCount;
        ifs >> vagonsCount;
        vagons.resize(vagonsCount);
        for (size_t i = 0; i < vagonsCount; ++i) {
            vagons[i].load(ifs);
        }

        ifs.ignore(); // Игнорируем символ новой строки после числа
        getline(ifs, trainName);
    }
};