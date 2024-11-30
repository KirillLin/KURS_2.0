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

void addRoute(std::vector<Route>& routes) {
    std::string routeName, departureTime, arrivalTime, trainName;
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

    char addMoreStops;
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

        cout << "Add another stop? (y/n): ";
        cin >> addMoreStops;
    } while (addMoreStops == 'y' || addMoreStops == 'Y');

    char addMoreVagons;
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

        cout << "Добавить ещё вагон? (y/n): ";
        cin >> addMoreVagons;
    } while (addMoreVagons == 'y' || addMoreVagons == 'Y');

    routes.push_back(newRoute);
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
        --n; // Уменьшаем n, так как последний элемент уже отсортирован
    } while (swapped);
}

using namespace std;

void viewRoutesAtStation(const std::vector<Route>& routes) {
    string stationToCheck;
    vector<Route> routesToShow; // список найденных маршрутов 
    cout << "Введите станцию для поиска: ";
    cin >> stationToCheck;
    bool isAllDaysOrForOneDay = chooseTypeOfFinding(); // 0 - конкретный день, 1 - все дни 

    bool found = false;
    int count = 1;
    if (isAllDaysOrForOneDay) {
        for (const auto& route : routes) {
            if (route.passesThrough(stationToCheck)) {
                cout << "Маршрут" << count;
                route.printRouteShortInfo(); // Вывод краткой информации
                routesToShow.push_back(route); // Добавление в временный массив
                std::cout << "\n";
                found = true;
            }
            count++;
        }
    }
    else {
        for (const auto& route : routes) {
            if (route.passesThroughForDay(stationToCheck)) {
                cout << "Маршрут" << count;
                route.printRouteShortInfo(); // Вывод краткой информации
                routesToShow.push_back(route); // Добавление в временный массив
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
        // Предоставляем пользователю возможность выбрать маршрут для получения подробной информации
        int choice;
        cout << "Выберите номер маршрута для получения подробной информации (0 - " << (routesToShow.size() - 1) << "): ";
        cin >> choice;

        if (choice >= 0 && choice < routesToShow.size()) {
            routesToShow[choice].printRouteInfo(); // Вывод подробной информации о выбранном маршруте
        }
        else {
            cout << "Некорректный номер маршрута.\n";
        }
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

void chooseOptionForAdmin(vector<Route>& routes)
{
    int choise;
    do {
        cout << "Выберите действие:\n";
        cout << "1 - Добавить маршрут в базу\n";
        cout << "2 - Удалить маршрут по номеру:\n";
        cout << "3 - Выйти\n";
        cin >> choise;

        switch (choise) {
        case 1:
            addRoute(routes);
            bubbleSort(routes);
            break;
        case 2:
            int trainNumber;
            cout << "Введите номер маршрута для удаления: ";
            cin >> trainNumber;
            deleteRouteByTrainNumber(routes, trainNumber);
            break;
        case 3:
            cout << "Возвращение в меню\n";
            break;
        default:
            cout << "Введите опцию от 1 до 3.\n";
            break;
        }
    } while (choise != 3);
}
