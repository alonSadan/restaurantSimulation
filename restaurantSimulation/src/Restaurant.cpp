//
// Created by alon on 04/11/18.
//
#include "Restaurant.h"
#include <iostream>
#include <string>
#include <sstream>

extern Restaurant* backup;

Restaurant::Restaurant():open(false)  {
    actionsLog = std::vector<BaseAction*>();
}

Restaurant::Restaurant(const std::string &configFilePath)  {
    std::string line;
    std::ifstream file (configFilePath);
    std::string dishName;
    std::string dishType;
    int dishId = 0;

    int dishPrice = 0;
    getline(file, line);
    while( line[0] == '#' || line.empty() ) {
        getline(file, line);
    }
    getline(file,line); //skip the number of tables line//
    /////////////we get number of tables as input but we never use it in our progream. is it OK?////
    while(line[0] == '#' || line.empty()) {
        getline(file, line);
    }

    std::istringstream row(line);
    int i;
    while (row >> i){
        Table *t = new Table(i);
        tables.push_back(t);
        if (row.peek() == ',' || row.peek() == ' '){
            row.ignore();
        }
    }


    getline(file,line);
    while((line[0] == '#' || line.empty())) {
        if(!file.good()){
            file.close();
            return ;
        }
        getline(file, line);

        std::string delimiter = ",";
        std::string token;
        int position = 0;


        while (line[0] != '#' && !line.empty()) {
            if (line.find(delimiter) != std::string::npos) {
                position = line.find(delimiter);
                token = (line.substr(0, position));
                dishName = token;
                line.erase(0, position + 1);
            }
            if (line.find(delimiter) != std::string::npos) {
                position = line.find(delimiter);
                token = (line.substr(0, position));
                dishType = token;
                line.erase(0, position + 1);
            }
            if (line.find(delimiter) == std::string::npos) {
                dishPrice = std::stoi(line);
                line.erase();
            }
            menu.emplace_back(Dish(dishId, dishName, dishPrice, stringToEnum(dishType)));
            ++dishId;


        }
    }


    file.close();

}

void Restaurant::start() {
    open = true;
    std::cout << "Restaurant is now open" << std::endl;
    std::string line;
    std::string word;
    int tableId = 0;
    int customerId = 0;



    while (line != "closeall" ) {
        getline(std::cin, line);
        std::istringstream str(line);
        str >> word;
        if (word == "open") {

            std::vector<Customer *> customerList;

            std::string customerType;
            std::string customerName;
            str >> tableId;

            while (str.good() && str >> word) {
                customerName = word.substr(0,word.find(','));
                customerType = word.substr(word.find(',')+1 ,word.size());     ////
                if (customerType == "veg") {
                    VegetarianCustomer *t = new VegetarianCustomer(customerName, customerId);
                    customerList.push_back(t);
                    ++customerId;
                } else if (customerType == "chp") {
                    CheapCustomer *t = new CheapCustomer(customerName, customerId);
                    customerList.push_back(t);
                    ++customerId;
                } else if (customerType == "spc") {
                    SpicyCustomer *t = new SpicyCustomer(customerName, customerId);
                    customerList.push_back(t);
                    ++customerId;
                } else if (customerType == "alc") {
                    AlchoholicCustomer *t = new AlchoholicCustomer(customerName, customerId);
                    customerList.push_back(t);
                    ++customerId;
                }

            }
            OpenTable *openTable = new OpenTable(tableId, customerList);
            openTable->act(*this);
            if (openTable->getStatus()==ERROR){
                customerList.clear();
            }
//            ++tableId;
            actionsLog.push_back(openTable);
            word = "";


        }else   if (word == "order") {
            str >> tableId;
            Order *order = new Order(tableId);
            order->act(*this);
            actionsLog.push_back(order);
            word = "";



        } else     if (word == "move") {
            str >> word;
            int src = std::stoi(word);
            str >> word ;
            int dest = std::stoi(word);
            str >> customerId;
            MoveCustomer *move = new MoveCustomer(src, dest, customerId);
            move->act(*this);
            actionsLog.push_back(move);
            word = "";


        }else    if (word == "close"){
            str >> word;
            tableId = std::stoi(word);
            Close *close = new Close (tableId);
            close->act(*this);
            actionsLog.push_back(close);
            word = "";


        }else   if(word == "closeall"){
            CloseAll *closeAll = new CloseAll();
            closeAll->act(*this);
            delete closeAll;
            this->open = false;

            return;

        }else if(word == "menu"){
            PrintMenu *printMenu = new PrintMenu();
            printMenu->act(*this);
            actionsLog.push_back(printMenu);
            word = "";


        } else if( word == "status"){
            str >> word;
            tableId =std::stoi(word);
            PrintTableStatus *printTableStatus = new PrintTableStatus(tableId);
            printTableStatus->act(*this);
            actionsLog.push_back(printTableStatus);
            word = "";


        } else if(word =="log" ){
            PrintActionsLog *printActionsLog = new PrintActionsLog();
            printActionsLog->act(*this);
            delete printActionsLog;
            word = "";


        }else if(word == "backup"){
            BackupRestaurant *backupRestaurant = new BackupRestaurant();
            backupRestaurant->act(*this);
            actionsLog.push_back(backupRestaurant);
            word = "";




        }else if (word == "restore"){
            RestoreRestaurant *restoreRestaurant = new RestoreRestaurant();
            restoreRestaurant->act(*this);
            actionsLog.push_back(restoreRestaurant);
            word = "";


        }

        }
    word = "";


    }



int Restaurant::getNumOfTables() const {
    return static_cast<int>(tables.size());            //return tables.size();//
}

Table *Restaurant::getTable(int ind) {
    return tables[ind];
}

const std::vector<BaseAction *> &Restaurant::getActionsLog() const {
    return this->actionsLog;
}


std::vector<Dish> &Restaurant::getMenu() {
    return menu;
}


//move constructor//
Restaurant::Restaurant(Restaurant &&other) : open(other.open), tables(other.tables), menu(other.menu), actionsLog(other.actionsLog) {
    other.clear();
}
// Copy Assignment operator
Restaurant &Restaurant::operator=(const Restaurant &other) {
    if(this != &other){
        if(this != nullptr)
        this->clear();
        open = other.open;
       for(auto &table : other.tables){
           this->tables.push_back(new Table(*table));
       }
        for (const auto &dish : other.menu) {
            this->menu.push_back(dish);
        }
        for (auto action : other.actionsLog) {
            this->actionsLog.push_back(action->clone());
        }
    }
    return *this;

}

void Restaurant::clear() {
    open = false;
    menu.clear();
    for (auto &table : tables) {
        delete table;
    }

    tables.clear();

    for (auto &i : actionsLog) {
        delete i;
    }

    actionsLog.clear();

}
//destructor
Restaurant::~Restaurant() {
    clear();
}
//copy constructor
Restaurant::Restaurant(const Restaurant &other): open(other.open)  {

   for(auto &table : other.tables){
       this->tables.push_back(new Table(*table));
   }
   for(auto &dish : other.menu){
       this->menu.push_back(Dish(dish));
   }
   for( auto &actionLog : other.actionsLog){
       this->actionsLog.push_back(actionLog->clone());
   }
}

DishType Restaurant::stringToEnum(const std::string str) {
    if(str == "VEG"){
        return VEG;
    }
    if(str == "SPC"){
        return SPC;
    }
    if(str == "BVG"){
        return BVG;
    }
    if(str == "ALC"){
        return ALC;
    }
    std::cout << "stringToEnum didnt go well" << std::endl;
    return VEG;
}
