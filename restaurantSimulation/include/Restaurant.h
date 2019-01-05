
#ifndef RESTAURANT_H_
#define RESTAURANT_H_

#include <vector>
#include <string>
#include "Dish.h"
#include "Table.h"
#include "Action.h"
#include <iostream>
#include <fstream>


class Restaurant{
public:
    Restaurant();
    Restaurant(const std::string &configFilePath);
    void start();
    int getNumOfTables() const;
    Table* getTable(int ind);
    const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Dish>& getMenu();
    void clear ();

    // Destructor
    virtual ~Restaurant();

    // Copy Constructor
    Restaurant(const Restaurant &other);

    // Move Constructor
    Restaurant(Restaurant &&other);

    // Copy Assignment operator
    Restaurant& operator=(const Restaurant &other);

    // Move Assignment operator
    Restaurant& operator=(Restaurant &&other);


    DishType stringToEnum(const std::string str);



private:
    bool open;
    std::vector<Table*> tables;


private:
    std::vector<Dish> menu;
    std::vector<BaseAction*> actionsLog;
};

#endif