#include <iostream>
#include <vector>
#include <string>
#include <locale>
#include <string.h>
#include <Windows.h>
#include <fstream>
using namespace std;

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

class InvalidStopChoiceException : public runtime_error { //Обработка исключения
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
        ifs.ignore(); // Игнорируем символ новой строки после числа
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
    virtual void displayInfo() const = 0; // Чисто виртуальная функция
};

class City : public MainObject {
public:
    string cityName;
    vector<Stop> stops; // Вектор остановок

    City(const string& name) : cityName(name) {}
    
    void addStopForCity(const Stop& stop) {
        stops.push_back(stop);
    }

    void displayInfo() const override {
        cout << "Город: " << cityName << endl;
        cout << "Остановки:" << endl;
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
    int key; // Ключ доступа

public:
    AdminUser(const string& name, int key) : User(name), key(key) {}

    AdminUser() : User("Кирилл"), key(12345) {}

    int enterKey() const {
        int inputtingKey, attempt = 3; 
        do {
            cout << "Введите ключ доступа. Есть 3 попытки\n";
            cin >> inputtingKey;
            if (inputtingKey != key)
            {
                cout << "Неверный ключ доступа, повторите попытку";
                attempt--;
            }
        } while (inputtingKey != key && attempt!=0);

        if (attempt == 0)
        {
            cout << "Больше нет попыток. Возвращение в главное меню\n";
            return 1;
        }
        else
        {
            cout << "Ключ верный...Вход\n";
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

    SimpleUser() : User("Пользователь") {}

    void displayInfo() const override {
        User::displayInfo();
        cout << "\nВаш город - " << cityOfResidence << "\n";
    }
};


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
        cout << "Введите день: \n1- Понедельник \n2- Вторник \n3- Среда \n4- Четверг \n5- Пятница \n6- Суббота \n7- Воскресенье\n";
        do {
            cin >> day;
            if (!(day == '1' || day == '2' || day == '3' || day == '4' || day == '5' || day == '6' || day == '7'))
                cout << "Неправильный ввод. Введите цифру от 1 до 7";
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

    void printRouteShortInfo(string stopName) const {//Будет выводить краткую информацию для незагромождённости поля
        printRouteType(routeType);
        cout << "\n" << routeNumber << "--" << routeName;
        for (const auto& stop : stops) {
            if (stop.name == stopName) {
                cout  << " || Приб:" << stop.departureTime <<  "Отпр:" << stop.arrivalTime << "";
            }
        }
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

void saveRoutes(const vector<Route>& routes, const string& filename) {
    ofstream ofs(filename);
    if (!ofs.is_open()) {
        cerr << "Ошибка открытия файла для записи!" << endl;
        return;
    }

    ofs << routes.size() << "\n"; // Сохраняем количество маршрутов
    for (const auto& route : routes) {
        route.save(ofs);
    }

    ofs.close();
}

void loadRoutes(vector<Route>& routes, const string& filename) {
    ifstream ifs(filename);
    if (!ifs.is_open()) {
        cerr << "Ошибка открытия файла для чтения!" << endl;
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

    cout << "Выберите тип маршрута\n";
    cout << "1 - Городские линии\n";
    cout << "2 - Региональные линии экономкласса\n";
    cout << "3 - Региональные линии бизнес класса\n";
    cout << "4 - Межрегиональные линии экономкласса\n";
    cout << "5 - Межрегиональные линии бизнес класса\n";
    cout << "6 - Международные линии\n";

    do
    {
        cin >> choise;
        if (choise < 1 || choise > 6)
            cout << "Некорректный ввод, нужно ввести число от 1 до 6\n";
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

    cout << "Выберите тип тяги поезда\n";
    cout << "1 - Электрическая\n";
    cout << "2 - Дизельная\n";
    cout << "3 - Другое (вагон + локомотив)\n";

    do
    {
        cin >> choise;
        if (choise < 1 || choise > 3)
            cout << "Некорректный ввод, нужно ввести число от 1 до 3\n";
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

    cout << "Выберите тип вагона\n";
    cout << "1 - Плацкарт\n";
    cout << "2 - Купе\n";
    cout << "3 - Сидячий (для дизелей и электричек только они)\n";
    cout << "4 - Общий\n";
    cout << "5 - СВ\n";

    do
    {
        cin >> choise;
        if (choise < 1 || choise > 5)
            cout << "Некорректный ввод, нужно ввести число от 1 до 5\n";
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
    cout << "Введите название маршрута (<Станция1> - <Станция2>): ";
    cin >> routeName;
    cout << "Введите номер маршрута: ";
    cin >> routeNumber;
    cout << "Введите время отправления от первой станции (HH:MM): ";
    cin >> departureTime;
    cout << "Введите время прибытия на конечную станцию (HH:MM): ";
    cin >> arrivalTime;
    cout << "Введите название поезда: ";
    cin >> trainName;

    Route newRoute(routeType, trainType, routeName, routeNumber, departureTime, arrivalTime, trainName);

    int addMoreStops;
    do {
        string stopName, arrivalTime, departureTime, daysOfTheWeek;
        cout << "Введите название станции: ";
        cin >> stopName;
        cout << "Введите время прибытия на станцию. Если станция первая - приблизительное время подачи поезда (HH:MM): ";
        cin >> arrivalTime;
        cout << "Введите время отправления со станции. Если она последняя - приблизительное время завершения процесса высадки пассажиров (HH:MM): ";
        cin >> departureTime;
        cout << "Введите дни недели, когда есть остановка в формате: 1234567 (каждый день), 23 (по вторникам и средам)  ";
        cin >> daysOfTheWeek; //Позволит правильно обрабатывать поезда, которые отправляются в один день и прибывают в другой, а также если поезд неежедневный

        newRoute.addStop(Stop(stopName, arrivalTime, departureTime, daysOfTheWeek));

        cout << "Добавить ещё остановку? (1 - да/0 - нет): ";
        cin >> addMoreStops;
    } while (addMoreStops == 1);

    int addMoreVagons;
    do {
        int number, numberOfSeats;
        string description;
        TypeVagon typeVagon = ChooseTypeOfVagon();
        cout << "Введите номер вагона: ";
        cin >> number;
        cout << "Введите количество мест: ";
        cin >> numberOfSeats;
        cout << "Введите описание вагона: ";
        cin >> description;
        newRoute.addVagon(Vagon(number, typeVagon, numberOfSeats, description));

        cout << "Добавить ещё вагон? (1 - да/0 - нет): ";
        cin >> addMoreVagons;
    } while (addMoreVagons == 1);

    routes.push_back(newRoute);
}

void addCity(vector<City>& сities) {
    string cityName;
    cout << "Введите название города\n";
    cin >> cityName;
    City newCity(cityName);
    int addMoreStops = 0;
    do {
        string stopName, arrivalTime="", departureTime="", daysOfTheWeek="";
        cout << "Введите название остановки в этом городе\n";
        cin >> stopName;
        newCity.addStopForCity(Stop(stopName, arrivalTime, departureTime, daysOfTheWeek));
        cout << "Хотите добавить еще остановку? (1 - да, 0 - нет)\n";
        cin >> addMoreStops;
    } while (addMoreStops == 1);
    сities.push_back(newCity);
}

void initializeUser(SimpleUser& simpleUser) {
    cout << "Введите ваш город. Можно оставить пустым ";
    cin >> simpleUser.cityOfResidence;
}

bool chooseTypeOfFinding()
{
    int choise;
    cout << "0 - Искать для конкретного дня\n";
    cout << "1 - Искать на все дни\n";
    do
    {
        cin >> choise;
        if (choise < 0 || choise > 1)
            cout << "Некорректный ввод, нужно ввести 0 или 1\n";
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

void bubbleSort(vector<Route>& routes) {  //1 блок схема
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
        --n; // Уменьшаем n, так как последний элемент уже отсортирован
    } while (swapped);
}

void viewRoutesAtStation(const vector<Route>& routes, string stationToCheck) {
    
    vector<Route> routesToShow; // список найденных маршрутов 
   
    bool isAllDaysOrForOneDay = chooseTypeOfFinding(); // 0 - конкретный день, 1 - все дни 

    bool found = false;
    int count = 1;
    if (isAllDaysOrForOneDay) {
        for (const auto& route : routes) {
            if (route.passesThrough(stationToCheck)) {
                cout << "Маршрут" << count;
                route.printRouteShortInfo(stationToCheck); // Вывод краткой информации
                routesToShow.push_back(route); // Добавление в временный массив
                cout << "\n";
                found = true;
                count++;
            }
            
        }
    }
    else {
        for (const auto& route : routes) {
            if (route.passesThroughForDay(stationToCheck)) {
                cout << "Маршрут" << count;
                route.printRouteShortInfo(stationToCheck); // Вывод краткой информации
                routesToShow.push_back(route); // Добавление в временный массив
                cout << "\n";
                found = true;
                count++;
            }
            
        }
    }

    if (!found) {
        cout << "Нет маршрутов на этой станции " << stationToCheck << ".\n";
    }
    else {
        // Предоставляем пользователю возможность выбрать маршрут для получения подробной информации
        int choice;
        cout << "Выберите номер маршрута для получения подробной информации (0 - " << (routesToShow.size()-1) << "): ";
        cin >> choice;

        if (choice >= 0 && choice < routesToShow.size()) {
            routesToShow[choice].printRouteInfo(); // Вывод подробной информации о выбранном маршруте
        }
        else {
            cout << "Некорректный номер маршрута.\n";
        }
    }
}

void viewRoutesAtCity(const vector<Route>& routes, vector<City>& cities, SimpleUser simpleUser) {
    string inputtingCity;
    int choise;
    cout << "\nИскать по станциям вашего города - 0 \nПо станциям выбранного города - 1";
    cin >> choise;

    if (choise == 0) {
        inputtingCity = simpleUser.cityOfResidence;
        simpleUser.displayInfo();
    }
    else {
        cout << "Введите город: ";
        cin >> inputtingCity;
    }

    // Находим город в векторе cities
    auto cityIt = find_if(cities.begin(), cities.end(), [&inputtingCity](const City& city) {
        return city.cityName == inputtingCity;
        });

    if (cityIt != cities.end()) {
        cout << "Город найден: " << cityIt->cityName << endl;

        // Список остановок для найденного города
        vector<Stop> foundStops = cityIt->stops;

        // Выводим список остановок
        cout << "Список остановок в городе " << cityIt->cityName << ":\n";
        for (size_t i = 0; i < foundStops.size(); ++i) {
            cout << i + 1 << ". " << foundStops[i].name << endl;
        }

        // Пользователь выбирает остановку
        int stopChoice;
        cout << "Выберите остановку (введите номер): ";
        cin >> stopChoice;

        try {
            // Обработка исключения выхода за пределы
            if (stopChoice < 1 || stopChoice > foundStops.size()) {
                throw InvalidStopChoiceException("Ошибка: Неверный номер остановки.");
            }

            // Вызов функции поиска по станции
            string selectedStop = foundStops[stopChoice - 1].name;
            viewRoutesAtStation(routes, selectedStop);

        }
        catch (const InvalidStopChoiceException& e) {
            cout << e.what() << endl; // Выводим сообщение об ошибке
        }

    }
    else {
        cout << "Город не найден." << endl;
    }
}

void deleteRouteByTrainNumber(vector<Route>& routes, int trainNumber) {
    auto it = remove_if(routes.begin(), routes.end(), [trainNumber](const Route& route) {
        return route.routeNumber == trainNumber;
        });

    if (it != routes.end()) {
        routes.erase(it, routes.end());
        cout << "Маршрут с номером поезда " << trainNumber << " успешно удален." << endl;
    }
    else {
        cout << "Маршрут с номером поезда " << trainNumber << " не найден." << endl;
    }
}

void chooseOptionForAdmin(vector<Route>& routes, vector<City> cities) //2 блок схема
{
    int choise;
    do {
        cout << "Выберите действие:\n";
        cout << "1 - Добавить маршрут в базу\n";
        cout << "2 - Удалить маршрут по номеру:\n";
        cout << "3 - Добавить город:\n";
        cout << "4 - Выйти\n";
        cin >> choise;

        switch (choise) {
        case 1:
            addRoute(routes);
            bubbleSort(routes);
            break;
        case 2:
            int trainNumber;
            cout << "Удаление возможно по номеру маршрута. Введите номер маршрута для удаления: ";
            cin >> trainNumber;
            deleteRouteByTrainNumber(routes, trainNumber);
            break;
        case 3: 
            addCity(cities);
            break;
        case 4:
            cout << "Возвращение в меню\n";
            break;
        default:
            cout << "Введите опцию от 1 до 4.\n";
            break;
        }
    } while (choise != 4);
}





int main() { 
    vector<Route> routes; //список маршрутов
    AdminUser adminUser;
    SimpleUser simpleUser;
    vector<City> cities;

    setlocale(LC_ALL, "");
    cout << "Загрузка...\n";
    loadRoutes(routes, "C:/Routes1/Routes.txt");
    cout << "Готов к работе\n";
    int typeOfUser;
    do {
        cout << "Войти как:\n";
        cout << "Администратор (1)\n";
        cout << "Пользователь(2):\n";
        cout << "Выйти (3)\n";
        cin >> typeOfUser;
        int choise = 0;
        switch (typeOfUser) {
        case 1:
            if (adminUser.enterKey() == 0) {
                cout << "Добро пожаловать, ";
                adminUser.displayInfo();
                chooseOptionForAdmin(routes, cities);
                }
            else
                cout << "Ошибка входа\n";
            break;
        case 2:
            initializeUser(simpleUser);
            simpleUser.displayInfo();
            cout << "Введите 0 - Искать по станции\n1- Искать по городу ";
            cin >> choise;
            if (choise == 0)
            {
                string stationToCheck;
                cout << "Введите станцию для поиска: ";
                cin >> stationToCheck;
                viewRoutesAtStation(routes, stationToCheck);
            }
            else
                viewRoutesAtCity(routes, cities, simpleUser);
            break;
        case 3:
            cout << "Сохранение...\n";
            saveRoutes(routes, "C:/Routes1/Routes.txt");
            cout << "Выход.\n";
            break;
        default:
            cout << "Введите опцию от 1 до 3.\n";
            break;
        }

        cout << "\n"; // Для лучшей читаемости
    } while (typeOfUser != 3);
    return 0; 
}
