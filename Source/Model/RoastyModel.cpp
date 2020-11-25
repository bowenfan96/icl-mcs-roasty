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
Ingredient::Ingredient() {}

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
Event::Event() {}

Event::Event(std::string Type, long Timestamp, int* p_eventValue)
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

Roast::Roast(int Id, long Timestamp)
    : id(Id), timestamp(Timestamp) {
}

Roast::Roast(Roast const& copy) {

}


Roast::~Roast() {
  delete[] ingredients;
  delete[] events;
}

// Roast Add Functions

void Roast::addIngredient(Ingredient ingredient) {
  if (num_ingredients == max_ingredients) {
    Ingredient* temp = new Ingredient[2 * max_ingredients];

    for (int i = 0; i < max_ingredients; i++) {
      temp[i] = ingredients[i];
    }

    delete[] ingredients;
    max_ingredients *= 2;
    ingredients = temp;   //okay cos this is a pointer
  }

  ingredients[num_ingredients] = ingredient;
  num_ingredients++;
}

void Roast::addEvent(Event event) {
  if (num_events == max_events) {
    Event* temp = new Event[2 * max_events];

    for (int i = 0; i < max_events; i++) {
      temp[i] = events[i];
    }

    delete[] events;
    max_events *= 2;
    events = temp;   //okay cos this is a pointer
  }

  events[num_events] = event;
  num_events++;
}

// Roast Get Functions

int Roast::getId() {
  return id;
}

int Roast::getTimestamp() {
  return timestamp;
}

Ingredient Roast::getIngredient(int index) {
  if (index < num_ingredients) {
    return ingredients[index];
  }
}

Event Roast::getEvent(int index) {
  if (index < num_events) {
    return events[index];
  }
}

int Roast::getIngredientsCount() {
  return num_ingredients;
}

int Roast::getEventCount() {
  return num_events;
}


// Roast Remove Functions

void Roast::removeEventByTimestamp(long event_timestamp) {
  int i;
  for(i = 0; i < num_events; i++) {
    if(events[i].getTimestamp() == event_timestamp) {
      break;
    }

    if(i < num_events) {
      for(int j = i; j < num_events - 1; j++) {
        events[j] = events[j + 1];
      }
    }

  }

}

void Roast::removeIngredientByBeanName(std::string bean_name) {

}

