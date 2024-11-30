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
    PLAC, //�����������
    KUPE, //��������
    SV, //��
    MAIN, //�����
    SEAT //�������
};

enum class RouteType {
    CITYLINES, //��������� �����
    REGIONALLINESECONOMY, //������������ ����� ������������
    REGIONALLINESBIZNES, //������������ ����� ������ ������
    INTERCITYLINESECONOMY, //��������������� ����� ������������
    INTERCITYLINESBIZNES,  //��������������� ����� ������ ������
    INTERCOUNTRIES //������������� �����
};

enum class TrainType { //��� ���� ������
    ELECTRIC, //����������
    DIESEL, //������
    OTHER //� ��� ��� �����...
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

    void save(ofstream& ofs) const {
        ofs << number << "\n"
            << static_cast<int>(typeVagon) << "\n"
            << numberOfSeats << "\n"
            << description << "\n";
    }

    void load(ifstream& ifs) {
        ifs >> number;
        int type;
        ifs >> type;
        typeVagon = static_cast<TypeVagon>(type);
        ifs >> numberOfSeats;
        ifs.ignore(); // ���������� ������ ����� ������ ����� �����
        getline(ifs, description);
    }
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

    void save(ofstream& ofs) const {
        ofs << name << "\n" << arrivalTime << "\n" << departureTime << "\n" << daysOfTheWeek << "\n";
    }

    void load(ifstream& ifs) {
        getline(ifs, name);
        getline(ifs, arrivalTime);
        getline(ifs, departureTime);
        getline(ifs, daysOfTheWeek);
    }
};