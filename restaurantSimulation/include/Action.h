//
// Created by alon on 04/11/18.
//

#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "Customer.h"

enum ActionStatus{
    PENDING, COMPLETED, ERROR

};

//Forward declaration
class Restaurant;

class BaseAction{
public:
    BaseAction();
    BaseAction(const BaseAction &other);
    ActionStatus getStatus() const;
    virtual void act(Restaurant& restaurant)=0;
    virtual std::string toString() const=0;
    BaseAction&  operator=(const BaseAction &other);
    virtual BaseAction* clone()=0;
    virtual ~BaseAction();
protected:
    void complete();
    void error(std::string errorMsg);
    std::string getErrorMsg() const;
    //all actions contain no complex variables so default destructor is used
private:
    std::string errorMsg;
    ActionStatus status;
};


class OpenTable : public BaseAction {
public:
    OpenTable(int id, std::vector<Customer *> &customersList);
    OpenTable(const OpenTable& other);
    void act(Restaurant &restaurant) override;
    std::string toString() const override;
    OpenTable* clone() override;
    ~OpenTable();

private:
    const int tableId;
    std::vector<Customer *> customers;
};


class Order : public BaseAction {
public:
    Order(int id);
    Order(const Order& other);
    void act(Restaurant &restaurant);
    std::string toString() const;
    Order* clone();
    ~Order();

private:
    const int tableId; //yot

};


class MoveCustomer : public BaseAction {
public:
    MoveCustomer(int src, int dst, int customerId);
    MoveCustomer(const MoveCustomer& other);
    void act(Restaurant &restaurant);
    std::string toString() const;
    MoveCustomer* clone();
    ~MoveCustomer();
private:
    const int srcTable;
    const int dstTable;
    const int id;
};


class Close : public BaseAction {
public:
    Close(int id);
    Close(const Close& other);
    void act(Restaurant &restaurant);
    std::string toString() const;
    Close* clone();
    ~Close();
private:
    const int tableId;//yot
    std::string bill;
};


class CloseAll : public BaseAction {
public:
    CloseAll();
    CloseAll(const CloseAll& other);
    void act(Restaurant &restaurant);
    std::string toString() const;
    CloseAll* clone();
    ~CloseAll();
private:
    std:: string bill;//yot
};


class PrintMenu : public BaseAction {
public:
    PrintMenu();
    PrintMenu(const PrintMenu& other);
    void act(Restaurant &restaurant);
    std::string toString() const;
    PrintMenu* clone();
    ~PrintMenu();
private:
    std::string toPrint; //yot
};


class PrintTableStatus : public BaseAction {
public:
    PrintTableStatus(int id);
    PrintTableStatus(const PrintTableStatus& other);
    void act(Restaurant &restaurant) override;
    std::string toString() const;
    PrintTableStatus* clone();
    ~PrintTableStatus();

private:
    const int tableId;
  //  std::string s;
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    PrintActionsLog (const PrintActionsLog& other);
    void act(Restaurant &restaurant);
    std::string toString() const;
    PrintActionsLog* clone();
    ~PrintActionsLog();
private:
    std::string actionLog;
};


class BackupRestaurant : public BaseAction {
public:
    BackupRestaurant();
    BackupRestaurant(const BackupRestaurant& other);
    void act(Restaurant &restaurant);
    std::string toString() const;
    BackupRestaurant* clone();
    ~BackupRestaurant();
private:

};


class RestoreRestaurant : public BaseAction {
public:
    RestoreRestaurant();
    RestoreRestaurant(const RestoreRestaurant& other);  //copy constructor//
    void act(Restaurant &restaurant);
    std::string toString() const;
     RestoreRestaurant* clone(); //add by Alon
    ~RestoreRestaurant();
};

#endif