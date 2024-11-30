#include <iostream>
#include <vector>
#include <string>
#include <locale>
#include <string.h>
#include <Windows.h>
#include <fstream>
using namespace std;

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

class InvalidStopChoiceException : public runtime_error { //��������� ����������
public:
    InvalidStopChoiceException(const string& message) : runtime_error(message) {}
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
        : name(name), arrivalTime(arrivalTime), departureTime(departureTime), daysOfTheWeek(daysOfTheWeek) {}

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

class MainObject {
public:
    virtual ~MainObject() {}
    virtual void displayInfo() const = 0; // ����� ����������� �������
};

class City : public MainObject {
public:
    string cityName;
    vector<Stop> stops; // ������ ���������

    City(const string& name) : cityName(name) {}
    
    void addStopForCity(const Stop& stop) {
        stops.push_back(stop);
    }

    void displayInfo() const override {
        cout << "�����: " << cityName << endl;
        cout << "���������:" << endl;
        for (const auto& stop : stops) {
            cout << "- " << stop.name << endl;
        }
    }
};

class User : public MainObject {
public:
    string userName;

    User(const string& name) : userName(name) {}
    virtual ~User() {}

    void displayInfo() const override {
        cout << userName << endl;
    }
};

class AdminUser : public User {
private:
    int key; // ���� �������

public:
    AdminUser(const string& name, int key) : User(name), key(key) {}

    AdminUser() : User("������"), key(12345) {}

    int enterKey() const {
        int inputtingKey, attempt = 3; 
        do {
            cout << "������� ���� �������. ���� 3 �������\n";
            cin >> inputtingKey;
            if (inputtingKey != key)
            {
                cout << "�������� ���� �������, ��������� �������";
                attempt--;
            }
        } while (inputtingKey != key && attempt!=0);

        if (attempt == 0)
        {
            cout << "������ ��� �������. ����������� � ������� ����\n";
            return 1;
        }
        else
        {
            cout << "���� ������...����\n";
            return 0;
        }
    }

    void displayInfo() const override {
        User::displayInfo();
    }
};

class SimpleUser : public User {
public:
    string cityOfResidence;

    SimpleUser(const string& name, const string& cityOfResidence) : User(name), cityOfResidence(cityOfResidence) {}

    SimpleUser() : User("������������") {}

    void displayInfo() const override {
        User::displayInfo();
        cout << "\n��� ����� - " << cityOfResidence << "\n";
    }
};


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

    string getName() const { 
        return routeName;
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
    
    char inputDay() const{
        char day;
        cout << "������� ����: \n1- ����������� \n2- ������� \n3- ����� \n4- ������� \n5- ������� \n6- ������� \n7- �����������\n";
        do {
            cin >> day;
            if (!(day == '1' || day == '2' || day == '3' || day == '4' || day == '5' || day == '6' || day == '7'))
                cout << "������������ ����. ������� ����� �� 1 �� 7";
        } while(!(day == '1' || day == '2' || day == '3' || day == '4' || day == '5' || day == '6' || day == '7'));
        
        return day;
    }

    bool isDayTrue(Stop stop) const {
        char inputtingDay = inputDay();
        int flag = 0;
        for (int i = 0; stop.daysOfTheWeek[i] != '\0'; i++)
        {
            if (inputtingDay == stop.daysOfTheWeek[i])
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

    void printRouteShortInfo(string stopName) const {//����� �������� ������� ���������� ��� ����������������� ����
        printRouteType(routeType);
        cout << "\n" << routeNumber << "--" << routeName;
        for (const auto& stop : stops) {
            if (stop.name == stopName) {
                cout  << " || ����:" << stop.departureTime <<  "����:" << stop.arrivalTime << "";
            }
        }
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

void addRoute(vector<Route>& routes) {
    string routeName, departureTime, arrivalTime, trainName;
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

    int addMoreStops;
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

        cout << "�������� ��� ���������? (1 - ��/0 - ���): ";
        cin >> addMoreStops;
    } while (addMoreStops == 1);

    int addMoreVagons;
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

        cout << "�������� ��� �����? (1 - ��/0 - ���): ";
        cin >> addMoreVagons;
    } while (addMoreVagons == 1);

    routes.push_back(newRoute);
}

void addCity(vector<City>& �ities) {
    string cityName;
    cout << "������� �������� ������\n";
    cin >> cityName;
    City newCity(cityName);
    int addMoreStops = 0;
    do {
        string stopName, arrivalTime="", departureTime="", daysOfTheWeek="";
        cout << "������� �������� ��������� � ���� ������\n";
        cin >> stopName;
        newCity.addStopForCity(Stop(stopName, arrivalTime, departureTime, daysOfTheWeek));
        cout << "������ �������� ��� ���������? (1 - ��, 0 - ���)\n";
        cin >> addMoreStops;
    } while (addMoreStops == 1);
    �ities.push_back(newCity);
}

void initializeUser(SimpleUser& simpleUser) {
    cout << "������� ��� �����. ����� �������� ������ ";
    cin >> simpleUser.cityOfResidence;
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

void bubbleSort(vector<Route>& routes) {  //1 ���� �����
    bool swapped;
    int n = routes.size();

    do {
        swapped = false;
        for (int i = 0; i < n - 1; ++i) {
            if (routes[i].getRouteNumber() > routes[i + 1].getRouteNumber()) {
                swap(routes[i], routes[i + 1]);
                swapped = true;
            }
        }
        --n; // ��������� n, ��� ��� ��������� ������� ��� ������������
    } while (swapped);
}

void viewRoutesAtStation(const vector<Route>& routes, string stationToCheck) {
    
    vector<Route> routesToShow; // ������ ��������� ��������� 
   
    bool isAllDaysOrForOneDay = chooseTypeOfFinding(); // 0 - ���������� ����, 1 - ��� ��� 

    bool found = false;
    int count = 1;
    if (isAllDaysOrForOneDay) {
        for (const auto& route : routes) {
            if (route.passesThrough(stationToCheck)) {
                cout << "�������" << count;
                route.printRouteShortInfo(stationToCheck); // ����� ������� ����������
                routesToShow.push_back(route); // ���������� � ��������� ������
                cout << "\n";
                found = true;
                count++;
            }
            
        }
    }
    else {
        for (const auto& route : routes) {
            if (route.passesThroughForDay(stationToCheck)) {
                cout << "�������" << count;
                route.printRouteShortInfo(stationToCheck); // ����� ������� ����������
                routesToShow.push_back(route); // ���������� � ��������� ������
                cout << "\n";
                found = true;
                count++;
            }
            
        }
    }

    if (!found) {
        cout << "��� ��������� �� ���� ������� " << stationToCheck << ".\n";
    }
    else {
        // ������������� ������������ ����������� ������� ������� ��� ��������� ��������� ����������
        int choice;
        cout << "�������� ����� �������� ��� ��������� ��������� ���������� (0 - " << (routesToShow.size()-1) << "): ";
        cin >> choice;

        if (choice >= 0 && choice < routesToShow.size()) {
            routesToShow[choice].printRouteInfo(); // ����� ��������� ���������� � ��������� ��������
        }
        else {
            cout << "������������ ����� ��������.\n";
        }
    }
}

void viewRoutesAtCity(const vector<Route>& routes, vector<City>& cities, SimpleUser simpleUser) {
    string inputtingCity;
    int choise;
    cout << "\n������ �� �������� ������ ������ - 0 \n�� �������� ���������� ������ - 1";
    cin >> choise;

    if (choise == 0) {
        inputtingCity = simpleUser.cityOfResidence;
        simpleUser.displayInfo();
    }
    else {
        cout << "������� �����: ";
        cin >> inputtingCity;
    }

    // ������� ����� � ������� cities
    auto cityIt = find_if(cities.begin(), cities.end(), [&inputtingCity](const City& city) {
        return city.cityName == inputtingCity;
        });

    if (cityIt != cities.end()) {
        cout << "����� ������: " << cityIt->cityName << endl;

        // ������ ��������� ��� ���������� ������
        vector<Stop> foundStops = cityIt->stops;

        // ������� ������ ���������
        cout << "������ ��������� � ������ " << cityIt->cityName << ":\n";
        for (size_t i = 0; i < foundStops.size(); ++i) {
            cout << i + 1 << ". " << foundStops[i].name << endl;
        }

        // ������������ �������� ���������
        int stopChoice;
        cout << "�������� ��������� (������� �����): ";
        cin >> stopChoice;

        try {
            // ��������� ���������� ������ �� �������
            if (stopChoice < 1 || stopChoice > foundStops.size()) {
                throw InvalidStopChoiceException("������: �������� ����� ���������.");
            }

            // ����� ������� ������ �� �������
            string selectedStop = foundStops[stopChoice - 1].name;
            viewRoutesAtStation(routes, selectedStop);

        }
        catch (const InvalidStopChoiceException& e) {
            cout << e.what() << endl; // ������� ��������� �� ������
        }

    }
    else {
        cout << "����� �� ������." << endl;
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

void chooseOptionForAdmin(vector<Route>& routes, vector<City> cities) //2 ���� �����
{
    int choise;
    do {
        cout << "�������� ��������:\n";
        cout << "1 - �������� ������� � ����\n";
        cout << "2 - ������� ������� �� ������:\n";
        cout << "3 - �������� �����:\n";
        cout << "4 - �����\n";
        cin >> choise;

        switch (choise) {
        case 1:
            addRoute(routes);
            bubbleSort(routes);
            break;
        case 2:
            int trainNumber;
            cout << "�������� �������� �� ������ ��������. ������� ����� �������� ��� ��������: ";
            cin >> trainNumber;
            deleteRouteByTrainNumber(routes, trainNumber);
            break;
        case 3: 
            addCity(cities);
            break;
        case 4:
            cout << "����������� � ����\n";
            break;
        default:
            cout << "������� ����� �� 1 �� 4.\n";
            break;
        }
    } while (choise != 4);
}





int main() { 
    vector<Route> routes; //������ ���������
    AdminUser adminUser;
    SimpleUser simpleUser;
    vector<City> cities;

    setlocale(LC_ALL, "");
    cout << "��������...\n";
    loadRoutes(routes, "C:/Routes1/Routes.txt");
    cout << "����� � ������\n";
    int typeOfUser;
    do {
        cout << "����� ���:\n";
        cout << "������������� (1)\n";
        cout << "������������(2):\n";
        cout << "����� (3)\n";
        cin >> typeOfUser;
        int choise = 0;
        switch (typeOfUser) {
        case 1:
            if (adminUser.enterKey() == 0) {
                cout << "����� ����������, ";
                adminUser.displayInfo();
                chooseOptionForAdmin(routes, cities);
                }
            else
                cout << "������ �����\n";
            break;
        case 2:
            initializeUser(simpleUser);
            simpleUser.displayInfo();
            cout << "������� 0 - ������ �� �������\n1- ������ �� ������ ";
            cin >> choise;
            if (choise == 0)
            {
                string stationToCheck;
                cout << "������� ������� ��� ������: ";
                cin >> stationToCheck;
                viewRoutesAtStation(routes, stationToCheck);
            }
            else
                viewRoutesAtCity(routes, cities, simpleUser);
            break;
        case 3:
            cout << "����������...\n";
            saveRoutes(routes, "C:/Routes1/Routes.txt");
            cout << "�����.\n";
            break;
        default:
            cout << "������� ����� �� 1 �� 3.\n";
            break;
        }

        cout << "\n"; // ��� ������ ����������
    } while (typeOfUser != 3);
    return 0; 
}
