#include "Table.h"
#include <iostream>

Table::Table(int t_capacity) : capacity(t_capacity), open(false) {}


int Table::getCapacity() const {
    return capacity;
}

void Table::addCustomer(Customer *customer) {
    customersList.push_back(customer);
}

void Table::removeCustomer(int id) {
    for (unsigned int i = 0; i < customersList.size(); ++i) {
        if (customersList[i]->getId() == id) {
            //remove
            std:: vector <Customer*> tmp;
            for (unsigned int j = 0; j < customersList.size(); ++j) {
                if(j != i)
                    tmp.emplace_back(customersList[j]);
            }
            customersList.clear();
            for (unsigned int k = 0; k < tmp.size(); ++k) {
                customersList.emplace_back(tmp[k]);
            }
            //
        }
    }
}
Customer *Table::getCustomer(int id) {
    for (auto &i : customersList) {
        if (i->getId() == id)
            return i;
    }
    std::cout << "customer doesnt exist" << std::endl;
    return nullptr;
}

std::vector<Customer *> &Table::getCustomers() {
    return customersList;
}

std::vector<OrderPair> &Table::getOrders() {
    return orderList;
}

void Table::order(const std::vector<Dish> &menu) {
    for (auto &i : customersList) {
        std:: vector <int> orders = i->order(menu);
        for (int order : orders) {
            OrderPair p = std::make_pair(i->getId(),getDish(menu, order));
            orderList.push_back(p);
        }
    }

}

void Table::openTable() {
    open = true;
}

void Table::closeTable() {
    open = false;
    clear();
}

int Table::getBill() {
    int bill = 0;
    for (auto &i : orderList) {
        bill = bill + i.second->getPrice();
    }
    return bill;
}

bool Table::isOpen() {
    return open;
}

Dish* Table::getDish (const std::vector<Dish> &menu, int d_id) {   //changed by Alon to Dish*//
    for(const auto &dish : menu){
        if (dish.getId() == d_id){
           // return new Dish(dish);
           return const_cast<Dish *>(&dish);
        }
    }
    std:: cout <<"the dish is not in the menu"<< std::endl;
    return nullptr;
}

//destructor
Table::~Table() {
    clear();
}
//copy constructor
Table::Table(const Table &other) {
    if(this != &other) {
      //  Table *t = new Table(other.getCapacity());
        for(auto customer : other.customersList ){
                this->customersList.push_back(customer->clone());
        }
        for(auto order : other.orderList){
            this->orderList.push_back(order);
        }
        open = other.open;
    }
}

//move constructor
//Table::Table(Table &&other) : capacity(other.capacity), open(other.open), customersList(other.customersList), orderList(other.orderList) {
 //   other.clear();
//}
//copy assignment operator
Table &Table::operator=(const Table &other) {
    if(this != &other) {
        this->clear();
        for(auto &customer: other.customersList ){
            customersList.push_back(customer->clone());
        }
        for(auto &orderPair:other.orderList){
            this->orderList.push_back(std::make_pair(orderPair.first,new Dish(*orderPair.second)));
        }
        capacity = other.capacity;
        open = other.open;

    }
    return *this;
}
//move assignment operator
//Table &Table::operator=(Table &&other) {
//    if(this != &other){
//        clear();
//        customersList = other.customersList;
//        orderList = other.orderList;
//        capacity = other.capacity;
//        open = other.open;
//        other.clear();
//    }
//    return *this;
//}

void Table:: clear(){
    open = false;
    capacity = 0;

//    for (auto &orderPair : orderList) {
//        delete orderPair.second;
//    }
    orderList.clear();
    for(auto &customer: customersList){
        delete customer;
    }
    customersList.clear();
}





