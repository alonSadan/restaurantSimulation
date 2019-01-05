#include <utility>
#include <Dish.h>


//
// Created by alon on 04/11/18.
//

#include "Dish.h"

Dish::Dish(int d_id, std::string d_name, int d_price, DishType d_type) : id(d_id), name(std::move(d_name)), price(d_price), type(d_type) {}


int Dish::getId() const {
    return id;
}

std::string Dish::getName() const {
    return name;
}

int Dish::getPrice() const {
    return price;
}

DishType Dish::getType() const {
    return type;
}

Dish::Dish(const Dish &other): id(other.getId()), name(other.getName()), price(other.getPrice()), type(other.getType())  {}




