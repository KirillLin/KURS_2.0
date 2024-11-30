#include <iostream>
#include <vector>
#include <string>
#include <locale>
#include <string.h>
#include <Windows.h>
#include <fstream>
using namespace std;

enum class DayOfWeek {
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
    SUNDAY
};

enum class TypeVagon {
    PLAC, //Плацкартный
    KUPE, //Купейный
    SV, //СВ
    MAIN, //Общий
    SEAT //Сидячий
};

enum class RouteType {
    CITYLINES, //Городские линии
    REGIONALLINESECONOMY, //Региональные линии экономкласса
    REGIONALLINESBIZNES, //Региональные линии бизнес класса
    INTERCITYLINESECONOMY, //Межрегиональные линии экономкласса
    INTERCITYLINESBIZNES,  //Межрегиональные линии бизнес класса
    INTERCOUNTRIES //Международные линии
};

enum class TrainType { //тип тяги поезда
    ELECTRIC, //Электричка
    DIESEL, //Дизель
    OTHER //А кто его знает...
};

class Vagon {
public:
    int number;
    TypeVagon typeVagon;
    int numberOfSeats;
    string description;

    Vagon(int number, TypeVagon typeVagon, int numberOfSeats, const string description)
        : number(number), numberOfSeats(numberOfSeats), description(description) {}

    Vagon() : number(0), typeVagon(TypeVagon::PLAC), numberOfSeats(0), description("") {}

    ~Vagon() {

    }

    void save(ofstream& ofs);

    void load(ifstream& ifs);
};

class Stop {
public:
    string name;
    string arrivalTime;
    string departureTime;
    string daysOfTheWeek;


    Stop(const string& name, const string& arrivalTime, const string& departureTime, const string daysOfTheWeek)
        : name(name), arrivalTime(arrivalTime), departureTime(departureTime) {}

    Stop() : name(""), arrivalTime(""), departureTime(""), daysOfTheWeek("") {}

    ~Stop() {

    }

    void save(ofstream& ofs);

    void load(ifstream& ifs);
};
