#include <utility>

#include <string>
#include <Action.h>

#include "Action.h"
#include "Table.h"
#include "Restaurant.h"

extern Restaurant* backup;

BaseAction::BaseAction() : status(PENDING) {}

ActionStatus BaseAction::getStatus() const {
    return status;
}
void BaseAction::complete() {
    status = COMPLETED;                //some methods has complete() and some methods not, is it on purpose?//
    errorMsg = "";
}

void BaseAction::error(std::string errorMsg)  {
    status = ERROR;
    this->errorMsg = std::move(errorMsg);
}

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}



BaseAction &BaseAction::operator=(const BaseAction &other) {
    errorMsg = other.errorMsg;
    status = other.status;
    return *this;
}


BaseAction::BaseAction(const BaseAction &other)
        : errorMsg(other.errorMsg), status(other.status) {
}

BaseAction::~BaseAction() {

}

std::string OpenTable::toString() const {
    std::string ans = "open "+ std::to_string(tableId) + " ";

    for (auto customer : customers) {
       ans +=  customer->toString() +" ";
   }
    std:: string stat="";
    if( getStatus()==0){
        stat = "Pending";
    } else if(getStatus() == 1 ){
        stat = "Completed";
    } else if(getStatus() == 2){
        stat = getErrorMsg();
        return stat;
    }

    ans += stat;
    return ans; // maybe we do need a string here like all the prints?
}


void OpenTable::act(Restaurant &restaurant) {
    if (restaurant.getTable(tableId) == nullptr || restaurant.getTable(tableId)->isOpen()) {
        std::string message= "open "+ std::to_string(tableId) + " ";
        for(auto customer: customers){
            message += customer->toString()+ " ";
        }
        message += "Error: table doesnt exist or is already open";

        error(message);
        std:: cout << "Error: table doesnt exist or is already open" << std::endl;
        for (auto &customer : customers){
            delete customer;
        }
        customers.clear();
        return;
    }
    restaurant.getTable(tableId)->openTable();
    for (auto &customer : customers) {
        restaurant.getTable(tableId)->addCustomer(customer->clone());
    }
    complete();
}


OpenTable::OpenTable(int id, std::vector<Customer *> &customersList) : tableId(id), customers(customersList) {}

OpenTable::OpenTable(const OpenTable &other): BaseAction(other), tableId(other.tableId) {
    for (auto &i : other.customers) {
        this->customers.push_back(i->clone());
    }
}

OpenTable *OpenTable::clone() {

    return new OpenTable(*this);
}

OpenTable::~OpenTable() {
for(auto &customer: customers){
    delete customer;
}
    this->customers.clear();

}


Order::Order(int id) : tableId(id) {}

void Order::act(Restaurant &restaurant) {
    std::string ppl_orders;
    restaurant.getTable(tableId)->order(restaurant.getMenu());


    for (auto &i : restaurant.getTable(tableId)->getCustomers()) {
        for(unsigned int j = 0; j < i->getNextOrder().size(); ++j){
            ppl_orders += i->getName()+ " " + "ordered ";
            ppl_orders += restaurant.getTable(tableId)->getDish(restaurant.getMenu(),i->getNextOrder()[j] )->getName() +"\n";
        }
    }
    std::cout << ppl_orders << std:: endl ;
    complete();
}

std::string Order::toString() const {
    std:: string stat="";
    if( getStatus()==0){
        stat = "Pending";
    } else if(getStatus() == 1 ){
        stat = "Completed";
    } else if(getStatus() == 2){
        stat = "Error: "+ getErrorMsg();
    }
        return "Order " + std::to_string(tableId) + " " + stat;
}

Order::Order(const Order &other) : BaseAction(other),tableId(other.tableId){
}

Order *Order::clone() {
    return new Order(*this);
}

Order::~Order() {

}


MoveCustomer::MoveCustomer(int src, int dst, int customerId) : srcTable(src), dstTable(dst), id(customerId) {}

void MoveCustomer::act(Restaurant &restaurant) {

    //origin table exists and open
    if( restaurant.getTable(srcTable) == nullptr || !restaurant.getTable(srcTable)->isOpen()){
        error("Cannot move customer");
        return;
    }
    //destination table exists and open
    if( restaurant.getTable(dstTable) == nullptr || !restaurant.getTable(dstTable)->isOpen()){
        error("Cannot move customer");
        return;
    }
    //destination table not full
    if (restaurant.getTable(dstTable)->getCustomers().size() + 1 > (unsigned int)restaurant.getTable(dstTable)->getCapacity()) {
        error("Cannot move customer");
        return;
    }
    //the customer is in the origin table
    bool isInOrigin = false;
    for (auto &j : restaurant.getTable(srcTable)->getCustomers()) {
        if (j->getId() == id)
            isInOrigin = true;
    }
    if(!isInOrigin){
        error("Cannot move customer");
        return;
    }

    // the customer is not already in the destination table
    for (auto &i : restaurant.getTable(dstTable)->getCustomers()) {
        if(i->getId() == id) {
            error("Cannot move customer");
            return;
        }
    }
    //add the customer to dstTable
    restaurant.getTable(dstTable)->addCustomer(restaurant.getTable(srcTable)->getCustomer(id));

    //add dishes to dst
    for (unsigned int i = 0; i < restaurant.getTable(srcTable)->getOrders().size(); ++i) {
        if (restaurant.getTable(srcTable)->getOrders()[i].first == id) // = customer id
            restaurant.getTable(dstTable)->getOrders().emplace_back(id, restaurant.getTable(srcTable)->getOrders()[i].second);
    }
    //remove dishes from src
    for (unsigned int i = 0; i < restaurant.getTable(srcTable)->getOrders().size(); ++i) {
        if (restaurant.getTable(srcTable)->getOrders()[i].first == id) { // = customer id

            std::vector<OrderPair> tmp;
            for (unsigned int j = 0; j < restaurant.getTable(srcTable)->getOrders().size(); ++j) {
                if (restaurant.getTable(srcTable)->getOrders()[j].first != id) {
                    tmp.emplace_back(restaurant.getTable(srcTable)->getOrders()[j]);
                }
            }
            restaurant.getTable(srcTable)->getOrders().clear();
            for (unsigned int k = 0; k < tmp.size(); ++k) {
                restaurant.getTable(srcTable)->getOrders().emplace_back(tmp[k]);
            }

        }
    }

    restaurant.getTable(srcTable)->removeCustomer(id);
    //if src empty close src
    if (restaurant.getTable(srcTable)->getCustomers().empty()){
        restaurant.getTable(srcTable)->closeTable();
    }
    complete();
}

std::string MoveCustomer::toString() const {
    if(getStatus() != COMPLETED)
        std:: cout << getErrorMsg() << std:: endl;
   std::string move = "";
    std:: string stat="";
    if( getStatus()==0){
        stat = "Pending";
    } else if(getStatus() == 1 ){
        stat = "Completed";
    } else if(getStatus() == 2){
        stat = "Error: "+ getErrorMsg();
    }

    move = "move " + std::to_string(srcTable)+ " "+ std::to_string(dstTable) + " " + std::to_string(id) + " " + stat;
    return move;
}

MoveCustomer::MoveCustomer(const MoveCustomer &other):BaseAction(other), srcTable(other.srcTable),
                                                      dstTable(other.dstTable),id(other.id) {

}

MoveCustomer *MoveCustomer::clone() {
    return new MoveCustomer(*this);
}

MoveCustomer::~MoveCustomer() {

}

Close::Close(int id) :tableId(id), bill("Bill ") {}

void Close::act(Restaurant &restaurant) {
    if( restaurant.getTable(tableId) == nullptr || !restaurant.getTable(tableId)->isOpen())
        error("Error: table does not exist or is not open");
    restaurant.getTable(tableId)->closeTable();
    bill += std::to_string(restaurant.getTable(tableId)->getBill()) + "NIS";
    complete();
}

std::string Close::toString() const {
    if(getStatus() != COMPLETED)
        return getErrorMsg();

    return "Table " + std:: to_string(tableId) + " is closed." +bill;
}

Close::Close(const Close &other): BaseAction(other), tableId(other.tableId) {

}

Close *Close::clone() {
    return new Close(*this);
}

Close::~Close() {

}

CloseAll::CloseAll() :bill("") {}

void CloseAll::act(Restaurant &restaurant) {
    //print bill, then close each table
    for (  int i = 0; i < restaurant.getNumOfTables(); ++i) {
        if(restaurant.getTable(i)->isOpen()) {
            bill += "Table " + std::to_string(i) + " was closed. BILL " + std::to_string(restaurant.getTable(i)->getBill()) + "NIS" + "\n";
            restaurant.getTable(i)->closeTable();
        }

    }
std::cout << bill << std::endl;
    restaurant.clear();
    complete();

}

std::string CloseAll::toString() const {
    return bill;
}

CloseAll::CloseAll(const CloseAll &other): BaseAction(other), bill(other.bill) {

}

CloseAll *CloseAll::clone() {
    return new CloseAll (*this);
}

CloseAll::~CloseAll() {

}


PrintMenu::PrintMenu() :toPrint("") {}

void PrintMenu::act(Restaurant &restaurant) {
    for (auto &i : restaurant.getMenu()) {
        toPrint += i.getName() + " ";
        toPrint += std::to_string(i.getType()) + " ";
        toPrint += std::to_string(i.getPrice()) + "\n";
    }
    std::cout << toPrint << std::endl;
    complete();
}

std::string PrintMenu::toString() const {

    std:: string stat="";
    if( getStatus()==0){
        stat = "Pending";
    } else if(getStatus() == 1 ){
        stat = "Completed";
    } else if(getStatus() == 2){
        stat = "Error: " + getErrorMsg();
    }
    return "menu " + stat;
}

PrintMenu::PrintMenu(const PrintMenu &other): BaseAction(other), toPrint(other.toPrint) {

}

PrintMenu *PrintMenu::clone() {
    return new PrintMenu(*this);
}

PrintMenu::~PrintMenu() {

}


PrintTableStatus::PrintTableStatus(int id) : tableId(id) {}

PrintTableStatus::PrintTableStatus(const PrintTableStatus &other): BaseAction(other), tableId(other.tableId) {
}

void PrintTableStatus::act(Restaurant &restaurant) {
    if(!restaurant.getTable(tableId)->isOpen()) {
        std::cout << "table " + std::to_string(tableId) + " is closed" << std::endl;
    }
    else {

        std::string s;

        s = "Table "+std::to_string(tableId)+" status: open\n";
        s += "Customers:\n";
        for (auto i : restaurant.getTable(tableId)->getCustomers()) {
            s += std::to_string(i->getId()) + " " + i->getName() + "\n";
        }
        s.append("Orders:\n");
        for (auto &j : restaurant.getTable(tableId)->getOrders()) {
            s += j.second->getName() + " " +  std::to_string(j.second->getPrice()) + "NIS" + " " + std::to_string(j.first) + "\n";
        }
        s.append("Current bill: ");
        s.append(std::to_string(restaurant.getTable(tableId)->getBill()));
        s.append("NIS");
        std::cout << s << std::endl;
    }
    complete();

}

std::string PrintTableStatus::toString() const {
    std:: string stat="";
    if( getStatus()==0){
        stat = "Pending";
    } else if(getStatus() == 1 ){
        stat = "Completed";
    } else if(getStatus() == 2){
        stat = "Error: " + getErrorMsg() ;
    }
    return "status " + std::to_string(tableId) + " " +stat;
}



PrintTableStatus *PrintTableStatus::clone() {
    return new PrintTableStatus(*this);
}

PrintTableStatus::~PrintTableStatus() {

}


PrintActionsLog::PrintActionsLog() : actionLog("") {}

void PrintActionsLog::act(Restaurant &restaurant) {
    std::vector<BaseAction*> acs = restaurant.getActionsLog();
    for (auto &ac : acs) {
        actionLog += ac->toString()+"\n";
    }
    std::cout << actionLog << std::endl;
    complete();
}

std::string PrintActionsLog::toString() const {
    return actionLog;

}

PrintActionsLog::PrintActionsLog(const PrintActionsLog &other):BaseAction(other),actionLog(other.actionLog) {
}

PrintActionsLog *PrintActionsLog::clone() {
    return new PrintActionsLog(*this);
}

PrintActionsLog::~PrintActionsLog() {

}

BackupRestaurant::BackupRestaurant() {}

void BackupRestaurant::act(Restaurant &restaurant) {
    if(backup!= nullptr){
        backup->clear();
    }
    backup = new Restaurant(restaurant);
    complete();
}

std::string BackupRestaurant::toString() const {
    std:: string stat="";
    if( getStatus()==0){
        stat = "Pending";
    } else if(getStatus() == 1 ){
        stat = "Completed";
    } else if(getStatus() == 2){
        stat = "Error: " + getErrorMsg();
    }
    return "backup " + stat  ;
}

BackupRestaurant* BackupRestaurant::clone() {
    return new BackupRestaurant(*this);
}

BackupRestaurant::BackupRestaurant(const BackupRestaurant &other):BaseAction(other) {

}

BackupRestaurant::~BackupRestaurant() {

}

RestoreRestaurant::RestoreRestaurant() {}

void RestoreRestaurant::act(Restaurant &restaurant) {
    if (backup == nullptr){
        std:: cout<<"no backup available"<< std::endl;
        return;
    }
    restaurant = *backup;// * is the right syntax? //
    complete();

}

std::string RestoreRestaurant::toString() const {
    std:: string stat="";
    if( getStatus()==0){
        stat = "Pending";
    } else if(getStatus() == 1 ){
        stat = "Completed";
    } else if(getStatus() == 2){
        stat = "Error: " + getErrorMsg();
    }
    return "restore " + stat;
}

RestoreRestaurant* RestoreRestaurant::clone() {
    return new RestoreRestaurant(*this);
}

RestoreRestaurant::RestoreRestaurant(const RestoreRestaurant &other):BaseAction(other) {

}

RestoreRestaurant::~RestoreRestaurant() {

}


