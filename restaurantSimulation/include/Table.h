#ifndef TABLE_H_
#define TABLE_H_

#include <vector>
#include "Customer.h"
#include "Dish.h"

typedef std::pair<int, Dish*> OrderPair;    //<customer ID, Dish>

class Table{
public:
    Table(int t_capacity);
    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void order(const std::vector<Dish> &menu);
    void openTable();
    void closeTable();
    int getBill();
    bool isOpen();

    // Destructor
     ~Table();

    // Copy Constructor
    Table(const Table &other);

    // Move Constructor
    Table(Table &&other);

    // Copy Assignment
    Table& operator=(const Table &other);

    // Move Assignment
    Table& operator=(Table &&other);

    Dish* getDish(const std::vector <Dish> &menu, int d_id); //yot

private:
    int capacity;
    bool open;
    std::vector<Customer*> customersList;
    void clear();
    std::vector<OrderPair> orderList; //(customer_id, Dish) //A list of pairs for each order in a table
};


#endif