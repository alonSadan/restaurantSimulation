//
// Created by alon on 04/11/18.
//

#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include "Dish.h"

class Customer{
public:
    Customer(std::string c_name, int c_id);
    Customer(const Customer &other);
    virtual std::vector<int> order(const std::vector<Dish> &menu)=0;
    virtual std::string toString() const = 0;
    std::string getName() const;
    std::vector<int> &getAllOrders();   //add by me//
    virtual ~Customer();
    std::vector<int> &getNextOrder();
    virtual Customer* clone ()=0;


    int getId() const;
private:
    const std::string name;
    const int id;
    std::vector<int> allOrders;   //add by me//
    std::vector<int> nextOrder;   //added by Alon//
};


class VegetarianCustomer : public Customer {
public:
    VegetarianCustomer(std::string name, int id);
    VegetarianCustomer(const VegetarianCustomer &other1);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    VegetarianCustomer* clone() override;
    ~VegetarianCustomer();
private:
};


class CheapCustomer : public Customer {
public:
    CheapCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    CheapCustomer(const CheapCustomer &other);
    CheapCustomer* clone();
    ~CheapCustomer();
private:

};


class SpicyCustomer : public Customer {
public:
    SpicyCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    SpicyCustomer(const SpicyCustomer &other);
    SpicyCustomer* clone() override;
    ~SpicyCustomer();
private:

};


class AlchoholicCustomer : public Customer {
public:
    AlchoholicCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    AlchoholicCustomer(const AlchoholicCustomer &other);
    AlchoholicCustomer* clone() override;
    ~AlchoholicCustomer();
private:
int lastOrderedAlcPrice;
};


#endif