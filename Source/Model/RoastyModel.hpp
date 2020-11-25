#pragma once
#include <string>

class Bean {
private:
  std::string name;
public:
  Bean(std::string& Name);
  std::string& getName();

  Bean& operator=(const Bean& copy);

};

class Ingredient {
private:
  Bean* bean;
  int amount;

public:
  Ingredient();
  Ingredient(Bean bean, int amount);
  Ingredient(const Ingredient& copy);
  Ingredient& operator=(const Ingredient& copy);
  Ingredient(Ingredient&& source);

  ~Ingredient();

  Bean getBean(std::string);
  int getAmount();

};

class Event {
private:
  std::string type;
  long timestamp;
  int eventValue;

public:
  Event();
  Event(std::string, long, int*);
  int getTimestamp();

};


class Roast {
private:
  int id;
  long timestamp;

  Ingredient* ingredients = new Ingredient[1];
  int max_ingredients;
  int num_ingredients;

  Event* events = new Event[1];
  int max_events;
  int num_events;


public:

  Roast(int, long);
  Roast(Roast const& copy);
  ~Roast();

  // Roast Add Functions
  void addIngredient(Ingredient);
  void addEvent(Event);

  // Roast Get Functions
  int getId();
  int getTimestamp();

  Ingredient getIngredient(int);
  Event getEvent(int);
  int getIngredientsCount();
  int getEventCount();

  // Roast Remove Functions
  void removeEventByTimestamp(long);
  void removeIngredientByBeanName(std::string);
};