#include "RoastyModel.hpp"
#include <algorithm>

// ========== Bean ==========
Bean::Bean(std::string& Name)
    : name(Name) {
}

Bean& Bean::operator=(const Bean& copy) {
  name = copy.name;
  return *this;
}

std::string& Bean::getName() {
  return name;
}



// ========== Ingredient ==========
Ingredient::Ingredient(Bean bean, int amount)
    : bean(new bean), amount(amount) {
}

Ingredient::Ingredient(const Ingredient& copy) {
  bean = copy.bean;
  amount = copy.amount;
}

Ingredient& Ingredient::operator=(const Ingredient& copy) {
  bean = copy.bean;
  amount = copy.amount;
}

Ingredient::Ingredient(Ingredient&& source) {

}

Ingredient::~Ingredient() {
  delete bean;
}

Bean Ingredient::getBean(std::string) {
  return bean.getName(); }

int Ingredient::getAmount() {
  return amount;
}


// ========== Event ==========
Event::Event(std::string Type, int Timestamp, int* p_eventValue)
    : type(Type), timestamp(Timestamp) {
  if(p_eventValue != nullptr) {
    eventValue = *p_eventValue;
  }
}

int Event::getTimestamp() {
  return timestamp;
}






// ========== Roast ==========

// Roast constructors

Roast::Roast(int Id, int Timestamp)
    : id(Id), timestamp(Timestamp) {
}

Roast::Roast(Roast const& copy) {

}


Roast::~Roast() {
  delete ingredients;
  delete events;
}

// Roast Add Functions

void Roast::addIngredient(Ingredient ingredient) {
  if (num_ingredients == max_ingredients) {

  }

}

void Roast::addEvent(Event event) {

}

// Roast Get Functions

int Roast::getId() {
  return id;
}

int Roast::getTimestamp() {
  return timestamp;
}

Ingredient Roast::getIngredient(int seq) {
  return ingredients.at(seq);
}

Event Roast::getEvent(int seq) {
  return events.at(seq);
}

int Roast::getIngredientsCount() {
  return ingredients.size();
}

int Roast::getEventCount() {
  return events.size();
}


// Roast Remove Functions

void Roast::removeEventByTimestamp(int event_timestamp) {

}

void Roast::removeIngredientByBeanName(std::string bean_name) {

}

