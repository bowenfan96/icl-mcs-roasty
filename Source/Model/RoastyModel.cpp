#include "RoastyModel.hpp"
#include <iostream>

// ================ BEAN ===================
Bean::Bean(std::string name) : name(name) {}

Bean& Bean::operator=(std::string const& name) {
  this->name = name;
  return *this;
}

std::string Bean::getName() const {
  return name;
}



// =============== INGREDIENT ===============
Ingredient::Ingredient(Bean& bean, int amount) : bean(bean), amount(amount) {}


Bean Ingredient::getBean() const {
  return bean;
}

int Ingredient::getAmount() const {
  return amount;
}




// =========== EVENT VALUE =================

EventValue::EventValue(int value) : value(value) {}

int EventValue::getValue() {
  return value;
}




// ================ EVENT ==================
Event::Event(std::string type, long timestamp, EventValue* eventValue)
    : type(type), timestamp(timestamp) {

  std::cout << "Hi imma event got init-ed" << std::endl;

  this->eventValue = eventValue;
}

Event::Event(std::string type, long timestamp)
    : type(type), timestamp(timestamp) {

  std::cout << "Hi imma event with no value inited" << std::endl;

}


long Event::getTimestamp() const {
  std::cout << "heres ur timestamp" << std::endl;
  std::cout << timestamp << std::endl;
  return timestamp;
}


std::string Event::getType() const {
  return type;
}

bool Event::hasValue() const {
  return eventValue != nullptr;
}

EventValue* Event::getValue() const {
  return eventValue;
}

// ============== ROAST ===================
Roast::Roast(long id, long timestamp) : id(id), timestamp(timestamp) {

  std::cout << "Roast created" << std::endl;

}


// ADD
void Roast::addIngredient(Ingredient ingd) {
  if(ingredients == nullptr) {
    ingredients = new Ingredient(ingd);
  }
  else {
    Ingredient* iterator = ingredients;
    while(iterator->next != nullptr) {
      iterator = iterator->next;
    }
    iterator->next = new Ingredient(ingd);
  }
  num_ingredients++;
}

void Roast::addEvent(Event evnt) {
  std::cout << "Hi Imma add event" << std::endl;
  if(events == nullptr) {
    std::cout << "No event so making one" << std::endl;
    events = new Event(evnt);
  }
  else {
    Event* iterator = events;
    while(iterator->next != nullptr) {
      iterator = iterator->next;
    }
    iterator->next = new Event(evnt);
  }
  std::cout << "Event added" << std::endl;
  num_events++;
}

// GET

long Roast::getId() const {
  return id;
}

long Roast::getTimestamp() const {
  return timestamp;
}

int Roast::getIngredientsCount() const {
  return num_ingredients;
}

int Roast::getEventCount() const {
  return num_events;
}

Ingredient& Roast::getIngredient(int index) const {
  if(ingredients != nullptr) {
    Ingredient* iterator = ingredients;
    int counter = 0;
    while(iterator != nullptr) {
      if(counter == index) {
        return *iterator;
      }
      counter++;
      iterator = iterator->next;
    }
  }
}

Event& Roast::getEvent(int index) const {
  std::cout << "Someone wants to getEvent at index: " << index << std::endl;
  if(events != nullptr) {
    Event* iterator = events;
    int counter = 0;
    while(iterator != nullptr) {
      if(counter == index) {
        std::cout << "Event returned" << std::endl;
        return *iterator;
      }
      counter++;
      iterator = iterator->next;
    }
  }
}

// REMOVE

void Roast::removeEventByTimestamp(long timestamp) {
  if(events == nullptr) {
    return;
  }
  else {
    Event** iterator = &events;
    while((*iterator) != nullptr) {
      if((*iterator)->getTimestamp() == timestamp) {
        auto old = (*iterator);
        (*iterator) = (*iterator)->next;
        delete old;
        return;
      }
      iterator = &((*iterator)->next);
    }
  }
  num_events--;
}

void Roast::removeIngredientByBeanName(std::string beanName) {
  if(ingredients == nullptr) {
    return;
  }
  else {
    Ingredient** iterator = &ingredients;
    while((*iterator) != nullptr) {
      if((*iterator)->getBean().getName() == beanName) {
        auto old = (*iterator);
        (*iterator) = (*iterator)->next;
        delete old;
        return;
      }
      iterator = &((*iterator)->next);
    }
  }
  num_ingredients--;
}


// DESTRUCTOR

Roast::~Roast() {
  if(events != nullptr) {
    while(events->next != nullptr) {
      auto ev = events;
      auto evTime = ev->getTimestamp();
      removeEventByTimestamp(evTime);
      delete ev;
    }
    delete events;
  }

  if(ingredients != nullptr) {
    while(ingredients->next != nullptr) {
      auto bn = ingredients->getBean();
      auto bnName = bn.getName();
      removeIngredientByBeanName(bnName);
    }
  }
  delete ingredients;
}
