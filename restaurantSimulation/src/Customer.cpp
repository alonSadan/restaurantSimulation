#include <utility>

//
// Created by alon on 04/11/18.
//

#include "Customer.h"
#include <limits>
#include <fcntl.h>
#include <Customer.h>


Customer::Customer(std::string c_name, int c_id): name((c_name)), id(c_id) {}

std::string Customer::getName() const {
    return name;
}

int Customer::getId() const {
    return id;
}

std::vector<int> &Customer::getAllOrders() {
    return allOrders;
}
//copy constructor
Customer::Customer(const Customer &other): name(other.getName()), id(other.getId()), allOrders((other.allOrders)), nextOrder(other.nextOrder) {
}

Customer::~Customer() {

}

std::vector<int> &Customer::getNextOrder() {
    return this->nextOrder;
}




//class vegetarian customer/////////////////////////////////

VegetarianCustomer::VegetarianCustomer(std::string name, int id) : Customer(name, id) {

}
VegetarianCustomer *VegetarianCustomer::clone() {
    return new VegetarianCustomer(*this);
}

VegetarianCustomer::VegetarianCustomer(const VegetarianCustomer &other): Customer(other) {

}


std::vector<int> VegetarianCustomer::order(const std::vector<Dish> &menu) {

    getNextOrder().clear();
    int lowestVegDishId = INT32_MAX;
    int mostExpensiveBvgPrice = -1;
    int mostExpensiveBvgId = INT32_MAX;

    for (const auto &j : menu) {

        //checking which dish is Veg and has the lowest ID//
        if (j.getType() == VEG && j.getId() <= lowestVegDishId) {
            lowestVegDishId = j.getId();

        }
        //checking which beverage has the highest price and lowest ID//
        if (j.getType() == BVG && j.getPrice() > mostExpensiveBvgPrice) {
            mostExpensiveBvgPrice = j.getPrice();
            mostExpensiveBvgId = j.getId();
        }
       //checking if there are two beverages with the same (highest) price//
        if (j.getType() == BVG && j.getPrice() == mostExpensiveBvgPrice) {
            if (j.getId() <= mostExpensiveBvgId) {
                mostExpensiveBvgId = j.getId();
            }
        }
    }
    //making  sure that there is a veg dish in the menu//
    if (lowestVegDishId < INT32_MAX) {
       getNextOrder().push_back(lowestVegDishId);
    }
    //making sure that there is a soft drink in the menu//
    if (mostExpensiveBvgId < INT32_MAX) {
        getNextOrder().push_back(mostExpensiveBvgId);
    }
    getAllOrders().insert(getAllOrders().end(), getNextOrder().begin(), getNextOrder().end()); //update all orders//
    return getNextOrder();
}

std::string VegetarianCustomer::toString() const {
    return this->getName() + ",veg";

}

VegetarianCustomer::~VegetarianCustomer() {

}

//class cheap customer/////////////////////////////////////
CheapCustomer::CheapCustomer(std::string name, int id) : Customer(name, id) {

    }
std::vector<int> CheapCustomer::order(const std::vector<Dish> &menu) {

    getNextOrder().clear();
    int cheapestDishID= -1;
    int cheapestDishPrice = INT32_MAX;

    for (const auto &i : menu) {
        if (i.getPrice() < cheapestDishPrice) {
            cheapestDishPrice = i.getPrice();
            cheapestDishID = i.getId();
        }else   if(i.getPrice() == cheapestDishPrice)
        {
            if(i.getId() <= cheapestDishID){
                cheapestDishID = i.getId();
            }
        }
    }
//checking if the costumer ordered already //
    if(getAllOrders().empty()) {
        getNextOrder().push_back(cheapestDishID);
    }

        getAllOrders().insert(getAllOrders().end(), getNextOrder().begin(), getNextOrder().end()); //update all orders//
        return getNextOrder();
    }



std::string CheapCustomer::toString() const {
    return this->getName() + ",chp";
}

CheapCustomer::CheapCustomer(const CheapCustomer &other): Customer(other){

}

CheapCustomer *CheapCustomer::clone() {
    return  new CheapCustomer(*this);
    }

CheapCustomer::~CheapCustomer() {

}

//class spicy customer/////////////////////////////////////////////

SpicyCustomer::SpicyCustomer(std::string name, int id) : Customer(name, id) {
}

std::vector<int> SpicyCustomer::order(const std::vector<Dish> &menu) {
    getNextOrder().clear();
    int mostExpensiveSpcPrice = -1;
    int mostExpensiveSpcId = -1;

   //first Order- most expensive Spc dish//
   if(getAllOrders().empty()) {
       for (const auto &i : menu) {
           if (i.getType() == SPC && i.getPrice() > mostExpensiveSpcPrice) {
               mostExpensiveSpcId = i.getId();
               mostExpensiveSpcPrice = i.getPrice();
           }
           if (i.getType() == SPC && i.getPrice() == mostExpensiveSpcPrice) {
             if(i.getId() <= mostExpensiveSpcId){
                 mostExpensiveSpcId = i.getId();
             }
           }
       }
      //making sure there is a spicy dish in the menu//
       if (mostExpensiveSpcId != -1) {
           getNextOrder().push_back(mostExpensiveSpcId);
       }
   }
    //further Orders-cheapest bvg//
    if(!getAllOrders().empty()) {
        int cheapestBvgPrice = INT32_MAX;
        int cheapestBvgId = -1;

        for (const auto &j : menu) {
            if (j.getType() == BVG && j.getPrice() < cheapestBvgPrice) {
                cheapestBvgId = j.getId();
                cheapestBvgPrice = j.getPrice();
            }
            if(j.getType() == BVG && j.getPrice() == cheapestBvgPrice){
                if(j.getId() <= cheapestBvgId){
                    cheapestBvgId = j.getId();
                }
            }
        }
        //making sure there is a Bvg on the menu//
        if (cheapestBvgId != -1) {
            getNextOrder().push_back(cheapestBvgId);
        }
    }
    getAllOrders().insert(getAllOrders().end(), getNextOrder().begin(), getNextOrder().end()); //update allOrders//

    return getNextOrder();
}

std::string SpicyCustomer::toString() const {
    return this->getName() + ",spc";
}

SpicyCustomer *SpicyCustomer::clone() {
    return new SpicyCustomer(*this);
}


SpicyCustomer::SpicyCustomer(const SpicyCustomer &other): Customer(other) {

}

SpicyCustomer::~SpicyCustomer() {

}

// alcoholic customer//
AlchoholicCustomer::AlchoholicCustomer(std::string name, int id) : Customer(name, id), lastOrderedAlcPrice(-1) {

}

std::vector<int> AlchoholicCustomer::order(const std::vector<Dish> &menu) {

    getNextOrder().clear();
    int nextCheapAlcPrice = INT32_MAX;
    int nextCheapAlcId = -1;

  if(getAllOrders().empty()) {         //firstOrder//
      for (const auto &i : menu) {
          if (i.getType() == ALC && i.getPrice() < nextCheapAlcPrice) {
              nextCheapAlcId = i.getId();
              nextCheapAlcPrice = i.getPrice();
          }
      }
      if(nextCheapAlcId < INT32_MAX && nextCheapAlcId != -1 ){  //making sure there is a next cheap dish//
      getNextOrder().push_back(nextCheapAlcId);
      lastOrderedAlcPrice = nextCheapAlcPrice;
      }
  }
  else {   //further orders//
      for (const auto &i : menu) {
          if (i.getType() == ALC && i.getPrice() < nextCheapAlcPrice &&  //dish is of type alc and it's the cheapest among the dishes that are available//
                  i.getPrice() >= lastOrderedAlcPrice && getAllOrders()[getAllOrders().size()-1] != i.getId()) { // making sure it is the *next* cheap alc and that it is not the same dish as before//
              nextCheapAlcId = i.getId();
                  nextCheapAlcPrice = i.getPrice();

          }else  if (i.getType() == ALC && i.getPrice() == nextCheapAlcPrice &&    //dish has the same price as another alc dish in the menu//
                  i.getPrice() >= lastOrderedAlcPrice && i.getId() < nextCheapAlcId) { //dish that among the available dishes with the same price has the lowest ID//
                  nextCheapAlcId = i.getId();

          }

      }

      if (nextCheapAlcPrice == lastOrderedAlcPrice) {
          if(nextCheapAlcId < getAllOrders()[getAllOrders().size()-1]){ //last ordered Alc was with the same price but higher ID//
              if(nextCheapAlcId!= -1 && nextCheapAlcPrice < INT32_MAX) {
                  getNextOrder().push_back(nextCheapAlcId);
                  getAllOrders().insert(getAllOrders().end(), getNextOrder().begin(),
                                        getNextOrder().end()); //update all orders//
              }
              return getNextOrder();
          }else{  //last ordered Alc was the most expensive one//
            return getNextOrder();
          }
      }

      if(nextCheapAlcPrice > lastOrderedAlcPrice ||( nextCheapAlcPrice == lastOrderedAlcPrice
      && nextCheapAlcId < getAllOrders()[getAllOrders().size()-1])) {
          if (nextCheapAlcId != -1 && nextCheapAlcPrice < INT32_MAX) {
          lastOrderedAlcPrice = nextCheapAlcPrice;
          getNextOrder().push_back(nextCheapAlcId);
      }
      }
  }



    getAllOrders().insert(getAllOrders().end(), getNextOrder().begin(), getNextOrder().end()); //update all orders//
    return getNextOrder();

}

std::string AlchoholicCustomer::toString() const {
    return getName() + ",alc";
}

AlchoholicCustomer::AlchoholicCustomer(const AlchoholicCustomer &other): Customer(other){

}

AlchoholicCustomer *AlchoholicCustomer::clone() {
    return new AlchoholicCustomer(*this);
}

AlchoholicCustomer::~AlchoholicCustomer() {

}






