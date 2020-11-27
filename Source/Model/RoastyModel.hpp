#pragma once
#include <string>

class Bean {
private:
  std::string name;

public:
  Bean(std::string name);

  std::string getName() const;

  Bean& operator=(std::string const& name);

};


class Ingredient {
private:
  Bean& bean;

  int amount;

public:
  Ingredient* next = nullptr;

  Ingredient(Bean& bean, int amount);


  Bean getBean() const;
  int getAmount() const;

};


struct EventValue {
  int value;
  int getValue();

  EventValue(int value);
};

class Event {
private:
  std::string type;
  long timestamp;
  EventValue* eventValue = nullptr;

public:
  Event* next = nullptr;

  Event(std::string type, long timestamp, EventValue* eventValue);
  Event(std::string type, long timestamp);

  long getTimestamp() const;
  std::string getType() const;
  bool hasValue() const;

  EventValue* getValue() const;

};


class Roast {
private:
  long id;
  long timestamp;

  Ingredient* ingredients = nullptr;

  int num_ingredients;

  Event* events = nullptr;
  int num_events;


public:

  Roast(long id, long timestamp);

  ~Roast();

  // Roast Add Functions
  void addIngredient(Ingredient ingd);
  void addEvent(Event evnt);

  // Roast Get Functions
  long getId() const;
  long getTimestamp() const;

  int getIngredientsCount() const;
  int getEventCount() const;

  Ingredient& getIngredient(int index) const;
  Event& getEvent(int index) const;

  // Roast Remove Functions
  void removeEventByTimestamp(long);
  void removeIngredientByBeanName(std::string);
};